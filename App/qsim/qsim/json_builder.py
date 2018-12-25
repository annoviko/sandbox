import json


class json_builder:
    @staticmethod
    def start_qsim_response(session_id):
        response = {"id": session_id}
        return json.dumps(response)
    
    
    @staticmethod
    def stop_qsim_response():
        response = {}
        response["content"] = [{"parameter":  "",
                                "value":      ""}]
        return json.dumps(response)
    
    
    @staticmethod
    def execution_status_response(state):
        response = {"state": ""}
        return json.dumps(response)
    
    
    @staticmethod
    def service_status_response(state, counters):
        response = {"health": {"state": state}}

        if counters is not None:
            response["counters"] = counters
        
        return json.dumps(response)
