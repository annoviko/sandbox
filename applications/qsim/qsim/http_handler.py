from http.server import SimpleHTTPRequestHandler

import json
import time

from qsim.configuration import configuration
from qsim.logging import logging
from qsim.event_queue import queue, event_start_response, event_response, event_ignore
from qsim.http_definition import http_method, http_code
from qsim.http_parser import http_parser, parser_failure, struct_field
from qsim.json_builder import json_builder
from qsim.messages import tas_command_type
from qsim.task_manager import task_manager
from qsim.statistical import statistical


class http_handler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        self.server_version = "QServiceSimulator/"
        self.sys_version = ""

        statistical.inc_tas_requests()

        SimpleHTTPRequestHandler.__init__(self, *args, **kwargs)


    def __get_session_manager(self):
        return task_manager


    def __parser_failure_to_http_code(self, failure):
        if failure.UNKNOWN_PATH: return http_code.HTTP_NOT_FOUND
        elif failure.INCORRECT_BODY: return http_code.HTTP_BAD_REQUEST
        elif failure.UNKNOWN_METHOD: return http_code.HTTP_METHOD_NOT_SUPPORTED
        else: return http_code.HTTP_BAD_REQUEST


    def do_GET(self):
        tas_host, tas_port = self.client_address[:2]
        
        logging.info("Receive HTTP GET request from TAS: '%s' (address: '%s:%s').", self.path, tas_host, tas_port)
        request = http_parser.parse(http_method.HTTP_GET, self.path)
        
        if isinstance(request, parser_failure):
            self.__send_response(self.__parser_failure_to_http_code(request), "\"Impossible to process GET request.\"")
            return
        
        #
        # EXECUTION_STATUS
        #
        if request[struct_field.id] == tas_command_type.EXECUTION_STATUS:
            if configuration.get_failure_execution_status_code() is not None:
                self.__send_response(configuration.get_failure_execution_status_code(), None, configuration.get_failure_execution_status_message())
                return

            session_id = request[struct_field.session_id]
            #script_id   = request[struct_field.script_id];
            
            if self.__get_session_manager().exist(session_id):
                json_response = json_builder.execution_status_response("Ok")
                self.__send_response(http_code.HTTP_OK, json_response)
            
            else:
                self.__send_response(http_code.HTTP_NOT_FOUND, "\"Session '" + str(session_id) + "' is not found.\"")
        
        #
        # SERVICE_STATUS
        #
        elif request[struct_field.id] == tas_command_type.SERVICE_STATUS:
            if (configuration.get_failure_service_status_code() is not None):
                self.__send_response(configuration.get_failure_service_status_code(), None, configuration.get_failure_service_status_message())
                return
            
            json_response = json_builder.service_status_response("Ok", { "amount_sessions" : task_manager.get_amount_sessions() })
            self.__send_response(http_code.HTTP_OK, json_response)
        
        else:
            self.__send_response(http_code.HTTP_BAD_REQUEST, "\"Unknown GET type command '" + str(request[struct_field.id]) + "'.\"")


    def do_POST(self):
        tas_host, tas_port = self.client_address[:2]
        
        logging.vip("Receive HTTP POST request from TAS: '%s' (address: '%s:%s').", self.path, tas_host, tas_port)
        request = http_parser.parse(http_method.HTTP_POST, self.path)
        
        if isinstance(request, parser_failure):
            self.__send_response(self.__parser_failure_to_http_code(request), "\"Impossible to process POST request.\"")
            return

        #
        # TASK_START
        #
        if request[struct_field.id] == tas_command_type.TASK_START:
            if configuration.get_failure_start_qsim_code() is not None:
                self.__send_response(configuration.get_failure_start_qsim_code(), None, configuration.get_failure_start_qsim_message())
                return

            # extract TAS request for session and attach additional information
            tas_request = {}
            json_request = self.rfile.read(int(self.headers['Content-Length'])).decode('utf-8')
            if (json_request is not None) and (len(json_request) > 0):
                tas_request = json.loads(json_request)

            tas_request["tas_address"] = {"ip": tas_host, "port": tas_port}
            tas_request["account_id"] = request[struct_field.account_id]

            # fill by default values
            tas_request["session_id"] = tas_request["sessionId"]
            tas_request["party_id"] = tas_request["inPartyId"]
            tas_request["q_party_id"] = tas_request["qPartyId"]
            tas_request["rcaccount_id"] = tas_request["account_id"]
            tas_request["rcextension_id"] = tas_request["account_id"]
            tas_request["rcbrand_id"] = "1210"

            logging.debug("QueueId (%s): Store default party ID (%s) and session ID (%s)." % (request[struct_field.script_id], tas_request["party_id"], tas_request["session_id"]))

            if self.__get_session_manager().create(request[struct_field.script_id], tas_request) is False:
                message = "Impossible to create session object due to lack of scenario file."
                logging.error(message)
                self.__send_response(http_code.HTTP_NOT_FOUND, "\"%s\"" % message)
                return

            try:
                response = queue.get(True, 2)
            except:
                response = None

            if response is None:
                message = "Event with response is not received."
                logging.error(message)
                self.__send_response(http_code.HTTP_INTERNAL_SERVER_ERROR, "\"%s\"" % message)
                return

            event_type = type(response)
            if event_type == event_start_response:
                self.__send_response(response.code, response.body, response.message, response.headers)

            elif event_type == event_ignore:
                pass

            else:
                message = "Unexpected event is received."
                logging.error(message)

        #
        # RESULT_ACTION
        #
        elif (request[struct_field.id] == tas_command_type.ON_COMMAND_UPDATE or
              request[struct_field.id] == tas_command_type.ON_COMMAND_ERROR):

            if configuration.get_failure_action_result_code() is not None:
                self.__send_response(configuration.get_failure_action_result_code(), None,
                                     configuration.get_failure_action_result_message())
                return

            message_size = int(self.headers['Content-Length'])
            json_result = self.rfile.read(message_size).decode('utf-8')

            # it is represented by map because most probably other staff may be conveyed to session.
            json_instance = None
            if message_size > 0:
                try:
                    json_instance = json.loads(json_result)
                except:
                    logging.error("Impossible to parse JSON - corrupted JSON payload is received.")
                    self.__send_response(http_code.HTTP_BAD_REQUEST, "\"Corrupted JSON payload in POST request.\"")
                    return

            message_playload = {'json': json_instance}

            action_type = request[struct_field.id]
            command_id = json_instance.get("commandId", None)
            if command_id is None:
                logging.error("Incorrect action request - commandId is not found in JSON.")
                self.__send_response(http_code.HTTP_BAD_REQUEST, "\"JSON body does not contain 'commandId' field.\"")
                return

            logging.info("Received callback id: '%s':", command_id)
            logging.debug(json_result)

            session_instance = self.__get_session_manager().get_session_by_command_id(command_id)

            if session_instance is not None:
                # check if code is returned by trigger
                reply_code = session_instance.notify(action_type, message_playload)
                #reply_code = self.__get_session_manager().notify(session_id, command_id, message_playload)

                if reply_code is None:
                    # if there is no trigger then let's take it from the queue, if there is user-specific code
                    try:
                        response = queue.get(True, 1)
                        event_type = type(response)
                        if event_type == event_ignore:
                            logging.debug("Ignore incoming request (do not sent response).")
                            return

                        reply_code = response.code
                        reply_message = response.message

                        logging.debug("Specific reply to incoming request '%s' (code: '%s', command ID: '%s')." %
                                      (command_id, reply_code, reply_message))

                    except:
                        # otherwise send default code
                        reply_code = http_code.HTTP_OK
                        reply_message = "\"Success.\""

                        logging.debug("Default reply is used for incoming request '%s'." % action_type)
                else:
                    reply_message = "\"Specified reply code is used.\""
                    logging.debug("Specific reply is used for incoming request '%s' via trigger "
                                  "(code: '%s', message: '%s')." % (command_id, reply_code, reply_message))

                self.__send_response(reply_code, reply_message)
                return

            self.__send_response(http_code.HTTP_NOT_FOUND, "\"Session for action '" + str(command_id) + "' is not found.\"")
            self.__get_session_manager().get_session_by_command_id(command_id)
        
        else:
            self.__send_response(http_code.HTTP_BAD_REQUEST, "\"Unknown POST type command '" + str(request[struct_field.id]) + "'.\"")
            return


    def do_DELETE(self):
        tas_host, tas_port = self.client_address[:2]
        
        logging.info("Receive HTTP DELETE request from TAS: '%s' (address: '%s:%s').", self.path, tas_host, tas_port)
        request = http_parser.parse(http_method.HTTP_DELETE, self.path)
        
        if isinstance(request, parser_failure):
            self.__send_response(self.__parser_failure_to_http_code(request), "\"Impossible to process DELETE request.\"")
            return
        
        #
        # TASK_STOP
        #
        if request[struct_field.id] != tas_command_type.TASK_STOP:
            self.__send_response(http_code.HTTP_BAD_REQUEST, "\"Unknown DELETE type command '" + str(request[struct_field.id]) + "'.\"")
            return

        if configuration.get_failure_stop_qsim_code() is not None:
            self.__send_response(configuration.get_failure_stop_qsim_code(), None, configuration.get_failure_stop_qsim_message())
            return

        session_id = request[struct_field.session_id]

        if self.__get_session_manager().exist(session_id) is True:
            # notify to check whether scenario contains something specific about termination
            self.__get_session_manager().notify(session_id, tas_command_type.TASK_STOP, None)

            try:
                response = queue.get(True, 1)
            except:
                response = None

            if response is None:    # scenario does not have anything specific about termination
                reply_code = self.__get_session_manager().delete(session_id)
                if reply_code is None:
                    reply_code = http_code.HTTP_OK_NO_CONTENT

                logging.info("Reply is not provided for TASK_STOP, use simulator response code (%d).", reply_code)
                self.__send_response(reply_code)

            else:
                event_type = type(response)
                if event_type == event_ignore:
                    pass

                elif event_type == event_response:
                    logging.info("Reply to TASK_STOP is provided by scenario (code: %d).", response.code)
                    self.__send_response(response.code, response.body, response.message, response.headers)

                else:
                    message = "Unexpected event is received."
                    logging.error(message)
        
        else:
            self.__send_response(http_code.HTTP_NOT_FOUND, "\"Session '" + str(session_id) + "' is not found.\"")


    def __send_response(self, http_code, json_data=None, http_message=None, headers=None):
        time.sleep(configuration.get_response_delay() / 1000.0)
        
        self.send_response(http_code, http_message)
        self.send_header('Content-Type', 'application/json')
        
        body = None
        if json_data is not None:
            body = json_data.encode("utf-8")
            self.send_header('Content-Length', len(body))


        if headers is not None:
            for key, value in headers.items():
                self.send_header(key, value)

        try:
            self.end_headers()
        
            if body is not None:
                self.wfile.write(body)

            statistical.inc_qsim_responses()
        
            logging.info("Send response to TAS (code '%d (%s)', body '%s')", http_code, str(http_message), body)

        except ConnectionResetError:
            logging.error("Impossible to send request to TAS due to reset connection.")

        except Exception as expection_object:
            logging.error("Impossible to send request to TAS due to unknown reason ('%s')." % expection_object)
