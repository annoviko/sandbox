import threading;


class statistical:
    __AMOUNT_TAS_REQUESTS   = 0;
    __AMOUNT_TAS_RESPONSES  = 0;
    __AMOUNT_IVR_REQUESTS   = 0;
    __AMOUNT_IVR_RESPONSES  = 0;
    
    __resource_locker = threading.RLock();


    @staticmethod
    def inc_tas_requests():
        with statistical.__resource_locker:
            statistical.__AMOUNT_TAS_REQUESTS += 1;


    @staticmethod
    def inc_tas_responses():
        with statistical.__resource_locker:
            statistical.__AMOUNT_TAS_RESPONSES += 1;


    @staticmethod
    def inc_ivr_requests():
        with statistical.__resource_locker:
            statistical.__AMOUNT_IVR_REQUESTS += 1;


    @staticmethod
    def inc_ivr_responses():
        with statistical.__resource_locker:
            statistical.__AMOUNT_IVR_RESPONSES += 1;


    @staticmethod
    def get_tas_requests():
        with statistical.__resource_locker:
            return statistical.__AMOUNT_TAS_REQUESTS;


    @staticmethod
    def get_tas_responses():
        with statistical.__resource_locker:
            return statistical.__AMOUNT_TAS_RESPONSES;


    @staticmethod
    def get_ivr_requests():
        with statistical.__resource_locker:
            return statistical.__AMOUNT_IVR_REQUESTS;


    @staticmethod
    def get_ivr_responses():
        with statistical.__resource_locker:
            return statistical.__AMOUNT_IVR_RESPONSES;
