from qsim.configuration import configuration
from qsim.logging import logging
from qsim.script import script


class task_context:
    # static counter
    __id_counter_generator = 1820000


    def __init__(self, queue_id, tas_request):
        self.__script_id = queue_id
        self.__tas_info = tas_request
        self.__action_id = None
        
        self.__play_id = None
        self.__collect_id = None

        self.__task_id = task_context.__generate_task_id()
        
        self.__cursor = 0
        self.__variables = {}
        
        block = script.load(queue_id)
        self.__actions = block.get_commands()
        self.__triggers = block.get_triggers()
        self.__functions = block.get_functions()
        
        self.__last_input_message = None
        
        if len(self.__actions) == 0:
            logging.warning("Script-file (script id: '%s') does not have any actions for TAS.", queue_id)

        logging.debug("Q Service context is generated (task id: '%s', script id: '%s').", self.__task_id, self.__script_id)
        logging.debug("Q Service script '%s' info (commands: '%d', triggers: '%d', functions: '%d').", self.__script_id, len(self.__actions), len(self.__triggers), len(self.__functions))


    def insert_code_block(self, code_block):
        self.__actions = self.__actions[:self.__cursor] + code_block + self.__actions[self.__cursor:]


    def set_last_input_message(self, message):
        self.__last_input_message = message


    def set_session_id(self, value):
        self.__tas_info["session_id"] = value


    def set_party_id(self, value):
        self.__tas_info["party_id"] = value

    def set_rcaccount_id(self, value):
        self.__tas_info["rcaccount_id"] = value

    def set_rcextension_id(self, value):
        self.__tas_info["rcextension_id"] = value

    def get_last_input_message(self):
        return self.__last_input_message


    def set_variable(self, name, value):
        self.__variables[name] = value


    def get_variable(self, name):
        if name not in self.__variables:
            logging.error("Impossible to get value of variable '%s' because it does not exist.", name)
            return None

        return self.__variables[name]


    def get_variables(self):
        return self.__variables


    def get_trigger(self, message):
        if message in self.__triggers:
            return self.__triggers[message]
        
        return None


    def set_play_id(self, play_id):
        self.__play_id = play_id


    def get_play_id(self):
        return self.__play_id


    def set_collect_id(self, collect_id):
        self.__collect_id = collect_id


    def get_collect_id(self):
        return self.__collect_id


    def get_next_command(self):
        if self.__cursor < len(self.__actions):
            self.__cursor = self.__cursor + 1
            return self.__actions[self.__cursor - 1]
        
        return None


    def view_next_command(self):
        if self.__cursor < len(self.__actions):
            return self.__actions[self.__cursor]
        
        return None


    def get_action_id(self):
        return self.__action_id


    def get_id(self):
        return self.__task_id


    def get_account_id(self):
        return self.__tas_info["account_id"]


    def get_queue_id(self):
        return self.__script_id

    def get_rcaccount_id(self):
        return self.__tas_info.get("rcaccount_id")

    def get_rcextension_id(self):
        return self.__tas_info.get("rcextension_id")

    def get_tas_address(self):
        return self.__tas_info["tas_address"]["ip"]


    def get_tas_port(self):
        return configuration.get_tas_port()


    def get_tas_link_session(self):
        return "%s:%s/telephony/v1/account/%s/sessions/%s" % (self.get_tas_address(), str(self.get_tas_port()), self.__tas_info["account_id"], self.__tas_info["session_id"])


    def get_tas_link_party(self):
        return "%s/parties/%s" % (self.get_tas_link_session(), self.__tas_info["party_id"])


    def get_tas_link_start_play(self):
        return "%s/play" % (self.get_tas_link_party())


    def get_tas_link_stop_play(self):
        return "%s/play/%s" % (self.get_tas_link_party(), str(self.get_play_id()))


    def get_tas_link_start_collect(self):
        return "%s/collect" % (self.get_tas_link_party())


    def get_tas_link_stop_collect(self):
        return "%s/collect/%s" % (self.get_tas_link_party(), str(self.get_collect_id()))


    def get_tas_link_forward(self):
        return "%s/forward" % (self.get_tas_link_party())


    def get_task_link_hunt(self):
        return "%s/hunt" % (self.get_tas_link_session())


    @staticmethod
    def __generate_task_id():
        task_context.__id_counter_generator += 1
        return str(task_context.__id_counter_generator)
