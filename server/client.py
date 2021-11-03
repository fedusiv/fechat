import socket

class Client:


    def __init__(self, client_socket):
        self.socket_d: socket.socket = client_socket
        pass

    def client_loop(self):
        while True:
            data = self.socket_d.recv(1024)
            if data:
                print(data)
            pass