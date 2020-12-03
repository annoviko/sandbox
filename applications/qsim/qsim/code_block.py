import sys
import re

from qsim.error_code import error_code
from qsim.logging import logging


class command_type:
    COMMAND_SEND = "SEND"
    COMMAND_TIMEOUT = "TIMEOUT"
    COMMAND_WAIT = "WAIT"
    COMMAND_REPLY = "REPLY"
    COMMAND_IGNORE = "IGNORE"
    COMMAND_EXIT = "EXIT"
    COMMAND_TRIGGER = "TRIGGER"
    COMMAND_END_TRIGGER = "END_TRIGGER"
    COMMAND_FUNCTION = "FUNCTION"
    COMMAND_END_FUNCTION = "END_FUNCTION"
    COMMAND_IF = "IF"
    COMMAND_ELSE = "ELSE"
    COMMAND_END_IF = "END_IF"
    COMMAND_MOVE_JSON = "MOVE_JSON"
    COMMAND_PRINT = "PRINT"
    COMMAND_ASK = "ASK"
    COMMAND_ASSIGN = "ASSIGN"


class code_block_type:
    CODE_BLOCK_TRIGGER = 0
    CODE_BLOCK_CONDITION = 1
    CODE_BLOCK_MAIN = 2
    CODE_BLOCK_FUNCTION = 3


class trigger:
    def __init__(self, message_id, commands, reply_code):
        self.message_id = message_id
        self.commands = commands
        self.reply_code = reply_code


