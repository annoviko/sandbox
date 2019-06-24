import http.client

from qsim.logging import logging
from qsim.statistical import statistical


class http_client:
    def __init__(self, address, port):
        self.__address = address
        self.__port = port


    def send_request(self, http_method, http_link, json_request, general_headers):
        response_body = None

        connection = http.client.HTTPConnection(self.__address, self.__port, timeout=1)

        try:
            logging.vip("Send HTTP request to TAS (%s:%s): '%s' '%s'.", self.__address, self.__port, http_method, http_link)
            logging.debug("JSON payload of the HTTP request to TAS: '%s'.", json_request)

            headers = {"Content-Type": "application/json",
                       "Accept": "*/*"}

            body = None
            if json_request is not None:
                body = json_request.encode("utf-8")
                headers['Content-Length'] = len(body)

            headers.update(general_headers)

            statistical.inc_qsim_requests()
            connection.request(http_method, http_link, body, headers)

            logging.debug("Check for HTTP response from TAS.")
            response = connection.getresponse()
            response_status = response.status

            logging.debug("Read body of the HTTP response from TAS.")
            response_body = response.read()

            statistical.inc_qsim_responses()
            logging.vip("HTTP response (code: %d) from TAS:", response_status)
            logging.debug(response_body.decode("utf-8"))

        except Exception as expection_object:
            response_status = 600
            logging.error("Impossible to communicate correctly with TAS using HTTP (reason: '%s')." % expection_object)

        finally:
            connection.close()
        
        return response_status, response_body
