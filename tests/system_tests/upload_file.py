import requests
import subprocess
import os
import re
import sys
import time
import signal

# ANSI escape codes for colors
GREEN = '\033[92m'
RED = '\033[91m'
YELLOW = '\033[93m'
RESET = '\033[0m'

def	make(directory):
	try:
		subprocess.run(['make', 'fclean'], cwd=os.path.join(os.path.dirname(__file__), '../../'), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
		subprocess.run(['make', '-j8'], cwd=os.path.join(os.path.dirname(__file__), '../../'), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
		return True
	except subprocess.CalledProcessError as e:
		return False

def upload_file(file_path, url):
	# Open the file in binary mode
	with open(file_path, 'rb') as file:
		# Define the files dictionary to send in the POST request
		files = {'file': file}

		# Make the POST request to upload the file
		response = requests.post(url, files=files)

		#todo: check for timeouts etc.

		# Print the response from the server
		if (response.status_code == 201 or response.status_code == 200):
			return 0
		else:
			return 1

# if __name__ == "__main__":
directory = "."
# Path to the file you want to upload
file_path = 'system_tests/upload_file.py'

# The URL to which the file will be uploaded
upload_url = 'http://localhost:8080/upload'

if (not make(".")):
	sys.exit(1)
server = subprocess.Popen(['./webserv'], cwd=os.path.join(os.path.dirname(__file__), '../../'), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
# wait for the server to be ready
time.sleep(3)
upload_status = upload_file(file_path, upload_url)
time.sleep(3)
server.send_signal(signal.SIGINT)
server_status = server.wait()
assert upload_status == 0
subprocess.run(['make', 'fclean'], cwd=os.path.join(os.path.dirname(__file__), '../../'), stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
