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
        if self.path.startswith('/data'):
            params=getParams(self.path)
            #print("GET", params)
            if "temp" in params:
                print("Temperature:", params["temp"][0])
            else:
                print("Invalid params")

            self.send_response(200)
            self.end_headers()

print("Listening...")
httpd = socketserver.TCPServer(("", 80), MyHandler)
httpd.serve_forever()