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

# Compile the project
subprocess.run(['make', '-j8'], cwd=os.path.join(os.path.dirname(__file__), '../../'), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

# Start the server
webserv = subprocess.Popen(['./webserv'], cwd=os.path.join(os.path.dirname(__file__), '../../'), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

#Wait for the server to start
time.sleep(1)

# Test the siege command
siege = subprocess.Popen(['siege', '-b', '-t10s', '127.0.0.1:8080'], stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)

# Wait for siege to finish
time.sleep(12)

# Check that availability is 100%
siege_err = siege.stderr.read().decode()
assert "Availability:		      100.00 %" in siege_err
#print("\tsiege_10_sec test".ljust(34) + f"{GREEN}OK!{RESET}")

# Terminate siege just in case
siege.kill()

# Terminate the server
webserv.kill()

# Clean up
subprocess.run(['make', 'fclean'], cwd=os.path.join(os.path.dirname(__file__), '../../'), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
