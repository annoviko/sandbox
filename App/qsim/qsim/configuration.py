import configparser
import os
import sys

from qsim.error_code import error_code
from qsim.logging import logging


class configuration:
    __configuration_filename = os.path.join(os.path.dirname(__file__), 'configuration.ini')
    __configuration_instance = None


    @staticmethod
    def load():
        if os.path.isfile(configuration.__configuration_filename) is False:
            logging.error("Impossible to find configuration file.")
            return sys.exit(error_code.ERROR_CONFIGURATION_NOT_FOUND)

        configuration.__configuration_instance = configparser.ConfigParser()
        configuration.__configuration_instance.read(configuration.__configuration_filename)
        
        return True


    @staticmethod
    def __get_section(section):
        parameter_pair = {}
        options = configuration.__configuration_instance.options(section)
        for option in options:
            try:
                parameter_pair[option] = configuration.__configuration_instance.get(section, option)

            except:
                parameter_pair[option] = None
        
        return parameter_pair


    @staticmethod
    def __get_boolean(section, key):
        line_value = configuration.__get_section(section)[key]
        if str(line_value).lower() == "true":
            return True

        return False


    @staticmethod
    def __get_integer(section, key):
        try:
            return int(configuration.__get_section(section)[key])
        except:
            logging.error("Impossible extract integer value '%s'.", configuration.__get_section(section)[key])
            return None


    @staticmethod
    def get_logging_file_enable():
        return configuration.__get_boolean("GENERAL", "logging_file_enable")


    @staticmethod
    def get_logging_file():
        return configuration.__get_section("GENERAL")['logging_file']


    @staticmethod
    def get_tas_port():
        return configuration.__get_section("NETWORK")['tas_port_server']


    @staticmethod
    def get_qsim_port():
        return configuration.__get_integer("NETWORK", "qsim_port_server")


    @staticmethod
    def get_failure_mode_enable():
        return configuration.__get_boolean("FAILURE_MODE", "failure_mode_enable")


    @staticmethod
    def __get_failure_descriptor(section, key):
        failure = [None, None]
        success = True

        failure_descriptor = ""
        if configuration.get_failure_mode_enable() is True:
            failure_descriptor = configuration.__get_section(section)[key].lstrip().rstrip()
            failure = failure_descriptor.split()
            if len(failure) == 2:
                try:
                    failure[0] = int(failure[0])
                except:
                    success = False
            
            elif len(failure) == 1:
                try:
                    failure[0] = int(failure[0])
                    failure.append("")
                
                except: 
                    success = False
        
            else:
                success = False
        
        if success is not True:
            logging.error("Impossible to extract failure descriptor '%s'.", failure_descriptor)
        
        return failure


    @staticmethod
    def __get_failure_code(section, key):
        return configuration.__get_failure_descriptor(section, key)[0]


    @staticmethod
    def __get_failure_message(section, key):
        return configuration.__get_failure_descriptor(section, key)[1]


    @staticmethod
    def get_failure_start_qsim_code():
        return configuration.__get_failure_code("FAILURE_MODE", "start_task_req")


    @staticmethod
    def get_failure_start_qsim_message():
        return configuration.__get_failure_message("FAILURE_MODE", "start_task_req")


    @staticmethod
    def get_failure_stop_qsim_code():
        return configuration.__get_failure_code("FAILURE_MODE", "stop_task_req")


    @staticmethod
    def get_failure_stop_qsim_message():
        return configuration.__get_failure_message("FAILURE_MODE", "stop_task_req")


    @staticmethod
    def get_failure_execution_status_code():
        return configuration.__get_failure_code("FAILURE_MODE", "execution_status_req")


    @staticmethod
    def get_failure_execution_status_message():
        return configuration.__get_failure_message("FAILURE_MODE", "execution_status_req")


    @staticmethod
    def get_failure_service_status_code():
        return configuration.__get_failure_code("FAILURE_MODE", "service_status_req")


    @staticmethod
    def get_failure_service_status_message():
        return configuration.__get_failure_message("FAILURE_MODE", "service_status_req")


    @staticmethod
    def get_failure_action_result_code():
        return configuration.__get_failure_code("FAILURE_MODE", "action_result_req")


    @staticmethod
    def get_failure_action_result_message():
        return configuration.__get_failure_message("FAILURE_MODE", "action_result_req")


    @staticmethod
    def get_response_delay():
        return configuration.__get_integer("NETWORK", "response_delay")
