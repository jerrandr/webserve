import socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("localhost", 8080))
request = (
    "POST /upload HTTP/1.1\r\n"
    "Host: localhost\r\n"
    "User-Agent: TestClient/1.0\r\n"
    "Transfer-Encoding: chunked\r\n"
    "Content-Type: application/json\r\n"
    "\r\n"
    "4\r\n"
    '{"na\r\n'
    "6\r\n"
    'me": "\r\n'
    "8\r\n"
    'ChatGPT"\r\n'
    "6\r\n"
    ', "age\r\n'
    "3\r\n"
    '": 3\r\n'
    "7\r\n"
    '0, "ci\r\n'
    "6\r\n"
    'ty": "\r\n'
    "8\r\n"
    'OpenAI"}\r\n'
    "0\r\n"
    "\r\n"
)
s.sendall(request.encode())
response = s.recv(4096)
print(response.decode(errors="ignore"))
s.close()