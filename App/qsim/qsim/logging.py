import datetime
import inspect
import threading
import os


class prefix:
    DEBUG       = "[     DEBUG] "
    INFO        = "[      INFO] "
    WARNING     = "[   WARNING] "
    ERROR       = "[     ERROR] "
    VIP         = "[       VIP] "
    USER        = "[      USER] "


class color:
    DEFAULT     = "\033[0m"
    DEBUG       = "\033[0m"
    INFO        = "\033[0m"
    WARNING     = "\033[33m"
    ERROR       = "\033[31m"
    VIP         = "\033[32m"
    USER        = "\033[96m"


class logging:
    __resource_locker = threading.RLock()
    __file_log = None


    @staticmethod
    def __time_mark():
        return datetime.datetime.now().strftime("%m.%d %H:%M:%S.%f ")


    @staticmethod
    def __write(message_prefix, message_color, file_caller, message, *args):
        with logging.__resource_locker:
            message = logging.__time_mark() + message_color + message_prefix + file_caller + ": " + message + color.DEFAULT
            print(message % args)

            if logging.__file_log is not None:
                file_descriptor = open(logging.__file_log, 'a')
                file_descriptor.write((logging.__time_mark() + message_prefix + file_caller + ": " + message + "\n") % args)
                file_descriptor.close()


    @staticmethod
    def set_filename(filename):
        logging.__file_log = filename


    @staticmethod
    def debug(message, *args):
        (_, filename, line_number, _, _, _) = inspect.getouterframes(inspect.currentframe())[1]
        caller = os.path.basename(filename) + ":" + str(line_number)

        logging.__write(prefix.DEBUG, color.DEBUG, caller, message, *args)


    @staticmethod
    def info(message, *args):
        (_, filename, line_number, _, _, _) = inspect.getouterframes(inspect.currentframe())[1]
        caller = os.path.basename(filename) + ":" + str(line_number)

        logging.__write(prefix.INFO, color.INFO, caller, message, *args)


    @staticmethod
    def warning(message, *args):
        (_, filename, line_number, _, _, _) = inspect.getouterframes(inspect.currentframe())[1]
        caller = os.path.basename(filename) + ":" + str(line_number)

        logging.__write(prefix.WARNING, color.WARNING, caller, message, *args)


    @staticmethod
    def error(message, *args):
        (_, filename, line_number, _, _, _) = inspect.getouterframes(inspect.currentframe())[1]
        caller = os.path.basename(filename) + ":" + str(line_number)

        logging.__write(prefix.ERROR, color.ERROR, caller, message, *args)


    @staticmethod
    def vip(message, *args):
        (_, filename, line_number, _, _, _) = inspect.getouterframes(inspect.currentframe())[1]
        caller = os.path.basename(filename) + ":" + str(line_number)

        logging.__write(prefix.VIP, color.VIP, caller, message, *args)


    @staticmethod
    def user(message, *args):
        logging.__write(prefix.USER, color.USER, "", message, *args)
