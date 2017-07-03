from SimpleHTTPServer           import SimpleHTTPRequestHandler;
from SocketServer               import TCPServer;

import httplib;
import json;
import re;
import threading;


class signalling_queue:
    __queue = [];
    __signalling_variable = threading.Condition();


    @staticmethod
    def push(signalling_object):
        with signalling_queue.__signalling_variable:
            signalling_queue.__queue.append(signalling_object);
            signalling_queue.__signalling_variable.notify_all();


    @staticmethod
    def pop(timeout = 1):
        with signalling_queue.__signalling_variable:
            while (len(signalling_queue.__queue) == 0):
                signalling_queue.__signalling_variable.wait();

            if (len(signalling_queue.__queue) > 0):
                return signalling_queue.__queue.pop();


    @staticmethod
    def clear():
        with signalling_queue.__signalling_variable:
            signalling_queue.__queue = [];



class http_action_handler(SimpleHTTPRequestHandler):
    __GENERATOR_ACTION_ID = 7888555;
    
    def do_POST(self):
        reg_object = re.match("/telephony/v1/account/(\d+)/sessions/(\d+)/parties/(\d+)/(\w+)$", self.path);
        if (reg_object):
            action_id = str(http_action_handler.__GENERATOR_ACTION_ID);
            self.__send_response(200, "{ \"id\": \"" + action_id + "\" }");
            
            json_request = self.rfile.read(int(self.headers['Content-Length']));
            ivr_request = json.loads(json_request);
            
            signalling_queue.push( (reg_object, ivr_request) );

        else:
            self.__send_response(404);


    def do_DELETE(self):
        reg_object = re.match("/telephony/v1/account/(\d+)/sessions/(\d+)/parties/(\d+)/(\w+)/(\d+)", self.path);
        if (reg_object):
            self.__send_response(200);
            signalling_queue.push( (reg_object, None) );

        else:
            self.__send_response(404);


    def do_PUT(self):
        self.do_POST();


    def __send_response(self, code, json_data = None):
        self.send_response(code);
        self.send_header('Content-Type', 'application/json');
        self.end_headers();
        
        if (json_data is not None):
            self.wfile.write(json_data.encode("utf-8"));



class http_tas_stub_server:
    def __init__(self):
        signalling_queue.clear();

        TCPServer.allow_reuse_address = True;

        self.__start_flag = False;
        self.__http_handler = http_action_handler;
        self.__server = TCPServer(("", 8080), self.__http_handler);


    def start(self):
        if (self.__start_flag is False):
            thread_session = threading.Thread(target = self.__run);
            thread_session.daemon = True;
            thread_session.start();
            
            self.__start_flag = True;


    def stop(self):
        if (self.__start_flag is True):
            self.__server.shutdown();
            self.__start_flag = False;


    def __run(self):
        self.__server.serve_forever();



class action_group:
    ACCOUNT         = 1;
    SESSION         = 2;
    PARTY           = 3;
    ACTION          = 4;
    ACTION_ID       = 5;


def extract_result(http_connection):
    response = http_connection.getresponse();
    
    status = response.status;
    json_content = response.read();
    content = { };
    
    if (len(json_content) > 0):
        try: content = json.loads(json_content);
        except: content = None;

    return (status, content);


def read_json_content(filename):
    with open("contents/" + filename) as file_descriptor:
        content = file_descriptor.read();
        
    return content.encode("utf-8");


def send_http_request(method, command, content = None):
    connection = httplib.HTTPConnection("127.0.0.1", 8000, timeout = 1);
    
    headers = { };
    if (content != None):
        headers = {'Content-Type': 'application/json'}
    
    connection.request(method, command, content, headers);
    
    (status, content) = extract_result(connection);
    connection.close();
    
    assert content != None;
    
    return (status, content);


def send_start_ivr(script_id):
    content = read_json_content("json_start_ivr");
    return send_http_request("POST", "/telephony/v1/account/1000001/distribution-services/ivrs/" + str(script_id) + "/instances", content);


def send_stop_ivr(script_id, ivr_session_id):
    return send_http_request("DELETE", "/telephony/v1/account/1000001/distribution-services/ivrs/" + str(script_id) + "/instances/" + str(ivr_session_id));


def send_execution_status(script_id, ivr_session_id):
    return send_http_request("GET", "/telephony/v1/account/1000001/distribution-services/ivrs/" + str(script_id) + "/instances/" + str(ivr_session_id));


def send_service_status(counters = None):
    return send_http_request("GET", "/telephony/v1/account/1000001/distribution-services/status");


def send_result_play(script_id, ivr_session_id, play_id):
    content = read_json_content("json_result_action");
    return send_http_request("POST", "/telephony/v1/account/1000001/distribution-services/ivrs/" + str(script_id) + "/instances/" + str(ivr_session_id) + "/actions/plays/" + play_id, content);


def send_result_say(script_id, ivr_session_id, say_id):
    content = read_json_content("json_result_action");
    return send_http_request("POST", "/telephony/v1/account/1000001/distribution-services/ivrs/" + str(script_id) + "/instances/" + str(ivr_session_id) + "/actions/says/" + say_id, content);


def send_result_collect(script_id, ivr_session_id, collect_id, content = None):
    if (content is None):
        content = read_json_content("json_result_action");
    
    return send_http_request("POST", "/telephony/v1/account/1000001/distribution-services/ivrs/" + str(script_id) + "/instances/" + str(ivr_session_id) + "/actions/collects/" + collect_id, content);


def receive_action(action, existance_action_id, timeout):
    (header, json_body) = signalling_queue.pop(timeout);

    try:
        if (header.group(action_group.ACTION) != action):
            return None;
    
        if (existance_action_id is True):
            action_id = header.group(action_group.ACTION_ID);
            if (action_id == ""):
                return None;
        
        else:
            # Exception should be obtained
            action_id = header.group(action_group.ACTION_ID);

    except:
        if (existance_action_id is True):
            return None;
        
        else:
            # We are satisfy by expection in case of none of action id
            pass;
    
    return (header, json_body);


def receive_play(timeout = 1):
    return receive_action('play', False, timeout);


def receive_stop_play(timeout = 1):
    return receive_action('play', True, timeout);


def receive_say(timeout = 1):
    return receive_action('say', False, timeout);


def receive_stop_say(timeout = 1):
    return receive_action('say', True, timeout);


def receive_collect(timeout = 1):
    return receive_action('collect', False, timeout);


def receive_stop_collect(timeout = 1):
    return receive_action('collect', True, timeout);


def receive_forward(timeout = 1):
    return receive_action('forward', False, timeout);
