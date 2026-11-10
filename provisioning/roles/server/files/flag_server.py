#!/usr/bin/env python3
"""
Dirty COW SeedLab Flag Verification Server
Runs on the 'server' VM (10.1.30.5:8080) to serve dynamic flags for APG
"""
import http.server
import socketserver
import os
import sys

PORT = 8080

FLAGS = {
    "/cow_flag/task1": os.environ.get("FLAG_TASK1", "{{ flag_task1 | default('FLAG{DIRTYCOW_DUMMY_WRITE_SUCCESS}') }}"),
    "/cow_flag/task2": os.environ.get("FLAG_TASK2", "{{ flag_task2 | default('FLAG{DIRTYCOW_ROOT_PRIV_ESC_PWNED}') }}")
}

class FlagHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        clean_path = self.path.split('?')[0].rstrip('/')
        if clean_path == "/health":
            self.send_response(200)
            self.send_header("Content-type", "text/plain")
            self.end_headers()
            self.wfile.write(b"OK\n")
            return

        if clean_path in FLAGS:
            flag_value = FLAGS[clean_path]
            self.send_response(200)
            self.send_header("Content-type", "text/plain")
            self.send_header("Access-Control-Allow-Origin", "*")
            self.end_headers()
            self.wfile.write((flag_value + "\n").encode())
        else:
            self.send_response(404)
            self.send_header("Content-type", "text/plain")
            self.end_headers()
            self.wfile.write(b"Endpoint not found. Valid endpoints: /cow_flag/task1, /cow_flag/task2\n")

    def log_message(self, format, *args):
        sys.stderr.write("%s - - [%s] %s\n" % (self.address_string(), self.log_date_time_string(), format % args))

if __name__ == "__main__":
    socketserver.TCPServer.allow_reuse_address = True
    with socketserver.TCPServer(("", PORT), FlagHandler) as httpd:
        print(f"[*] Dirty COW flag server listening on port {PORT}...")
        httpd.serve_forever()
