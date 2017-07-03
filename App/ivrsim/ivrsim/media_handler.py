from SimpleHTTPServer           import SimpleHTTPRequestHandler;

import re;
import os;

from ivrsim.configuration       import configuration;
from ivrsim.logging             import logging;


class media_handler(SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        self.server_version = "MediaServer/";
        self.sys_version = "";

        SimpleHTTPRequestHandler.__init__(self, *args, **kwargs);


    def do_GET(self):
        logging.info("Receive GET media request: '%s'.", self.path);
        
        reg_object = re.match("/media/(\S+)", self.path);
        if (reg_object is None):
            self.reply(404);
            return;
    
        media_name = reg_object.group(1);
        media_filepath = os.path.join(os.path.dirname(__file__), configuration.get_media_storage(), media_name);
        if (media_filepath is False):
            self.reply(404);
            return;
        
        if (os.path.isfile(media_filepath) is False):
            self.reply(404);
            return;
        
        media_descriptior = open(media_filepath);
        file_stat = os.fstat(media_descriptior.fileno());
        
        self.send_response(200);
        self.send_header('Content-type', 'audio/x-wav');
        self.send_header('Content-Length', file_stat.st_size);
        self.send_header('Accept-Ranges', 'bytes');
        self.end_headers();
        
        self.wfile.write(media_descriptior.read());
        
        media_descriptior.close();
        
        logging.info("Sent audio file '%s'.", media_name);


    def reply(self, code):
        self.send_response(code);
        self.end_headers();
