import threading

from qsim.logging import logging
from qsim.task import task


class task_manager:
    __tasks = {}
    __resource_locker = threading.RLock()


    def __init__(self):
        assert 0


    @staticmethod
    def get_amount_sessions():
        return len(task_manager.__tasks)


    @staticmethod
    def create(script_id, tas_request):
        with task_manager.__resource_locker:
            logging.vip("Create new session instance (script id: '%s', amount tasks: '%d').", script_id, len(task_manager.__tasks))
            session_instance = task(script_id, tas_request, task_manager)
            if session_instance is None:
                return None

            task_id = session_instance.get_id()

            task_manager.__tasks[task_id] = session_instance
        
        return task_id


    @staticmethod
    def launch(task_id):
        with task_manager.__resource_locker:
            logging.info("Launch session instance (task id: '%s').", task_id)
            task_manager.__tasks[task_id].start()


    @staticmethod
    def exist(session_id):
        with task_manager.__resource_locker:
            logging.debug("Amount of sessions '%d' (search session id '%s').", len(task_manager.__tasks), session_id)
            
            if session_id in task_manager.__tasks:
                return True
        
        return False


    @staticmethod
    def delete(session_id, internal=False):
        reply_code = None
        with task_manager.__resource_locker:
            if internal is False:
                reply_code = task_manager.__tasks[session_id].stop()
            
            if task_manager.exist(session_id):
                task_manager.__tasks.pop(session_id)
                logging.debug("Delete session instance (task id: '%s', sessions '%d').", session_id, len(task_manager.__tasks))

        return reply_code


    @staticmethod
    def notify(session_id, tas_notification_id, message_payload):
        with task_manager.__resource_locker:
            if task_manager.exist(session_id):
                logging.debug("Convey notification to session instance (session id: '%s', sessions '%d').", session_id, len(task_manager.__tasks))
                
                return task_manager.__tasks[session_id].notify(tas_notification_id, message_payload)
        
        return None
