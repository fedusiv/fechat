import socket
import config
import threading
from client import Client

class Server:


    def __init__(self):
        self.client_threads_list = []
        self.server_socket = socket.socket()
        self.run_connection()
        pass


    def run_connection(self):
        self.server_socket.bind((config.SERVER_ADDRESS,config.SERVER_PORT))
        self.server_socket.listen(5)
        server_thread = threading.Thread(target =self.server_logic, args=(self.server_socket,))
        server_thread.start()
        while True:
            client, address = self.server_socket.accept()
            print('Connected client: ' + address[0] + ':' + str(address[1]))
            # create client's thread
            client_thread = threading.Thread(target =self.create_client, args=(client, ))
            self.client_threads_list.append(client_thread)
            client_thread.start()

    # This method runs inside client's thread
    def create_client(self, client_socket):
        client_obj = Client(client_socket)
        # start loop
        client_obj.client_loop()

    def server_logic(self,socket):
        print("Started server logic loop")
        while True:
            pass