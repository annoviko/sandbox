from ivrsim.code_block      import command_type;
from ivrsim.configuration   import configuration;
from ivrsim.logging         import logging;
from ivrsim.script          import script;


class session_context:
    # static counter
    __id_counter_generator = 1000000;


    def __init__(self, script_id, tas_request):
        self.__script_id    = script_id;
        self.__tas_info     = tas_request;
        self.__action_id    = None;
        
        self.__play_id      = None;
        self.__say_id       = None;
        self.__collect_id   = None;

        self.__session_id   = session_context.__generate_session_id();
        
        self.__cursor       = 0;
        self.__variables    = { };
        
        block = script.load(script_id);
        self.__actions      = block.get_commands();
        self.__triggers     = block.get_triggers();
        self.__functions    = block.get_functions();
        
        self.__last_input_message = None;
        
        if (len(self.__actions) == 0):
            logging.warning("Script-file (script id: '%s') does not have any actions for TAS.", script_id);

        logging.debug("IVR context is generated (session id: '%s', script id: '%s').", self.__session_id, self.__script_id);
        logging.debug("IVR script '%s' info (commands: '%d', triggers: '%d', functions: '%d').", self.__script_id, len(self.__actions), len(self.__triggers), len(self.__functions));


    def insert_code_block(self, code_block):
        self.__actions = self.__actions[:self.__cursor] + code_block + self.__actions[self.__cursor:];


    def set_last_input_message(self, message):
        self.__last_input_message = message;


    def get_last_input_message(self):
        return self.__last_input_message;


    def set_variable(self, name, value):
        self.__variables[name] = value;


    def get_variable(self, name):
        if (name not in self.__variables):
            logging.error("Impossible to get value of variable '%s' because it does not exist.", name);
            return None;

        return self.__variables[name];


    def get_variables(self):
        return self.__variables;


    def get_trigger(self, message):
        if (message in self.__triggers):
            return self.__triggers[message];
        
        return None;


    def set_play_id(self, play_id):
        self.__play_id = play_id;


    def get_play_id(self):
        return self.__play_id;


    def set_say_id(self, say_id):
        self.__say_id = say_id;


    def get_say_id(self):
        return self.__say_id;


    def set_collect_id(self, collect_id):
        self.__collect_id = collect_id;


    def get_collect_id(self):
        return self.__collect_id;


    def get_next_command(self):
        if (self.__cursor < len(self.__actions)):
            self.__cursor = self.__cursor + 1;
            return self.__actions[self.__cursor - 1];
        
        return None;


    def view_next_command(self):
        if (self.__cursor < len(self.__actions)):
            return self.__actions[self.__cursor];
        
        return None;


    def get_action_id(self):
        return self.__action_id;


    def get_id(self):
        return self.__session_id;


    def get_account_id(self):
        return self.__tas_info["account_id"];


    def get_script_id(self):
        return self.__script_id;


    def get_tas_address(self):
        return self.__tas_info["tas_address"]["ip"];


    def get_tas_port(self):
        return configuration.get_tas_port();


    def get_tas_link_play(self):
        return self.__tas_info["links"].get("play", None);


    def get_tas_link_stop_play(self):
        return self.__tas_info["links"].get("play", None) + "/" + str(self.get_play_id());


    def get_tas_link_say(self):
        return self.__tas_info["links"].get("say", None);


    def get_tas_link_stop_say(self):
        return self.__tas_info["links"].get("say", None) + "/" + str(self.get_say_id());


    def get_tas_link_collect(self):
        return self.__tas_info["links"].get("collect", None);


    def get_tas_link_stop_collect(self):
        return self.__tas_info["links"].get("collect", None) + "/" + str(self.get_collect_id());


    def get_tas_link_forward(self):
        return self.__tas_info["links"].get("forward", None);

    def get_tas_link_exception(self):
        return self.__tas_info["links"].get("exception", None);


    @staticmethod
    def __generate_session_id():
        session_context.__id_counter_generator += 1;
        return str(session_context.__id_counter_generator);
