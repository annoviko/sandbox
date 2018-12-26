from socketserver import TCPServer

from qsim.http_handler import http_handler
from qsim.logging import logging
from qsim.statistical_logger import statistical_logger


class http_server:
    def __init__(self, ip="", port=8080, **kwargs):
        enable_statistical_logger = kwargs.get("statistical_logger", False)

        self.__ip = ip
        self.__port = port

        self.__statistical_logger = None
        if enable_statistical_logger:
            self.__statistical_logger = statistical_logger()

        self.__handler = http_handler
        self.__server = TCPServer((ip, port), self.__handler)


    def start(self):
        logging.vip("HTTP server is being starting (port: '%d').", self.__port)
        try:
            if self.__statistical_logger is not None:
                self.__statistical_logger.start()

            self.__server.serve_forever()

        except KeyboardInterrupt:
            self.stop()


    def stop(self):
        logging.vip("HTTP server is being closing (port: '%d').", self.__port)
        if self.__statistical_logger is not None:
            self.__statistical_logger.stop()

        self.__server.shutdown()
        self.__server.server_close()

