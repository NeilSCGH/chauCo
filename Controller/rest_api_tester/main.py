from lib import socketserver
from lib.server import BaseHTTPRequestHandler
from urllib.parse import parse_qs

def getParams(uri):
    if "?" in uri:
        param=uri.split("?")
        param=param[1]
        param=parse_qs(param)
        return param
    return {}

class MyHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        #if self.path.startswith('/temp'):
        params=getParams(self.path)
        print("GET", params)

        self.send_response(200)
        self.end_headers()

    def do_POST(self):
        params=getParams(self.path)
        print("POST", params)

        self.send_response(200)
        self.end_headers()

print("Listening...")
httpd = socketserver.TCPServer(("", 80), MyHandler)
httpd.serve_forever()