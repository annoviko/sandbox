from queue import Queue


class event_response:
    def __init__(self, code, message, headers, body):
        self.code = code
        self.message = message
        self.headers = headers
        self.body = body


class event_start_response(event_response):
    def __init__(self, code, message, headers, body):
        super().__init__(code, message, headers, body)


class event_stop_response(event_response):
    def __init__(self, code, message, headers, body):
        super().__init__(code, message, headers, body)


class event_ignore():
    pass


queue = Queue()
