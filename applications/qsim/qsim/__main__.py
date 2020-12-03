import sys
import os


sys.path.append(os.path.join(os.path.dirname(__file__), '..'))


from qsim.configuration import configuration
from qsim.http_server import http_server
from qsim.logging import logging


def run():
    configuration.load()

    logging_filename = None
    if configuration.get_logging_file_enable():
        logging_filename = configuration.get_logging_file()

    logging.set_filename(logging_filename)

    simulator = http_server("", configuration.get_qsim_port())
    simulator.start()


if __name__ == '__main__':
    run()
