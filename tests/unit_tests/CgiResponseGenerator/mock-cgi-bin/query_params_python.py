#!/usr/bin/python3

import cgi

# Parse query parameters from the request URL
query_params = cgi.FieldStorage()

# Set the Content-Type header to text/plain
print("Content-Type: text/plain")
print()

# Output some text
print("Hello, world!")

# Output query parameters
print("Query Parameters:")
for param in query_params:
    print(f"{param} = {query_params[param].value}")
