import re;
import os;

from optparse import OptionParser;



class analyser:
    def __init__(self, tam_state_evolutions, pool_evolution):
        self.__stm_evolutions = tam_state_evolutions;
        self.__pool_evolution = pool_evolution;


    def calculate_system_statistic(self):
        sys_stat = system_statistic(len(self.__stm_evolutions), len(self.__pool_evolution));
        
        for tam_id, tam_states in self.__stm_evolutions.items():
            if not tam_states[-1].is_idle():
                sys_stat.inc_amount_leak_state_machines(tam_id);

        for block_address, block_states in self.__pool_evolution.items():
            if not block_states[-1].is_free():
                sys_stat.inc_amount_leak_pool_blocks(block_address);

        return sys_stat;



class system_statistic:
    def __init__(self, total_ams, total_blocks):
        self.__amount_state_machines = total_ams;
        self.__free_state_machines = total_ams;
        self.__leak_state_machines = [];
        
        self.__amount_pool_blocks = total_blocks;
        self.__free_pool_blocks = total_blocks;
        self.__leak_pool_blocks = [];

    def get_amount_used_state_machines(self):
        return self.__amount_state_machines;

    def get_amount_free_state_machines(self):
        return self.__free_state_machines;

    def get_amount_leak_state_machines(self):
        return len(self.__leak_state_machines);

    def get_leak_state_machines(self):
        return self.__leak_state_machines;

    def get_amount_used_pool_blocks(self):
        return self.__amount_pool_blocks;

    def get_amount_free_pool_blocks(self):
        return self.__free_pool_blocks;

    def get_amount_leak_pool_blocks(self):
        return len(self.__leak_pool_blocks);

    def get_leak_pool_blocks(self):
        return self.__leak_pool_blocks;

    def inc_amount_leak_state_machines(self, tam_id):
        self.__leak_state_machines.append(tam_id);
        self.__free_state_machines -= 1;

    def inc_amount_leak_pool_blocks(self, address):
        self.__leak_pool_blocks.append(address);
        self.__free_pool_blocks -= 1;



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



class pool_state:
    POOL_STATE_FREE         = 0;
    POOL_STATE_ALLOCATED    = 1;
    
    def __init__(self, address, block_state, timestamp, alloc_line):
        self.__address      = address;
        self.__state        = block_state;
        self.__timestamp    = timestamp;
        self.__alloc_line   = alloc_line;

    def get_address(self):
        return self.__address;

    def get_alloc_line(self):
        return self.__alloc_line;

    def get_time(self):
        return self.__timestamp;

    def is_free(self):
        return self.__state == pool_state.POOL_STATE_FREE;



class tam_log_reader:
    def __init__(self, tam_file_logs):
        self.__tam_file_logs           = tam_file_logs;
        self.__tam_state_evolution     = { };      # hash { <AM ID> : <list of states of the AM> }
        self.__pool_state_evolution    = { };      # hash { <address> : <list of states of the pool block> }


    def load(self):
        for tam_log_file in self.__tam_file_logs:
            with open(tam_log_file, 'r', encoding='utf-8', errors='ignore') as log_file_descr:
                self.__process_filelog(log_file_descr);


    def get_tam_state_evolution(self):
        return self.__tam_state_evolution;


    def get_pool_state_evolution(self):
        return self.__pool_state_evolution;


    def __process_filelog(self, log_file_descr):
        for line in log_file_descr:
            line_parse_result = self.__try_parse_tam_state(line);
            if (line_parse_result is False):
                self.__try_parse_pool_state(line);


    def __try_parse_pool_state(self, line):
        pool_block_state = pool_parser(line).parse();
        if (pool_block_state is not None):
            block_address = pool_block_state.get_address();
            if (block_address in self.__pool_state_evolution):
                self.__pool_state_evolution[block_address].append(pool_block_state);
            
            else:
                self.__pool_state_evolution[block_address] = [ pool_block_state ];
            
            return True;
        
        return False;


    def __try_parse_tam_state(self, line):
        state = state_parser(line).parse();
        if (state is not None):
            if (state.get_id() in self.__tam_state_evolution):
                self.__tam_state_evolution[state.get_id()].append(state);
            
            else:
                self.__tam_state_evolution[state.get_id()] = [ state ];
            
            return True;
        
        return False;



class pool_parser:
    __MARKER_POOL_ALLOC     = "CPool::Allocate() TAM";
    __MARKER_POOL_DEALLOC   = "CPool::Deallocate() TAM";
    
    __PARSE_ALLOC       = "\|([0-9:.]+)\|.*\|CPool::Allocate\(\) TAM \[(\S+)\].*";
    __PARSE_DEALLOC     = "\|([0-9:.]+)\|.*\|CPool::Deallocate\(\) TAM \[(\S+)\].*";
    
    __GROUP_TIME        = 1;
    __GROUP_ADDRESS     = 2;
    
    def __init__(self, line):
        self.__line = line;
    
    def parse(self):
        pool_state_instance = None;
        if (pool_parser.__MARKER_POOL_ALLOC in self.__line):
            reg_object = re.match(pool_parser.__PARSE_ALLOC, self.__line);
            if (reg_object):
                timestamp   = reg_object.group(pool_parser.__GROUP_TIME);
                address     = reg_object.group(pool_parser.__GROUP_ADDRESS);
                pool_state_instance = pool_state(address, pool_state.POOL_STATE_ALLOCATED, timestamp, self.__line);
            
        
        if (pool_parser.__MARKER_POOL_DEALLOC in self.__line):
            reg_object = re.match(pool_parser.__PARSE_DEALLOC, self.__line);
            if (reg_object):
                timestamp   = reg_object.group(pool_parser.__GROUP_TIME);
                address     = reg_object.group(pool_parser.__GROUP_ADDRESS);
                pool_state_instance = pool_state(address, pool_state.POOL_STATE_FREE, timestamp, self.__line);
        
        return pool_state_instance;



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



