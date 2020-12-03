import configparser
import os
import re
import sys

from ivrstub.error_code import error_code
from ivrstub.logging import logging


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
    def get_ivr_port():
        return configuration.__get_integer("NETWORK", "ivr_port_server")


    @staticmethod
    def __get_failure_descriptor(section, key):
        failure_descriptor = configuration.__get_section(section)[key].lstrip().rstrip()
        reg_object = re.match("(\d+) (.*)", failure_descriptor)
        if reg_object:
            return [int(reg_object.group(1)), reg_object.group(2)]

        else:
            logging.error("Impossible to extract failure descriptor '%s'.", failure_descriptor)
        
        return [999, "Internal IVRStub Error"]


    @staticmethod
    def __get_failure_code(section, key):
        return configuration.__get_failure_descriptor(section, key)[0]


    @staticmethod
    def __get_failure_message(section, key):
        return configuration.__get_failure_descriptor(section, key)[1]


    @staticmethod
    def get_failure_start_ivr_code():
        return configuration.__get_failure_code("FAILURE_MODE", "start_task_req")


    @staticmethod
    def get_failure_start_ivr_message():
        return configuration.__get_failure_message("FAILURE_MODE", "start_task_req")


    @staticmethod
    def get_failure_stop_ivr_code():
        return configuration.__get_failure_code("FAILURE_MODE", "stop_task_req")


    @staticmethod
    def get_failure_stop_ivr_message():
        return configuration.__get_failure_message("FAILURE_MODE", "stop_task_req")


    @staticmethod
    def get_failure_any_code():
        return configuration.__get_failure_code("FAILURE_MODE", "any_req")


    @staticmethod
    def get_failure_any_message():
        return configuration.__get_failure_message("FAILURE_MODE", "any_req")


    @staticmethod
    def get_response_delay():
        return configuration.__get_integer("NETWORK", "response_delay")
