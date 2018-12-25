import os
import sys

from qsim.code_block import code_block
from qsim.error_code import error_code
from qsim.logging import logging


class script:
    __DEFAULT_FOLDER = os.path.join(os.path.dirname(__file__), 'scripts')


    @staticmethod
    def exist(script_id):
        script_filepath = os.path.join(script.__DEFAULT_FOLDER, script_id)
        return os.path.isfile(script_filepath)


    @staticmethod
    def load(script_id):
        script_filepath = os.path.join(script.__DEFAULT_FOLDER, script_id)
        if script_filepath is False:
            logging.error("Impossible to load script because it does not exist.")
            sys.exit(error_code.ERROR_SCRIPT_NOT_FOUND)
        
        with open(script_filepath) as file_descriptor:
            content = file_descriptor.readlines()
        
        block = code_block(0, content)
        block.load()
        
        return block
