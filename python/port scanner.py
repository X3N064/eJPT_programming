from asyncio.proactor_events import _ProactorSocketTransport
import socket

target = input("Enter the IP address to scan : ")
portrange = input("Enter the port range to scan (5-200)")


LOWPORT = int(portrange.split('-')[0])
HIGHPORT = int(portrange.split('-')[1])

print("scanning host ", target, "from port", LOWPORT, "to port", HIGHPORT)

for port in range(LOWPORT, HIGHPORT):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    status = s.connect_ex((target, port))
    if(status == 0):
        print("*** port", port, "- open ***")
    else:
        print("port", port, "- closeed")
    s.close()
