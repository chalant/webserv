import socket

server_address = ('localhost', 8080)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(server_address)
# Send your HTTP request
s.sendall(b'GET / HTTP/1.1\r\nHost: localhost\r\n\r\n')
# Receive the response
response = s.recv(4096)
print("--------RESPONSE--------")
print(response.decode())