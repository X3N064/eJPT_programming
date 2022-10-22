import socket
import os
import platform

SER_ADDR = "192.168.219.101"
SER_PORT = 5050

my_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
my_sock.connect((SER_ADDR, SER_PORT))

my_sock.sendall(platform.machine(), platform.uname(),
                platform.version(), platform.system())
