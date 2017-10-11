import re;
import os;

from optparse import OptionParser;


class analyser:
    def __init__(self, tam_state_evolutions):
        self.__evolutions = tam_state_evolutions;

    def calculate_system_statistic(self):
        sys_stat = system_statistic(len(self.__evolutions));
        for tam_id, tam_states in self.__evolutions.items():
            if not tam_states[-1].is_idle():
                sys_stat.inc_amount_leak_state_machines(tam_id);

        return sys_stat;



class system_statistic:
    def __init__(self, total):
        self.__amount_state_machines = total;
        self.__free_state_machines = total;
        self.__leak_state_machines = [];

    def get_amount_used_state_machines(self):
        return self.__amount_state_machines;
    
    def get_amount_free_state_machines(self):
        return self.__free_state_machines;
    
    def get_amount_leak_state_machines(self):
        return len(self.__leak_state_machines);

    def get_leak_state_machines(self):
        return self.__leak_state_machines;

    def inc_amount_leak_state_machines(self, tam_id):
        self.__leak_state_machines.append(tam_id);
        self.__free_state_machines -= 1;



class tam_state:
    __MARKER_IDLE_STATE = "CStIdle";

    def __init__(self, tam_id, state_time, state, log_line):
        self.__tam_id = tam_id;
        self.__time = state_time;
        self.__state = state;
        self.__line = log_line;

    def get_id(self):
        return self.__tam_id;

    def get_time(self):
        return self.__time;

    def get_state(self):
        return self.__state;

    def get_line(self):
        return self.__line;

    def is_idle(self):
        return self.__state == tam_state.__MARKER_IDLE_STATE;



class tam_log_reader:
    def __init__(self, tam_file_logs):
        self.__tam_file_logs = tam_file_logs;

    def load(self):
        tam_state_evolution = { };
        for tam_log_file in self.__tam_file_logs:
            with open(tam_log_file, 'r', encoding='utf-8', errors='ignore') as log_file_descr:
                self.__process_filelog(log_file_descr, tam_state_evolution);
        
        return tam_state_evolution;

    def __process_filelog(self, log_file_descr, tam_state_evolution):
        for line in log_file_descr:
            state = state_parser(line).parse();
            if (state is None):
                continue;
            
            if (state.get_id() in tam_state_evolution):
                tam_state_evolution[state.get_id()].append(state);
            
            else:
                tam_state_evolution[state.get_id()] = [ state ];



class state_parser:
    __MARKER_ANSWERING_MACHINE = "|AM_";
    __MARKER_STATE = "|------------- ";
    __PARSE_EXPRESSION = "\|([0-9:.]+)\|.*\|AM_(\d+).*\|.*: (\S+) -.*";

    __GROUP_TIME = 1;
    __GROUP_TAM_ID = 2;
    __GROUP_STATE = 3;

    def __init__(self, line):
        self.__line = line;

    def parse(self):
        if (state_parser.__MARKER_STATE not in self.__line):
            return None;
        
        if (state_parser.__MARKER_ANSWERING_MACHINE not in self.__line):
            return None;
        
        reg_object = re.match(state_parser.__PARSE_EXPRESSION, self.__line);
        if (reg_object):
            timestamp = reg_object.group(state_parser.__GROUP_TIME);
            tam_id = reg_object.group(state_parser.__GROUP_TAM_ID);
            state = reg_object.group(state_parser.__GROUP_STATE);
            
            return tam_state(tam_id, timestamp, state, self.__line);
        
        return None;



def callback_file_reader(option, opt, value, parser):
    setattr(parser.values, option.dest, value.split(' '))



if __name__ == '__main__':
    parser = OptionParser();
    parser.add_option("-f", "--files", dest="files", type="string", default=[], action='callback', callback=callback_file_reader,
                      help="TAM log files that should be checked for AM leakage.", metavar="LOG_FILES");
    parser.add_option("-l", "--limit", dest="limit", type="int", default=20, 
                      help="amount of states of leaked AM that should be displayed.", metavar="LIMIT");
    
    (options, args) = parser.parse_args();
    tam_log_files = options.files;
    for tam_log in tam_log_files:
        if (not os.path.isfile(tam_log)):
            print("ERROR: TAM log is not found '" + tam_log + "'.\n");
            parser.print_help();
            exit(-1);
    
    tam_evolutions = tam_log_reader(tam_log_files).load();
    stats = analyser(tam_evolutions).calculate_system_statistic();
    
    print("------------------------------------------------------------------");
    print("General information about AMs (files: " + str(tam_log_files) + "):");
    print("------------------------------------------------------------------");
    print("* Amount of used AMs: " + str(stats.get_amount_used_state_machines()) + ";");
    print("* Amount of free AMs: " + str(stats.get_amount_free_state_machines()) + ";");
    print("* Amount of leak AMs: " + str(stats.get_amount_leak_state_machines()) + ";");
    print("------------------------------------------------------------------");
    
    if (stats.get_amount_leak_state_machines() == 0):
        print("\n[ SUCCESS ] No leakage is detected in AMs\n")
    else:
        print("\n[ FAILURE ] Leakage is detected in AMs\n")
    
    print("------------------------------------------------------------------");
    
    
    if (stats.get_amount_leak_state_machines() > 0):
        print("\nInformation about leak in AMs:");
        
        tam_leak_ams = stats.get_leak_state_machines();
        for am_id in tam_leak_ams:
            print("------------------------------------------------------------------");
            print("AM ID: " + str(am_id) + ";");
            print("States (" + str(len(tam_evolutions[am_id])) + "): ");
            
            states = tam_evolutions[am_id];
            length = len(states);
            if (length > options.limit):
                print("\t... ... <Last '" + str(options.limit) + "' states are displayed> ... ...");
            
            for state in states:
                if (length <= options.limit):
                    if (state.is_idle()):
                        print("\t" + state.get_time() + "   " + state.get_state() + "; \t\t[STATE MACHINE IS FREE]");
                    else:
                        print("\t" + state.get_time() + "   " + state.get_state() + ";");
                
                length -= 1;
            
            print("------------------------------------------------------------------");
        exit(-1);
    
    exit(0);