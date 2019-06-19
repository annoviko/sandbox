import re

from enum import IntEnum

from qsim.http_definition import http_method
from qsim.logging import logging
from qsim.messages import tas_command_type


class struct_field:
    id = "id"
    account_id = "account_id"
    script_id = "script_id"
    session_id = "session_id"
    action_id = "action_id"


class parser_failure(IntEnum):
    UNKNOWN_PATH = 1
    INCORRECT_BODY = 2
    UNKNOWN_METHOD = 3


class http_parser:
    @staticmethod
    def parse(method, path):
        if method == http_method.HTTP_POST:
            reg_object = re.match("/telephony/v1/account/(\d+)/services/queue/(\d+)/voice/tasks$", path)
            if (reg_object):
                return http_parser.extract_qsim_start_request(reg_object)

            if re.match(".*/on-command-error", path):
                return http_parser.extract_tas_error_callback()
            if re.match(".*/on-command-update", path):
                return http_parser.extract_tas_update_callback()

            #just a backward compatibility. To be removed after all callbacks will be updated at TAS
            reg_object = re.match("/telephony/v1/account/(\d+)/services/queue/(\d+)/voice/tasks/(\d+)/actions/(.*)", path)
            if (reg_object):
                return http_parser.extract_tas_action_result(reg_object)

        elif method == http_method.HTTP_DELETE:
            reg_object = re.match("/telephony/v1/account/(\d+)/services/queue/(\d+)/voice/tasks/(\d+)", path)
            if reg_object:
                return http_parser.extract_qsim_stop_request(reg_object)
            
        elif method == http_method.HTTP_GET:
            reg_object = re.match("/telephony/v1/account/(\d+)/services/queue/(\d+)/voice/tasks/(\d+)", path)
            if reg_object:
                return http_parser.extract_qsim_execution_status(reg_object)
        
            reg_object = re.match("/telephony/v1/account/(\d+)/services/status(.*)", path)
            if reg_object:
                return http_parser.extract_qsim_service_status(reg_object)
        
        else:
            logging.error("HTTP method '%s' is not supported.", method)
            return parser_failure.UNKNOWN_METHOD

        return parser_failure.UNKNOWN_PATH


    @staticmethod
    def extract_qsim_start_request(reg_object):
        return {struct_field.id:          tas_command_type.TASK_START,
                struct_field.account_id:  reg_object.group(1),
                struct_field.script_id:   reg_object.group(2)}


    @staticmethod
    def extract_qsim_stop_request(reg_object):
        return {struct_field.id:          tas_command_type.TASK_STOP,
                struct_field.account_id:  reg_object.group(1),
                struct_field.script_id:   reg_object.group(2),
                struct_field.session_id:  reg_object.group(3)}


    @staticmethod
    def extract_qsim_execution_status(reg_object):
        return {struct_field.id:          tas_command_type.EXECUTION_STATUS,
                struct_field.account_id:  reg_object.group(1),
                struct_field.script_id:   reg_object.group(2),
                struct_field.session_id:  reg_object.group(3)}


    @staticmethod
    def extract_qsim_service_status(reg_object):
        return {struct_field.id:          tas_command_type.SERVICE_STATUS,
                struct_field.account_id:  reg_object.group(1)}

    @staticmethod
    def extract_tas_error_callback():
        return {struct_field.id:    tas_command_type.ON_COMMAND_ERROR}

    @staticmethod
    def extract_tas_update_callback():
        return {struct_field.id:    tas_command_type.ON_COMMAND_UPDATE}

    @staticmethod
    def extract_tas_action_result(reg_object):
        action_line = reg_object.group(4)
        action_reg_object = re.match("(\S+)", action_line)
        if action_reg_object:
            action = action_reg_object.group(1)

            if action == "collects":
                message_id = tas_command_type.RESULT_COLLECT

            elif action == "result_forward_group":
                message_id = tas_command_type.RESULT_FORWARD_GROUP

            elif action == "notify_forward_group":
                message_id = tas_command_type.NOTIFY_FORWARD_GROUP

            else:
                logging.error("Unknown result action from TAS is detected '%s'.", action)
                return parser_failure.UNKNOWN_PATH


            return {struct_field.id:            message_id,
                    struct_field.account_id:    reg_object.group(1),
                    struct_field.script_id:     reg_object.group(2),
                    struct_field.session_id:    reg_object.group(3),
                    struct_field.action_id:     0}
        else:
            logging.error("Unknown result action format from TAS is detected '%s'.", action_line)
            return parser_failure.INCORRECT_BODY
