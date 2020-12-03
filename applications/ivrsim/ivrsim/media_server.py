from SocketServer               import TCPServer;

from ivrsim.logging             import logging;
from ivrsim.media_handler       import media_handler;

import threading;


class media_server:
    def __init__(self, ip = "", port = 8080):
        self.__ip = ip;
        self.__port = port;
        
        self.__handler = media_handler;
        self.__server = TCPServer((ip, port), self.__handler);


    def start(self):
        logging.vip("Media server is being starting (port: '%d').", self.__port);
        
        thread_session = threading.Thread(target = self.__run);
        thread_session.daemon = True;
        thread_session.start();


    def stop(self):
        logging.vip("Media server is being closing (port: '%d').", self.__port);
        self.__server.shutdown();
        self.__server.server_close();


    def __run(self):
        logging.vip("Media server thread is started.");
        self.__server.serve_forever();
        logging.vip("Media server thread is terminated.");