class console_visualizer:
    def __init__(self, tam_evolutions, pool_evolutions, files, options):
        self.__tam_evolutions   = tam_evolutions;
        self.__pool_evolutions  = pool_evolutions;
        self.__files            = files;
        self.__stats            = None;
        self.__options          = options;


    def visualize(self):
        self.__stats = analyser(tam_evolutions, pool_evolutions).calculate_system_statistic();
        
        self.__print_answering_maching_summary();
        self.__print_pool_block_summary();
        
        if (self.__stats.get_amount_leak_state_machines() > 0):
            print("\n");
            self.__print_leakage_answering_machines();
        
        if (self.__stats.get_amount_leak_pool_blocks() > 0):
            print("\n");
            self.__print_leakage_pool_blocks();


    def __print_answering_maching_summary(self):
        print("------------------------------------------------------------------");
        print("General information about AMs (files: " + str(self.__files) + "):");
        print("------------------------------------------------------------------");
        print("* Amount of used AMs: " + str(self.__stats.get_amount_used_state_machines()) + ";");
        print("* Amount of free AMs: " + str(self.__stats.get_amount_free_state_machines()) + ";");
        print("* Amount of leak AMs: " + str(self.__stats.get_amount_leak_state_machines()) + ";");
        print("------------------------------------------------------------------");
        
        if (self.__stats.get_amount_leak_state_machines() == 0):
            print("\n[ SUCCESS ] No leakage is detected in AMs\n")
        else:
            print("\n[ FAILURE ] Leakage is detected in AMs\n")
        
        print("------------------------------------------------------------------");


    def __print_pool_block_summary(self):
        print("------------------------------------------------------------------");
        print("General information about TAM pool (files: " + str(self.__files) + "):");
        print("------------------------------------------------------------------");
        print("* Amount of used blocks: " + str(self.__stats.get_amount_used_pool_blocks()) + ";");
        print("* Amount of free blocks: " + str(self.__stats.get_amount_free_pool_blocks()) + ";");
        print("* Amount of leak blocks: " + str(self.__stats.get_amount_leak_pool_blocks()) + ";");
        print("------------------------------------------------------------------");
        
        if (self.__stats.get_amount_leak_pool_blocks() == 0):
            print("\n[ SUCCESS ] No leakage is detected in TAM pool\n")
        else:
            print("\n[ FAILURE ] Leakage is detected in TAM pool\n")
        
        print("------------------------------------------------------------------");


    def __print_leakage_answering_machines(self):
        print("------------------------------------------------------------------");
        print("Information about leak in AMs:");
        
        tam_leak_ams = self.__stats.get_leak_state_machines();
        for am_id in tam_leak_ams:
            print("------------------------------------------------------------------");
            print("AM ID: " + str(am_id) + ";");
            print("States (" + str(len(self.__tam_evolutions[am_id])) + "): ");
            
            states = self.__tam_evolutions[am_id];
            length = len(states);
            if (length > self.__options.limit):
                print("\t... ... <Last '" + str(self.__options.limit) + "' states are displayed> ... ...");
            
            for state in states:
                if (length <= self.__options.limit):
                    if (state.is_idle()):
                        print("\t" + state.get_time() + "   " + state.get_state() + "; \t\t[STATE MACHINE IS FREE]");
                    else:
                        print("\t" + state.get_time() + "   " + state.get_state() + ";");
                
                length -= 1;
            
            print("------------------------------------------------------------------");


    def __print_leakage_pool_blocks(self):
        print("------------------------------------------------------------------");
        print("Information about leak in TAM pool:");
        
        leak_pool_blocks = self.__stats.get_leak_pool_blocks();
        for address in leak_pool_blocks:
            print("------------------------------------------------------------------");
            print("Block address: " + address + ";");
            print("Memory operations (" + str(len(self.__pool_evolutions)) + "): ");
            
            states = self.__pool_evolutions[address];
            length = len(states);
            if (length > self.__options.limit):
                print("\t... ... <Last '" + str(self.__options.limit) + "' states are displayed> ... ...");
            
            for state in states:
                if (length <= self.__options.limit):
                    if (state.is_free()):
                        print("\t" + state.get_time() + "   block is deallocated;");
                    else:
                        print("\t" + state.get_time() + "   block is allocated;");
                
                length -= 1;
            
            print("------------------------------------------------------------------");



def callback_file_reader(option, opt, value, parser):
    setattr(parser.values, option.dest, value.split(' '))



if __name__ == '__main__':
    parser = OptionParser();
    parser.add_option("-f", "--files", dest="files", type="string", default=[], action='callback', callback=callback_file_reader,
                      help="TAM log files that should be checked for AM leakage.", metavar="LOG_FILES");
    parser.add_option("-l", "--limit", dest="limit", type="int", default=10, 
                      help="amount of states of leaked AM that should be displayed.", metavar="LIMIT");
    
    (options, args) = parser.parse_args();
    tam_log_files = options.files;
    for tam_log in tam_log_files:
        if (not os.path.isfile(tam_log)):
            print("ERROR: TAM log is not found '" + tam_log + "'.\n");
            parser.print_help();
            exit(-1);
    
    log_reader = tam_log_reader(tam_log_files);
    log_reader.load();
    
    tam_evolutions = log_reader.get_tam_state_evolution();
    pool_evolutions = log_reader.get_pool_state_evolution();
    
    visualizer = console_visualizer(tam_evolutions, pool_evolutions, tam_log_files, options);
    visualizer.visualize();
    
    exit(0);