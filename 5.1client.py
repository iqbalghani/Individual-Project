import socket

s = socket.socket()

port = 8887

s.connect(('192.168.56.104', port))

data = s.recv(1024)

s.send(b'Hi, saya client. Terima Kasih!');

print (data)

s.close()
