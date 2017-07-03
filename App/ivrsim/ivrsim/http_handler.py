from SimpleHTTPServer           import SimpleHTTPRequestHandler;

import json;
import time;

from ivrsim.configuration       import configuration;
from ivrsim.logging             import logging;
from ivrsim.http_definition     import http_method, http_code;
from ivrsim.http_parser         import http_parser, struct_field;
from ivrsim.json_builder        import json_builder;
from ivrsim.messages            import tas_command_type;
from ivrsim.session_manager     import session_manager;
from ivrsim.statistical         import statistical;


class http_handler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        self.server_version = "IvrSimulator/";
        self.sys_version = "";

        statistical.inc_tas_requests();

        SimpleHTTPRequestHandler.__init__(self, *args, **kwargs);


    def __get_session_manager(self):
        return session_manager;


    def do_GET(self):
        tas_host, tas_port = self.client_address[:2];
        
        logging.info("Receive HTTP GET request from TAS: '%s' (address: '%s:%s').", self.path, tas_host, tas_port);
        request = http_parser.parse(http_method.HTTP_GET, self.path);
        
        if (request == None):
            self.__send_response(http_code.HTTP_BAD_REQUEST, "\"Impossible to parse GET request.\"");
            return;
        
        #
        # TAS_IVR_EXECUTION_STATUS
        #
        if (request[struct_field.id] == tas_command_type.EXECUTION_STATUS):
            if (configuration.get_failure_execution_status_code() is not None):
                self.__send_response(configuration.get_failure_execution_status_code(), None, configuration.get_failure_execution_status_message());
                return;

            session_id  = request[struct_field.session_id];
            #script_id   = request[struct_field.script_id];
            
            if (self.__get_session_manager().exist(session_id)):
                json_response = json_builder.execution_status_response("Ok");
                self.__send_response(http_code.HTTP_OK, json_response);
            
            else:
                self.__send_response(http_code.HTTP_NOT_FOUND, "\"Session '" + str(session_id) + "' is not found.\"");
        
        #
        # TAS_IVR_SERVICE_STATUS
        #
        elif (request[struct_field.id] == tas_command_type.SERVICE_STATUS):
            if (configuration.get_failure_service_status_code() is not None):
                self.__send_response(configuration.get_failure_service_status_code(), None, configuration.get_failure_service_status_message());
                return;
            
            json_response = json_builder.service_status_response("Ok", { "amount_sessions" : session_manager.get_amount_sessions() });
            self.__send_response(http_code.HTTP_OK, json_response);
        
        else:
            self.__send_response(http_code.HTTP_BAD_REQUEST, "\"Unknown GET type command '" + str(request[struct_field.id]) + "'.\"");


    def do_POST(self):
        tas_host, tas_port = self.client_address[:2];
        
        logging.info("Receive HTTP POST request from TAS: '%s' (address: '%s:%s').", self.path, tas_host, tas_port);
        request = http_parser.parse(http_method.HTTP_POST, self.path);
        
        if (request == None):
            self.__send_response(http_code.HTTP_BAD_REQUEST, "\"Impossible to parse POST request.\"");
            return;
            
        #
        # TAS_IVR_START_REQ
        #
        if (request[struct_field.id] == tas_command_type.IVR_START):
            if (configuration.get_failure_start_ivr_code() is not None):
                self.__send_response(configuration.get_failure_start_ivr_code(), None, configuration.get_failure_start_ivr_message());
                return;

            # extract TAS request for session and attach additional information
            json_request = self.rfile.read(int(self.headers['Content-Length']));
            tas_request = json.loads(json_request);
            tas_request["tas_address"] = { "ip": tas_host, "port": tas_port };
            tas_request["account_id"] = request[struct_field.account_id];
            
            session_id = self.__get_session_manager().create(request[struct_field.script_id], tas_request);
            if (session_id is None):
                self.__send_response(http_code.HTTP_NOT_FOUND, "\"Session '" + str(session_id) + "' is not found.\"");
                return;
            
            json_response = json_builder.start_ivr_response(session_id);
            self.__send_response(http_code.HTTP_OK_CREATED, json_response);
            
            self.__get_session_manager().launch(session_id);
        #
        # RESULT_ACTION
        #
        elif ( request[struct_field.id] == tas_command_type.RESULT_COLLECT or
               request[struct_field.id] == tas_command_type.RESULT_PLAY or
               request[struct_field.id] == tas_command_type.RESULT_SAY      ):
            
            if (configuration.get_failure_action_result_code() is not None):
                self.__send_response(configuration.get_failure_action_result_code(), None, configuration.get_failure_action_result_message());
                return;
            
            session_id  = request[struct_field.session_id];
            message_id  = request[struct_field.id];
            # script_id   = request[struct_field.script_id];
            
            json_result = self.rfile.read(int(self.headers['Content-Length']));
            
            logging.info("Callback action result is received (id: '%d').", request[struct_field.id]);
            logging.debug("Content of the callback result:\n%s", json_result);
            
            # it is represented by map because most probably other staff may be conveyed to session.
            json_instance = None;
            try:    
                json_instance = json.loads(json_result);
                
            except: 
                logging.error("Impossible to parse JSON - corrupted JSON payload is received.");
                self.__send_response(http_code.HTTP_BAD_REQUEST, "\"Corrupted JSON payload in POST request.\"");
                return;
            
            message_playload = { 'json' : json_instance };
            
            if (self.__get_session_manager().exist(session_id)):
                reply_code = self.__get_session_manager().notify(session_id, message_id, message_playload);
                
                reply_message = None;
                if (reply_code is None):
                    reply_code = http_code.HTTP_OK;
                    reply_message = "\"Success.\"";
                else:
                    reply_message = "\"Specified reply code is used.\"";
                
                self.__send_response(reply_code, reply_message);
                return;
            
            self.__send_response(http_code.HTTP_NOT_FOUND, "\"Session '" + str(session_id) + "' is not found.\"");
        
        else:
            self.__send_response(http_code.HTTP_BAD_REQUEST, "\"Unknown POST type command '" + str(request[struct_field.id]) + "'.\"");
            return;


    def do_DELETE(self):
        tas_host, tas_port = self.client_address[:2];
        
        logging.info("Receive HTTP DELETE request from TAS: '%s' (address: '%s:%s').", self.path, tas_host, tas_port);
        request = http_parser.parse(http_method.HTTP_DELETE, self.path);
        
        if (request == None):
            self.__send_response(http_code.HTTP_BAD_REQUEST, "\"Impossible to parse DELETE request.\"");
            return;
        
        #
        # TAS_IVR_STOP_REQ
        #
        if (request[struct_field.id] != tas_command_type.IVR_STOP):
            self.__send_response(http_code.HTTP_BAD_REQUEST, "\"Unknown DELETE type command '" + str(request[struct_field.id]) + "'.\"");
            return;

        if (configuration.get_failure_stop_ivr_code() is not None):
            self.__send_response(configuration.get_failure_stop_ivr_code(), None, configuration.get_failure_stop_ivr_message());
            return;

        session_id  = request[struct_field.session_id];
        # script_id   = request[struct_field.script_id];
        
        if (self.__get_session_manager().exist(session_id) is True):
            reply_code = self.__get_session_manager().delete(session_id);
            if (reply_code is None):
                reply_code = http_code.HTTP_OK_DELETED;
            
            json_response = json_builder.stop_ivr_response();
            self.__send_response(reply_code, json_response);
        
        else:
            self.__send_response(http_code.HTTP_NOT_FOUND, "\"Session '" + str(session_id) + "' is not found.\"");


    def __send_response(self, http_code, json_data = None, http_message = None):
        time.sleep(configuration.get_response_delay() / 1000.0);
        
        self.send_response(http_code, http_message);
        self.send_header('Content-Type', 'application/json');
        
        body = None;
        if (json_data is not None):
            body = json_data.encode("utf-8");
            self.send_header('Content-Length', len(body));
            
        self.end_headers();
        
        if (body is not None):
            self.wfile.write(body);
        
        statistical.inc_ivr_responses();
        
        logging.info("Send response to TAS (HTTP code '%d (%s)', HTTP body '%s')", http_code, str(http_message), body);
