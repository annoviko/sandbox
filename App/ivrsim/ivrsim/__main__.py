import sys;
import os;

sys.path.append(os.path.join(os.path.dirname(__file__), '..'));


from ivrsim.configuration import configuration;
from ivrsim.http_server import http_server;
from ivrsim.logging import logging;


def run():
    configuration.load();

    logging_filename = None;
    if (configuration.get_logging_file_enable()):
        logging_filename = configuration.get_logging_file();

    logging.set_filename(logging_filename);

    simulator = http_server("", configuration.get_ivr_port());
    simulator.start();


if __name__ == '__main__':
    run();
