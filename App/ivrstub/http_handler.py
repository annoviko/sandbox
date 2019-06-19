from http.server import SimpleHTTPRequestHandler

import time

from ivrstub.configuration import configuration
from ivrstub.logging import logging
from ivrstub.http_parser import http_parser
from ivrstub.messages import tas_command_type

class http_handler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        self.server_version = "IVRStub/"
        self.sys_version = ""

        SimpleHTTPRequestHandler.__init__(self, *args, **kwargs)


    def do_GET(self):
        self.__common_handler('GET')


    def do_POST(self):
        tas_host, tas_port = self.client_address[:2]
        
        logging.info("Receive HTTP POST request from TAS: '%s' ('%s:%s').", self.path, tas_host, tas_port)
        tas_command = http_parser.parse('POST', self.path)

        if tas_command == tas_command_type.task_start:
            self.__send_response(configuration.get_failure_start_ivr_code(), None,
                                 configuration.get_failure_start_ivr_message())

        elif tas_command == tas_command_type.task_stop:
            self.__send_response(configuration.get_failure_stop_ivr_code(), None,
                                 configuration.get_failure_stop_ivr_message())

        else:
            self.__send_response(configuration.get_failure_any_code(), None, configuration.get_failure_any_message())


    def do_DELETE(self):
        self.__common_handler('DELETE')


    def __common_handler(self, method):
        tas_host, tas_port = self.client_address[:2]

        logging.info("Receive HTTP %s request from TAS: '%s' ('%s:%s').", method, self.path, tas_host, tas_port)
        self.__send_response(configuration.get_failure_any_code(), None, configuration.get_failure_any_message())


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
        
            logging.info("Send response to TAS (code '%d (%s)', body '%s')", http_code, str(http_message), body)

        except ConnectionResetError:
            logging.error("Impossible to send request to TAS due to reset connection.")

        except Exception as expection_object:
            logging.error("Impossible to send request to TAS due to unknown reason ('%s')." % expection_object)
