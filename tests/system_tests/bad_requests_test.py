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

def getResponseStatusLine(request):
    server_address = ('localhost', 8080)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(server_address)
    s.sendall(request)
    response = s.recv(4096)
    s.close()
    return response.decode().splitlines()[0]

# Compile the project
subprocess.run(['make', '-j8'], cwd=os.path.join(os.path.dirname(__file__), '../../'), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

# Start the server
webserv = subprocess.Popen(['./webserv'], cwd=os.path.join(os.path.dirname(__file__), '../../'), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

#Wait for the server to start
time.sleep(1)


# Test an Unknown Method
response_status_line = getResponseStatusLine(b'NOTKNOWN / HTTP/1.1\r\nHost: localhost\r\n\r\n')
assert response_status_line == "HTTP/1.1 405 Method Not Allowed"
print("\tUnknown Method(405) test".ljust(34) + f"{GREEN}OK!{RESET}")

# Test an Unimplemented Method
response_status_line = getResponseStatusLine(b'PATCH / HTTP/1.1\r\nHost: localhost\r\n\r\n')
assert response_status_line == "HTTP/1.1 501 Not Implemented"
print("\tUnimplemented method(501) test".ljust(34) + f"{GREEN}OK!{RESET}")

# Test a request with a uri that is too long
response_status_line = getResponseStatusLine(b'GET /' + b'a'*1100 + b' HTTP/1.1\r\nHost: localhost\r\n\r\n')
assert response_status_line == "HTTP/1.1 414 URI Too Long"
print("\tURI too long(414) test".ljust(34) + f"{GREEN}OK!{RESET}")

# Test a request with a payload that is too long
response_status_line = getResponseStatusLine(b'POST / HTTP/1.1\r\nHost: localhost\r\nContent-Length: 1000000\r\n\r\n' + b'a'*1000000)
assert response_status_line == "HTTP/1.1 413 Payload Too Large"
print("\tPayload too large(413) test".ljust(34) + f"{GREEN}OK!{RESET}")

# Terminate the server
webserv.send_signal(signal.SIGINT)

# Wait for the server to terminate
webserv.wait()

# Clean up
subprocess.run(['make', 'fclean'], cwd=os.path.join(os.path.dirname(__file__), '../../'), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