class code_block:
    __KEYWORD_COMMENT = "###"
    __KEYWORD_COMMAND = "@@@"
    
    def __init__(self, line_number, content, type_block = code_block_type.CODE_BLOCK_MAIN):
        self.__content_flag = False
        self.__command_sequence = []
        self.__command_content = ""

        self.__command_headers = {}

        self.__triggers = {}
        self.__functions = {}
        
        self.__line_counter = line_number
        self.__line = None
        self.__content = content
        self.__subfunction = type_block
    
    
    def load(self):
        while self.__line_counter < len(self.__content):
            self.__line = self.__content[self.__line_counter]
            self.__line_counter += 1
            self.__line = self.__line.lstrip().rstrip()
            
            if not self.__line:
                continue
            
            reg_object = re.match("^(" + code_block.__KEYWORD_COMMAND + ")(\s+)(\S+)(.*)", self.__line)
            if reg_object:
                command = reg_object.group(3)
                argument = reg_object.group(4).lstrip()
                
                if self.__process_command(command, argument) is False:
                    return

            elif self.__line[0:len(code_block.__KEYWORD_COMMENT)] != code_block.__KEYWORD_COMMENT:
                self.__process_command_content()
        
        # Script is over and it was the last command
        if self.__content_flag is True:
            self.__command_sequence[len(self.__command_sequence) - 1][1][1] = self.__command_content.lstrip().rstrip()


    def get_commands(self):
        return self.__command_sequence


    def get_triggers(self):
        return self.__triggers


    def get_functions(self):
        return self.__functions


    def get_cursor(self):
        return self.__line_counter


    def __process_command_content(self):
        if self.__content_flag is not True:
            logging.error("Incorrect script-file format in line '%s' (%d).", self.__line_counter, self.__line)
            sys.exit(error_code.ERROR_SCRIPT_INCORRECT_FORMAT)

        reg_object = re.match("HEADER (.*) (.*)", self.__line)

        if reg_object:
            key = reg_object.group(1).rstrip().lstrip()
            value = reg_object.group(2).rstrip().lstrip()

            self.__command_headers[key] = value
        else:
            self.__command_content += self.__line


    def __process_command(self, command, argument):
        if self.__content_flag is True:
            if self.__command_sequence[-1][0] == command_type.COMMAND_REPLY:
                self.__command_sequence[-1][1][2] = self.__command_content.lstrip().rstrip()
                self.__command_sequence[-1][1][3] = self.__command_headers
            elif self.__command_sequence[-1][0] == command_type.COMMAND_SEND:
                self.__command_sequence[-1][1][1] = self.__command_content.lstrip().rstrip()
                self.__command_sequence[-1][1][2] = self.__command_headers
        
        self.__content_flag = False
        self.__command_content = ""
        self.__command_headers = {}

        if command == command_type.COMMAND_SEND:
            self.__process_command_send(command, argument)

        elif command == command_type.COMMAND_TIMEOUT:
            self.__process_command_timeout(command, argument)
        
        elif command == command_type.COMMAND_WAIT:
            self.__process_command_wait(command, argument)

        elif command == command_type.COMMAND_REPLY:
            self.__process_command_reply(command, argument)

        elif command == command_type.COMMAND_IGNORE:
            self.__process_command_ignore(command, argument)

        elif command == command_type.COMMAND_ASK:
            self.__process_command_ask(command, argument)

        elif command == command_type.COMMAND_ASSIGN:
            self.__process_command_assign(command, argument)

        elif command == command_type.COMMAND_EXIT:
            self.__process_command_exit(command)
        
        elif command == command_type.COMMAND_PRINT:
            self.__process_command_print(command, argument)

        elif command == command_type.COMMAND_MOVE_JSON:
            self.__process_command_move_json(command, argument)
        
        elif command == command_type.COMMAND_IF:
            self.__process_command_if(command, argument)
        
        elif command == command_type.COMMAND_ELSE:
            if self.__process_command_else(command) is False:
                return False
        
        elif command == command_type.COMMAND_END_IF:
            if self.__process_command_end_if(command) is False:
                return False
        
        elif command == command_type.COMMAND_TRIGGER:
            self.__process_command_trigger(command, argument)
        
        elif command == command_type.COMMAND_END_TRIGGER:
            if self.__process_command_end_trigger(command) is False:
                return False
        
        else:
            logging.error("Unknown command is detected '%s' in line '%s' (%d).", command, self.__line, self.__line_counter)
            sys.exit(error_code.ERROR_SCRIPT_UNKNOWN_COMMAND)


    def __process_command_send(self, command, argument):
        self.__content_flag = True
        
        command_line_arguments = argument.split()
        self.__command_sequence.append((command, [command_line_arguments[0], "", {}, command_line_arguments[1:]]))
        
        return True


    def __process_command_timeout(self, command, argument):
        self.__command_sequence.append((command, [int(argument)]))
        return True


    def __process_command_wait(self, command, argument):
        self.__command_sequence.append((command, [argument]))
        return True


    def __process_command_reply(self, command, argument):
        self.__content_flag = True

        command_line_arguments = argument.split()
        code = command_line_arguments[0]
        message = ""
        if len(command_line_arguments) > 1:
            message = command_line_arguments[1]

        self.__command_sequence.append((command, [int(code), message, None, None]))

        return True


    def __process_command_ignore(self, command, argument):
        self.__command_sequence.append((command, None))
        return True


    def __process_command_ask(self, command, argument):
        self.__command_sequence.append((command, [argument]))
        return True


    def __process_command_assign(self, command, argument):
        command_line_arguments = argument.split()
        self.__command_sequence.append((command, [command_line_arguments[0], command_line_arguments[1]]))
        return True


    def __process_command_exit(self, command):
        self.__command_sequence.append((command, None))
        return True


    def __process_command_print(self, command, argument):
        self.__command_sequence.append((command, [argument]))
        return True


    def __process_command_move_json(self, command, argument):
        self.__command_sequence.append((command, [argument]))
        return True


    def __process_command_if(self, command, argument):
        block = code_block(self.__line_counter, self.__content, code_block_type.CODE_BLOCK_CONDITION)
        block.load()
        
        self.__line_counter = block.get_cursor()
        self.__command_sequence.append((command, [argument, block.get_commands()]))
        
        return True


    def __process_command_else(self, command):
        if self.__command_sequence[len(self.__command_sequence) - 1][0] != command_type.COMMAND_IF:
            if self.__subfunction == code_block_type.CODE_BLOCK_MAIN:
                logging.error("Unexpected keyword '%s' (else branch of conditional block) without conditional block in line '%s' (%d).", command, self.__line, self.__line_counter)
                sys.exit(error_code.ERROR_SCRIPT_INCORRECT_FORMAT)
                
            # go to upper level to process it without shifting line (we still need to process else keyword)
            self.__line_counter = self.__line_counter - 1
            return False
        
        else:
            block = code_block(self.__line_counter, self.__content, code_block_type.CODE_BLOCK_CONDITION)
            block.load()
            
            self.__line_counter = block.get_cursor()
            self.__command_sequence.append((command, [block.get_commands()]))
        
        return True


    def __process_command_end_if(self, command):
        if self.__subfunction != code_block_type.CODE_BLOCK_CONDITION:
            logging.error("Unexpected keyword '%s' (end of conditional block) without conditional block in line '%s' (%d).", command, self.__line, self.__line_counter)
            sys.exit(error_code.ERROR_SCRIPT_INCORRECT_FORMAT)
        
        return True


    def __process_command_trigger(self, command, argument):
        if self.__subfunction != code_block_type.CODE_BLOCK_MAIN:
            logging.error("Unexpected keyword '%s' (start trigger) in sub-block in line '%s' (%d).", command, self.__line, self.__line_counter)
            sys.exit(error_code.ERROR_SCRIPT_INCORRECT_FORMAT)
        
        if argument in self.__triggers:
            logging.error("Trigger redefinition (trigger for '%s' is already defined) in line '%s' (%d).", argument, self.__line, self.__line_counter)
            sys.exit(error_code.ERROR_SCRIPT_INCORRECT_FORMAT)
        
        # extract code block
        block = code_block(self.__line_counter, self.__content, code_block_type.CODE_BLOCK_TRIGGER)
        block.load()
        
        self.__line_counter = block.get_cursor()
        
        # extract arguments
        command_line_arguments = argument.split()
        
        if len(command_line_arguments) == 1:
            trigger_message_id, trigger_reply_code = command_line_arguments[0], None
            
        elif len(command_line_arguments) == 2:
            trigger_message_id, trigger_reply_code = command_line_arguments[0], command_line_arguments[1]
            
        else:
            logging.error("Invalid number of arguments of '%d' in line '%s' (%d).", len(command_line_arguments), self.__line, self.__line_counter)
            sys.exit(error_code.ERROR_SCRIPT_AMOUNT_ARGUMENTS)
        
        if trigger_reply_code is not None:
            trigger_reply_code = int(trigger_reply_code)
        
        # create and store trigger
        self.__triggers[trigger_message_id] = trigger(trigger_message_id, block.get_commands(), trigger_reply_code)
        
        return True


    def __process_command_end_trigger(self, command):
        if self.__subfunction != code_block_type.CODE_BLOCK_TRIGGER:
            logging.error("Unexpected keyword '%s' is appeard in non-subbody in line '%s' (%d).", command, self.__line, self.__line_counter)
            sys.exit(error_code.ERROR_SCRIPT_INCORRECT_FORMAT)
        
        return True
