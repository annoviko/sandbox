from SocketServer import TCPServer;

from ivrsim.configuration       import configuration;
from ivrsim.http_handler        import http_handler;
from ivrsim.logging             import logging;
from ivrsim.media_server        import media_server;
from ivrsim.statistical_logger  import statistical_logger;


class http_server:
    def __init__(self, ip = "", port = 8080):
        self.__ip = ip;
        self.__port = port;
        
        self.__statistical_logger = statistical_logger();
        self.__handler = http_handler;
        self.__server = TCPServer((ip, port), self.__handler);
        
        self.__media_server = media_server("", configuration.get_media_port());


    def start(self):
        logging.vip("HTTP server is being starting (port: '%d').", self.__port);
        try:
            if (configuration.get_media_server_enable()):
                self.__media_server.start();
            
            self.__statistical_logger.start();
            self.__server.serve_forever();

        except KeyboardInterrupt:
            self.stop();


    def stop(self):
        if (configuration.get_media_server_enable()):
            self.__media_server.stop();
        
        logging.vip("HTTP server is being closing (port: '%d').", self.__port);
        self.__statistical_logger.stop();
        self.__server.shutdown();
        self.__server.server_close();

