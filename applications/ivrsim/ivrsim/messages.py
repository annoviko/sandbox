class tas_command_type:
    IVR_START                   = 0x0001;
    IVR_STOP                    = 0x0002;
    EXECUTION_STATUS            = 0x0003;
    SERVICE_STATUS              = 0x0004;

    RESULT_PLAY                 = 0x0101;
    RESULT_SAY                  = 0x0102;
    RESULT_COLLECT              = 0x0103;

    # Ugly solution because of Python27, it the beginning it was Python36 and it was IntEnum.
    __NAME_MAP = { IVR_START :         'IVR_START',
                   IVR_STOP :          'IVR_STOP',
                   EXECUTION_STATUS :  'EXECUTION_STATUS',
                   SERVICE_STATUS :    'SERVICE_STATUS',
                   RESULT_PLAY :       'RESULT_PLAY',
                   RESULT_SAY :        'RESULT_SAY',
                   RESULT_COLLECT :    'RESULT_COLLECT'        };

    @staticmethod
    def to_string(value):
        return tas_command_type.__NAME_MAP[value];
