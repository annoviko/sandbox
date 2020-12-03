from ivrsim.code_block              import command_type;
from ivrsim.expression_analyser     import expression_analyser;
from ivrsim.http_client             import http_client;
from ivrsim.logging                 import logging;
from ivrsim.messages                import tas_command_type;
from ivrsim.script                  import script;
from ivrsim.session_context         import session_context;

import re;
import json;
import threading;
import time;


class session(object):
    def __new__(cls, script_id, tas_request, manager):
        if (script.exist(script_id) is False):
            logging.warning("Script-file (script id: '%s') is not found - reject to create session.", script_id);
            return None;

        instance = object.__new__(cls);
        return instance;


    def __init__(self, script_id, tas_request, manager):
        self.__context = session_context(script_id, tas_request);
        self.__active = True;
        self.__manager = manager;
        
        self.__code_block_locker = threading.RLock();
        
        self.__tas_response_condition = threading.Condition();
        self.__tas_responses = [ ];


    def start(self):
        self.__active = True;
        thread_session = threading.Thread(target = self.__run);
        thread_session.daemon = True;
        thread_session.start();


    def stop(self):
        with self.__code_block_locker:
            self.__context.insert_code_block([ (command_type.COMMAND_EXIT, "") ]);
            reply_code = self.notify(tas_command_type.IVR_STOP);
        
        logging.info("Stop command from session manager is received (reply code: '%s').", str(reply_code));
        
        return reply_code;


    def get_script_id(self):
        return self.__context.get_script_id();


    def get_id(self):
        return self.__context.get_id();


    def notify(self, tas_notification_id, message_payload = None):
        reply_code = None;
        string_message_id = tas_command_type.to_string(tas_notification_id);
        
        with self.__code_block_locker:
            trigger = self.__context.get_trigger(string_message_id);
            if (trigger is not None):
                self.__context.insert_code_block(trigger.commands);
                reply_code = trigger.reply_code;
                
                logging.info("Code block for the message '%s' is found (reply code: '%s').", string_message_id, str(reply_code));

        with self.__tas_response_condition:
            self.__context.set_last_input_message((tas_notification_id, message_payload));
            self.__tas_responses.append(string_message_id);
            self.__tas_response_condition.notify_all();
        
        return reply_code;


    def __send(self, tas_method, tas_link, tas_content):
        tas_client = http_client(self.__context.get_tas_address(), self.__context.get_tas_port());
        return tas_client.send_request(tas_method, tas_link, tas_content);


    def __run(self):
        logging.debug("(IVR session '%s') session '%s' is started at '%s'.", self.__context.get_id(), self.__context.get_id(), time.ctime());
        
        with self.__code_block_locker:
            command_container = self.__context.get_next_command();
        
        while( (self.__active is True) and (command_container is not None) ):
            (command, arguments) = command_container;
            
            if (command == command_type.COMMAND_SEND):
                self.__process_send_command(arguments[0], arguments[1], arguments[2]);
            
            elif (command == command_type.COMMAND_TIMEOUT):
                self.__process_timeout_command(arguments[0]);
            
            elif (command == command_type.COMMAND_WAIT):
                self.__process_wait_command(arguments[0]);
            
            elif (command == command_type.COMMAND_PRINT):
                self.__process_print_comand(arguments[0]);
            
            elif (command == command_type.COMMAND_EXIT):
                logging.info("(IVR session '%s') exit command is detected - termination...", self.__context.get_id());
                self.__active = False;
            
            elif (command == command_type.COMMAND_MOVE_JSON):
                self.__process_move_json(arguments[0]);
            
            elif (command == command_type.COMMAND_IF):
                self.__process_conditional_block(arguments[0], arguments[1]);
            
            else:
                logging.error("(IVR session '%s') unexpected command is detected...", self.__context.get_id());
            
            with self.__code_block_locker:
                command_container = self.__context.get_next_command();
        
        self.__manager.delete(self.__context.get_id(), internal = True);
        logging.info("(IVR session '%s') session is terminated at '%s'.", self.__context.get_id(), time.ctime());


    def __process_wait_command(self, expected_response):
        logging.debug("(IVR session '%s') wait for TAS callback command (response: '%s')...", self.__context.get_id(), expected_response);
        
        while(self.__active is True):
            response = "";
            
            with self.__tas_response_condition:
                while (len(self.__tas_responses) == 0 and self.__active is True):
                    self.__tas_response_condition.wait();

                if (len(self.__tas_responses) > 0):
                    response = self.__tas_responses.pop();
            
            if (response != expected_response):
                logging.warning("(IVR session '%s') unexpected response is received from TAS: '%s' (but expected: '%s'), continue to wait.", self.__context.get_id(), response, expected_response);
                continue;
            
            else:
                logging.debug("(IVR session '%s') expected response is received '%s', stop waiting process.", self.__context.get_id(), response);
                break;


    def __process_timeout_command(self, timeout):
        logging.debug("(IVR session '%s') command TIMEOUT is executing (time: '%d')...", self.__context.get_id(), timeout);
        time.sleep(timeout / 1000.0);


    def __process_print_comand(self, text):
        logging.user(text);


    def __process_send_command(self, method, tas_content, arguments):
        logging.debug("(IVR session '%s') command SEND is executing...", self.__context.get_id());
        
        tas_link = None;
        tas_method = None;
        
        if (method == "SAY"):
            tas_link = self.__context.get_tas_link_say();
            tas_method = "POST";
        
        elif (method == "STOP_SAY"):
            tas_link = self.__context.get_tas_link_stop_say();
            tas_method = "DELETE";
        
        elif (method == "PLAY"):
            tas_link = self.__context.get_tas_link_play();
            tas_method = "POST";
        
        elif (method == "STOP_PLAY"):
            tas_link = self.__context.get_tas_link_stop_play();
            tas_method = "DELETE";
        
        elif (method == "COLLECT"):
            tas_link = self.__context.get_tas_link_collect();
            tas_method = "POST";
        
        elif (method == "STOP_COLLECT"):
            tas_link = self.__context.get_tas_link_stop_collect();
            tas_method = "DELETE";
        
        elif (method == "FORWARD"):
            tas_link = self.__context.get_tas_link_forward();
            tas_method = "PUT";

        elif (method == "EXCEPTION"):
            tas_link = self.__context.get_tas_link_exception();
            tas_method = "PUT";

        if (len(arguments) > 0):
            tas_link_pattern = arguments[0];
            tas_link = self.__change_tas_link(tas_link, tas_link_pattern);
        
        tas_content = self.__translate_content(tas_content);
        
        logging.info("(IVR session '%s') send command request to TAS ('%s', '%s').", self.__context.get_id(), tas_method, method);
        (status, json_response) = self.__send(tas_method, tas_link, tas_content);
        if ( (status >= 200) and (status <= 299) ):
            if (tas_method == "POST"):
                response = json.loads(json_response);
                if (response.get("id", None) == None):
                    logging.error("(IVR session '%s') TAS reply to '%s' command '%s' without JSON body with 'id' key.", self.__context.get_id(), tas_method, method);
                
                else:
                    action_id = response['id'];
                    
                    if (method == "SAY"): self.__context.set_say_id(action_id);
                    elif (method == "PLAY"): self.__context.set_play_id(action_id);
                    elif (method == "COLLECT"): self.__context.set_collect_id(action_id);
                    
                    logging.info("(IVR session '%s') TAS accepts '%s' command '%s' and return action id: '%s'.", self.__context.get_id(), tas_method, method, action_id)
        
        else:
            logging.warning("(IVR session '%s') TAS reply to '%s' command '%s' by failure status (code: '%d').", self.__context.get_id(), tas_method, method, status);


    def __process_move_json(self, name):
        message_payload = self.__context.get_last_input_message()[1];
        self.__context.set_variable(name, message_payload['json']);


    def __process_conditional_block(self, expression, code_block):
        next_command_container = self.__context.view_next_command();
        
        condition_analyser = expression_analyser(expression, self.__context);
        condition_result = condition_analyser.evaluate();
        
        if ( (next_command_container is not None) and (next_command_container[0] == command_type.COMMAND_ELSE) ):
            # in this case we need to skip ELSE block - move cursor (iterator) - it should be found by general switch.
            self.__context.get_next_command();
            
            if (condition_result is True):
                self.__context.insert_code_block(code_block);
            
            else:
                else_code_block = next_command_container[1][0];
                self.__context.insert_code_block(else_code_block);
        
        else:
            self.__context.insert_code_block(code_block);


    def __translate_content(self, tas_content):
        content = tas_content;
        if ( (tas_content is not None) and (len(tas_content) > 0) ):
            content = content.replace("$(accountId)", self.__context.get_account_id());
            content = content.replace("$(scriptId)", self.__context.get_script_id());
            content = content.replace("$(sessionId)", self.__context.get_id());
            
        return content;


    def __change_tas_link(self, tas_link_original, tas_link_pattern):
        change_tas_link = None;
        
        action_link_type = None;
        account_id, tas_session_id, party_id, action_id = None, None, None, None;

        reg_object = re.match("/telephony/v1/account/(\S+)/sessions/(\S+)/parties/(\S+)/(collect|play|say)(.*)", tas_link_pattern);
        if (reg_object):
            if (reg_object.group(1) != "$(accountId)"):
                account_id = reg_object.group(1);
            
            if (reg_object.group(2) != "$(tasSessionId)"):
                tas_session_id = reg_object.group(2);
            
            if (reg_object.group(3) != "$(partyId)"):
                party_id = reg_object.group(3);
        
            if (reg_object.group(5) != ""):
                action_link = reg_object.group(4) + reg_object.group(5);
                reg_object = re.match("(\S+)/(\S+)", action_link);
                if (reg_object):
                    action_link_type = reg_object.group(1);
                    action_id = reg_object.group(2);
                
                else:
                    action_link_type = None;
        else:
            logging.warning("TAS link pattern format '%s' is differ from expected - ignore processing variables.", tas_link_pattern);
            return change_tas_link;
        
        reg_object = re.match("/telephony/v1/account/(\d+)/sessions/(\S+)/parties/(\d+)/(.*)", tas_link_original);
        if (reg_object):
            if (account_id is None):
                account_id = reg_object.group(1);
            
            if (tas_session_id is None):
                tas_session_id = reg_object.group(2);
            
            if (party_id is None):
                party_id = reg_object.group(3);
            
            change_tas_link = "/telephony/v1/account/" + account_id + "/sessions/" + tas_session_id + "/parties/" + party_id + "/";
            if (action_link_type is None):
                change_tas_link += reg_object.group(4);
            
            else:
                reg_object = re.match("(\S+)/(\S+)", reg_object.group(4));
                if (reg_object):
                    change_tas_link += reg_object.group(1) + "/";
                    if (action_id is None):
                        change_tas_link += reg_object.group(2);
                    else:
                        change_tas_link += action_id;
        
        else:
            logging.warning("Impossible to change specified TAS link obtained in StartIVR due to format contradiction.");
        
        return change_tas_link;