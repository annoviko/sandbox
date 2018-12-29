from http.server import SimpleHTTPRequestHandler

import json
import time

from qsim.configuration import configuration
from qsim.logging import logging
from qsim.http_definition import http_method, http_code
from qsim.http_parser import http_parser, struct_field
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


    def do_GET(self):
        tas_host, tas_port = self.client_address[:2]
        
        logging.info("Receive HTTP GET request from TAS: '%s' (address: '%s:%s').", self.path, tas_host, tas_port)
        request = http_parser.parse(http_method.HTTP_GET, self.path)
        
        if request is None:
            self.__send_response(http_code.HTTP_BAD_REQUEST, "\"Impossible to parse GET request.\"")
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
        
        logging.info("Receive HTTP POST request from TAS: '%s' (address: '%s:%s').", self.path, tas_host, tas_port)
        request = http_parser.parse(http_method.HTTP_POST, self.path)
        
        if request is None:
            self.__send_response(http_code.HTTP_BAD_REQUEST, "\"Impossible to parse POST request.\"")
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

            task_id = self.__get_session_manager().create(request[struct_field.script_id], tas_request)
            if task_id is None:
                self.__send_response(http_code.HTTP_NOT_FOUND, "\"Session '" + str(task_id) + "' is not found.\"")
                return
            
            json_response = json_builder.start_qsim_response(task_id)
            self.__send_response(http_code.HTTP_OK_CREATED, json_response)
            
            self.__get_session_manager().launch(task_id)
        #
        # RESULT_ACTION
        #
        elif (request[struct_field.id] == tas_command_type.RESULT_COLLECT or
              request[struct_field.id] == tas_command_type.RESULT_PLAY):
            
            if configuration.get_failure_action_result_code() is not None:
                self.__send_response(configuration.get_failure_action_result_code(), None, configuration.get_failure_action_result_message())
                return
            
            session_id = request[struct_field.session_id]
            message_id = request[struct_field.id]
            # script_id   = request[struct_field.script_id];

            message_size = int(self.headers['Content-Length'])
            json_result = self.rfile.read(message_size)
            
            logging.info("Callback action result is received (id: '%s').", message_id)
            logging.debug("Content of the callback result:\n%s", json_result)
            
            # it is represented by map because most probably other staff may be conveyed to session.
            json_instance = None
            if message_size > 0:
                try:
                    json_instance = json.loads(json_result)

                except:
                    logging.error("Impossible to parse JSON - corrupted JSON payload is received.")
                    self.__send_response(http_code.HTTP_BAD_REQUEST, "\"Corrupted JSON payload in POST request.\"")
                    return
            
            message_playload = {'json' : json_instance}
            
            if (self.__get_session_manager().exist(session_id)):
                reply_code = self.__get_session_manager().notify(session_id, message_id, message_playload)

                if reply_code is None:
                    reply_code = http_code.HTTP_OK
                    reply_message = "\"Success.\""
                else:
                    reply_message = "\"Specified reply code is used.\""
                
                self.__send_response(reply_code, reply_message)
                return
            
            self.__send_response(http_code.HTTP_NOT_FOUND, "\"Session '" + str(session_id) + "' is not found.\"")
        
        else:
            self.__send_response(http_code.HTTP_BAD_REQUEST, "\"Unknown POST type command '" + str(request[struct_field.id]) + "'.\"")
            return


    def do_DELETE(self):
        tas_host, tas_port = self.client_address[:2]
        
        logging.info("Receive HTTP DELETE request from TAS: '%s' (address: '%s:%s').", self.path, tas_host, tas_port)
        request = http_parser.parse(http_method.HTTP_DELETE, self.path)
        
        if request is None:
            self.__send_response(http_code.HTTP_BAD_REQUEST, "\"Impossible to parse DELETE request.\"")
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
        # script_id   = request[struct_field.script_id];
        
        if self.__get_session_manager().exist(session_id) is True:
            reply_code = self.__get_session_manager().delete(session_id)
            if reply_code is None:
                reply_code = http_code.HTTP_OK_NO_CONTENT

            self.__send_response(reply_code)
        
        else:
            self.__send_response(http_code.HTTP_NOT_FOUND, "\"Session '" + str(session_id) + "' is not found.\"")


    def __send_response(self, http_code, json_data=None, http_message=None):
        time.sleep(configuration.get_response_delay() / 1000.0)
        
        self.send_response(http_code, http_message)
        self.send_header('Content-Type', 'application/json')
        
        body = None
        if json_data is not None:
            body = json_data.encode("utf-8")
            self.send_header('Content-Length', len(body))
            
        self.end_headers()
        
        if body is not None:
            self.wfile.write(body)
        
        statistical.inc_qsim_responses()
        
        logging.info("Send response to TAS (HTTP code '%d (%s)', HTTP body '%s')", http_code, str(http_message), body)
