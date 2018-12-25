import re

from qsim.http_definition import http_method
from qsim.logging import logging
from qsim.messages import tas_command_type


class struct_field:
    id = "id"
    account_id = "account_id"
    script_id = "script_id"
    session_id = "session_id"
    action_id = "action_id"


class http_parser:
    @staticmethod
    def parse(method, path):
        if method == http_method.HTTP_POST:
            reg_object = re.match("/telephony/v1/account/(\d+)/services/queue/(\d+)/voice/tasks", path)
            if (reg_object):
                return http_parser.extract_qsim_start_request(reg_object)
            
            reg_object = re.match("/telephony/v1/account/(\d+)/services/queue/(\d+)/voice/tasks/(\d+)/actions/(.*)", path)
            if (reg_object):
                return http_parser.extract_tas_action_result(reg_object)
        
        elif method == http_method.HTTP_DELETE:
            reg_object = re.match("/telephony/v1/account/(\d+)/services/queue/(\d+)/voice/tasks/(\d+)", path)
            if reg_object:
                return http_parser.extract_qsim_stop_request(reg_object)
            
        elif method == http_method.HTTP_GET:
            reg_object = re.match("/telephony/v1/account/(\d+)/services/queue/(\d+)/instances/(\d+)", path)
            if reg_object:
                return http_parser.extract_qsim_execution_status(reg_object)
        
            reg_object = re.match("/telephony/v1/account/(\d+)/services/status(.*)", path)
            if reg_object:
                return http_parser.extract_qsim_service_status(reg_object)
        
        else:
            logging.error("HTTP method '%s' is not supported.", method)
        
        return None


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
    def extract_tas_action_result(reg_object):
        action_line = reg_object.group(4)
        action_reg_object = re.match("(\S+)/(\d+)", action_line)
        if action_reg_object:
            action = action_reg_object.group(1)
            message_id = None
            
            if action == "play":
                message_id = tas_command_type.RESULT_PLAY
            
            elif action == "collect":
                message_id = tas_command_type.RESULT_COLLECT
            
            else:
                logging.error("Unknown result action from TAS is detected '%s'.", action)
                return None
            
            return {struct_field.id:           message_id,
                    struct_field.account_id:   reg_object.group(1),
                    struct_field.script_id:    reg_object.group(2),
                    struct_field.session_id:   reg_object.group(3),
                    struct_field.action_id:    action_reg_object.group(2)}
        else:
            logging.error("Unknown result action format from TAS is detected '%s'.", action_line)
            return None
