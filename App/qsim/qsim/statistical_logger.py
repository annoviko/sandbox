import threading

from qsim.logging import logging
from qsim.statistical import statistical


class statistical_logger:
    def __init__(self, logging_period_seconds=20):
        self.__stop_flag = False
        self.__logging_period_seconds = logging_period_seconds
        self.__stop_condition = threading.Condition()
        self.__logger = None


    def start(self):
        self.__stop_flag = False
        
        self.__logger = threading.Thread(target=self.__run)
        self.__logger.start()


    def stop(self):
        with self.__stop_condition:
            self.__stop_flag = True
            self.__stop_condition.notify_all()
        
        self.__logger.join()


    def __run(self):
        while self.__stop_flag is not True:
            with self.__stop_condition:
                self.__stop_condition.wait(self.__logging_period_seconds)

            logging.info("QSim Simulator statistics:\n\n"
                         "\tTAS REQs (in):   %4d,    TAS RESPs (in):   %4d\n"
                         "\tQSim REQs (out): %4d,    QSim RESPs (out): %4d\n\n",
                         statistical.get_tas_requests(), statistical.get_tas_responses(),
                         statistical.get_qsim_requests(), statistical.get_qsim_responses())
