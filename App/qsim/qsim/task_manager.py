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
        tas_session_id = tas_request["session_id"]

        with task_manager.__resource_locker:
            if tas_session_id in task_manager.__script_mapping:
                logging.debug("Notify about start message (queue ID: '%s', session ID: '%s', amount tasks: '%d').",
                              queue_id, tas_session_id, len(task_manager.__tasks))

                task_id = task_manager.__script_mapping[tas_session_id]
                task_manager.__tasks[task_id].notify(tas_command_type.TASK_START)

            else:
                session_instance = task(queue_id, tas_request, task_manager)
                if session_instance is None:
                    return False

                task_id = session_instance.get_id()

                task_manager.__script_mapping[tas_session_id] = task_id
                task_manager.__tasks[task_id] = session_instance

                session_instance.start()

                logging.vip("Create new session instance (queue ID: '%s', session ID: '%s', task ID: '%s', amount tasks: '%d').",
                            queue_id, tas_session_id, task_id, len(task_manager.__tasks))

                session_instance.notify(tas_command_type.TASK_START)

        return True


    @staticmethod
    def launch(task_id):
        with task_manager.__resource_locker:
            logging.info("Launch session instance (task ID: '%s').", task_id)
            task_manager.__tasks[task_id].start()


    @staticmethod
    def exist(task_id):
        with task_manager.__resource_locker:
            logging.debug("Amount of tasks '%d' (search task ID: '%s').", len(task_manager.__tasks), task_id)
            
            if task_id in task_manager.__tasks:
                return True
        
        return False


    @staticmethod
    def get_session_by_command_id(command_id):
        for task_id in task_manager.__tasks:
            session_instance = task_manager.__tasks[task_id]
            if session_instance.contains_action_id(command_id) is True:
                return session_instance

        return None


    @staticmethod
    def delete(task_id, internal=False):
        reply_code = None

        with task_manager.__resource_locker:
            if task_manager.exist(task_id):
                if internal is False:  # is already stopped by itself
                    reply_code = task_manager.__tasks[task_id].stop()

                tas_session_id = task_manager.__tasks[task_id].get_session_id()

                task_manager.__script_mapping.pop(tas_session_id)
                task_manager.__tasks.pop(task_id)
                logging.debug("Delete session instance (task ID: '%s', session ID: '%s' amount tasks: '%d').",
                              task_id, tas_session_id, len(task_manager.__tasks))

        return reply_code


    @staticmethod
    def notify(task_id, tas_notification_id, message_payload):
        with task_manager.__resource_locker:
            if task_manager.exist(task_id):
                logging.debug("Convey notification to session instance (task ID: '%s', amount tasks: '%d').",
                              task_id, len(task_manager.__tasks))
                
                return task_manager.__tasks[task_id].notify(tas_notification_id, message_payload)
        
        return None
