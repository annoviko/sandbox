from socketserver import TCPServer

from qsim.http_handler import http_handler
from qsim.logging import logging
from qsim.statistical_logger import statistical_logger


class http_server:
    def __init__(self, ip="", port=8080):
        self.__ip = ip
        self.__port = port
        
        self.__statistical_logger = statistical_logger()
        self.__handler = http_handler
        self.__server = TCPServer((ip, port), self.__handler)


    def start(self):
        logging.vip("HTTP server is being starting (port: '%d').", self.__port)
        try:
            self.__statistical_logger.start()
            self.__server.serve_forever()

        except KeyboardInterrupt:
            self.stop()


    def stop(self):
        logging.vip("HTTP server is being closing (port: '%d').", self.__port)
        self.__statistical_logger.stop()
        self.__server.shutdown()
        self.__server.server_close()

