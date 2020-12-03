from socketserver import TCPServer

from ivrstub.http_handler import http_handler
from ivrstub.logging import logging


class http_server:
    def __init__(self, ip="", port=8080):
        self.__ip = ip
        self.__port = port

        self.__handler = http_handler
        self.__server = TCPServer((ip, port), self.__handler)


    def start(self):
        logging.vip("HTTP server is being starting (port: '%d').", self.__port)
        try:
            self.__server.serve_forever()

        except KeyboardInterrupt:
            self.stop()


    def stop(self):
        logging.vip("HTTP server is being closing (port: '%d').", self.__port)

        self.__server.shutdown()
        self.__server.server_close()

