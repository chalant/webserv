import socket
import subprocess
import time
import signal
import os

# ANSI escape codes for colors
GREEN = '\033[92m'
RED = '\033[91m'
YELLOW = '\033[93m'
RESET = '\033[0m'

# Handle signals in case of ctrl+c to avoid leaving the server running
def sigHandler(sig, frame):
    webserv.kill()
    exit(0)
    
signal.signal(signal.SIGINT, sigHandler)

def getResponseBody(request):
    server_address = ('localhost', 8080)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(server_address)
    s.sendall(request)
    response = s.recv(4096)
    s.close()
    # return the body of the response
    return response.decode().splitlines()[-1]

# Compile the project
subprocess.run(['make', '-j8'], cwd=os.path.join(os.path.dirname(__file__), '../../'), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

# Start the server
webserv = subprocess.Popen(['./webserv'], cwd=os.path.join(os.path.dirname(__file__), '../../'), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

#Wait for the server to start
time.sleep(1)

# Test /hello.py
response_body = getResponseBody(b'GET /hello.py HTTP/1.1\r\nHost: localhost\r\n\r\n')
assert response_body == "Hello, world!"
#print("\thello.py test".ljust(34) + f"{GREEN}OK!{RESET}")

# Terminate the server
webserv.kill()

# Clean up
subprocess.run(['make', 'fclean'], cwd=os.path.join(os.path.dirname(__file__), '../../'), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
