import socket
import threading
import time


class client_handler(threading.Thread):
    def __init__(self, connection):
        threading.Thread.__init__(self)

        self.__connection = connection

        self.__event_condition = threading.Condition()
        self.__is_running = True


    def run(self):
        with self.__event_condition:
            while self.__is_running is True:
                self.__event_condition.wait()


    def notify(self, log_line):
        try:
            self.__connection.send(bytes(log_line, 'utf-8'))

        except:
            self.__connection.close()

            with self.__event_condition:
                self.__is_running = False
                self.__event_condition.notify()


    def stop(self):
        self.__connection.close()

        with self.__event_condition:
            self.__is_running = False
            self.__event_condition.notify()



class logger_server:
    def __init__(self, ip_address, port):
        self.__ip = ip_address
        self.__port = port

        self.__status_locker = threading.RLock()
        self.__is_running = True

        self.__clients_locker = threading.RLock()
        self.__clients = []

        self.__tcp_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.__tcp_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.__tcp_socket.bind(('', self.__port))
        self.__tcp_socket.listen(5)

        self.__server = threading.Thread(target=self.__run)
        self.__server.start()

        self.__supervisor = threading.Thread(target=self.__client_supervisor)
        self.__supervisor.start()


    def is_running(self):
        with self.__status_locker:
            return self.__is_running


    def notify(self, log_line):
        with self.__clients_locker:
            for client in self.__clients:
                client.notify(log_line)


    def __client_supervisor(self):
        while self.is_running():
            with self.__clients_locker:
                for client in self.__clients:
                    if not client.is_alive():
                        client.stop()
                        client.join()
                        self.__clients.remove(client)

            time.sleep(1)


    def __run(self):
        while self.is_running():
            try:
                (connection, (ip, port)) = self.__tcp_socket.accept()

                request = connection.recv(64).decode("utf-8")
                if request == 'subscribe':
                    client = client_handler(connection)
                    client.start()

                    self.__clients.append(client)

                else:
                    connection.close()

            except:
                pass


    def stop(self):
        with self.__status_locker:
            self.__is_running = False

        self.__tcp_socket.close()
        self.__server.join()

        self.__supervisor.join()

        for client in self.__clients:
            client.stop()
            client.join()
            self.__clients.remove(client)
