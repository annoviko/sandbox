import httplib;

from ivrsim.logging import logging;
from ivrsim.statistical import statistical;


class http_client:
    def __init__(self, address, port):
        self.__address = address;
        self.__port = port;


    def send_request(self, http_method, http_link, json_request):
        response_status = 404;
        response_body = None;

        connection = httplib.HTTPConnection(self.__address, self.__port, timeout = 1);

        try:
            logging.debug("Send HTTP request to TAS (%s:%s): '%s' '%s'.", self.__address, self.__port, http_method, http_link);
            logging.debug("JSON payload of the HTTP request to TAS: '%s'.", json_request);
            
            headers = {'Content-Type': 'application/json' };
            
            body = None;
            if (json_request is not None):
                body = json_request.encode("utf-8");
                headers['Content-Length'] = len(body);
            
            statistical.inc_ivr_requests();
            connection.request(http_method, http_link, body, headers);
            
            logging.debug("Check for HTTP response from TAS.");
            response = connection.getresponse();
            response_status = response.status;
    
            logging.debug("Read body of the HTTP response from TAS.");
            response_body = response.read();
            
            statistical.inc_ivr_responses();
            logging.debug("HTTP response (code: %d) from TAS:\n%s", response_status, response_body);

        except:
            response_status = 404;
            logging.error("Impossible to communicate correctly with TAS using HTTP.");
              
        finally:
            connection.close();
        
        return (response_status, response_body);