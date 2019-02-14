import threading

from qsim.logging import logging
from qsim.messages import tas_command_type
from qsim.task import task


class task_manager:
    __tasks = {}
    __script_mapping = {}
    __resource_locker = threading.RLock()


    @staticmethod
    def get_amount_sessions():
        return len(task_manager.__tasks)


    @staticmethod
    def create(queue_id, tas_request):
        with task_manager.__resource_locker:
            if queue_id in task_manager.__script_mapping:
                logging.debug("Notify about start message (queue id: '%s', amount tasks: '%d').", queue_id, len(task_manager.__tasks))

                task_id = task_manager.__script_mapping[queue_id]
                task_manager.__tasks[task_id].notify(tas_command_type.TASK_START)

            else:
                session_instance = task(queue_id, tas_request, task_manager)
                if session_instance is None:
                    return False

                task_id = session_instance.get_id()

                task_manager.__script_mapping[queue_id] = task_id
                task_manager.__tasks[task_id] = session_instance

                session_instance.start()

                logging.vip("Create new session instance (queue id: '%s', task id: '%s' amount tasks: '%d').", queue_id, task_id, len(task_manager.__tasks))

                session_instance.notify(tas_command_type.TASK_START)

        return True


    @staticmethod
    def launch(task_id):
        with task_manager.__resource_locker:
            logging.info("Launch session instance (task id: '%s').", task_id)
            task_manager.__tasks[task_id].start()


    @staticmethod
    def exist(task_id):
        with task_manager.__resource_locker:
            logging.debug("Amount of tasks '%d' (search task id '%s').", len(task_manager.__tasks), task_id)
            
            if task_id in task_manager.__tasks:
                return True
        
        return False


    @staticmethod
    def delete(session_id, internal=False):
        reply_code = None

        with task_manager.__resource_locker:
            if task_manager.exist(session_id):
                if internal is False:  # is already stopped by itself
                    reply_code = task_manager.__tasks[session_id].stop()

                queue_id = task_manager.__tasks[session_id].get_queue_id()
                task_manager.__script_mapping.pop(queue_id)
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
