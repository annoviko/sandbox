import threading;

from ivrsim.logging     import logging;
from ivrsim.session     import session;


class session_manager:
    __sessions = { };
    __resource_locker = threading.RLock();


    def __init__(self):
        assert(0);


    @staticmethod
    def get_amount_sessions():
        return len(session_manager.__sessions);


    @staticmethod
    def create(script_id, tas_request):
        with session_manager.__resource_locker:
            logging.vip("Create new session instance (script id: '%s', amount sessions: '%d').", script_id, len(session_manager.__sessions));
            session_instance = session(script_id, tas_request, session_manager);
            if (session_instance is None):
                return None;
            
            session_id = session_instance.get_id();
    
            session_manager.__sessions[session_id] = session_instance;
        
        return session_id;


    @staticmethod
    def launch(session_id):
        with session_manager.__resource_locker:
            logging.info("Launch session instance (session id: '%s').", session_id);
            session_manager.__sessions[session_id].start();


    @staticmethod
    def exist(session_id):
        with session_manager.__resource_locker:
            logging.debug("Amount of sessions '%d' (search session id '%s').", len(session_manager.__sessions), session_id);
            
            if (session_id in session_manager.__sessions):
                return True;
        
        return False;


    @staticmethod
    def delete(session_id, internal = False):
        reply_code = None;
        with session_manager.__resource_locker:
            if (internal is False):
                reply_code = session_manager.__sessions[session_id].stop();
            
            if (session_manager.exist(session_id)):
                session_manager.__sessions.pop(session_id);
                logging.debug("Delete session instance (session id: '%s', sessions '%d').", session_id, len(session_manager.__sessions));
        
        return reply_code;


    @staticmethod
    def notify(session_id, tas_notification_id, message_payload):
        with session_manager.__resource_locker:
            if (session_manager.exist(session_id)):
                logging.debug("Convey notifiction to session instance (session id: '%s', sessions '%d').", session_id, len(session_manager.__sessions));
                
                return session_manager.__sessions[session_id].notify(tas_notification_id, message_payload);
        
        return None;
