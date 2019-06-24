import socket

host = socket.gethostname()
port = 8010

tcp_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
tcp_socket.connect((host, port))

tcp_socket.send(bytes("subscribe", 'utf-8'))

while True:
    data = tcp_socket.recv(256)
    if len(data) > 0:
        print(data.decode("utf-8"))

    elif len(data) == 0:
        print("QService Simulator closed connection.")
        exit(-1)
