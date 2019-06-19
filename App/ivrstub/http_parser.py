import re

from ivrstub.messages import tas_command_type

class http_parser:
    @staticmethod
    def parse(method, path):
        if method == 'POST':
            reg_object = re.match("/ivr/v1/callback/on-call-enter", path)
            if reg_object:
                return tas_command_type.task_start

            reg_object = re.match("/ivr/v1/callback/on-call-exit", path)
            if reg_object:
                return tas_command_type.task_stop

        return tas_command_type.any
