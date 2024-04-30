#!/usr/bin/python3

import os
import sys
import cgi
import time

# Set the Content-Type header to text/plain
print("Content-Type: text/plain")
print()

# Read and output data from the request body (if present)
if "CONTENT_LENGTH" in os.environ:
    content_length = int(os.environ["CONTENT_LENGTH"])
    post_data = sys.stdin.buffer.read(content_length)
    print("\nPOST Data:")
    print(post_data.decode("utf-8"))
