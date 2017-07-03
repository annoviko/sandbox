import unittest;

from ivrsim.http_definition         import http_method;
from ivrsim.http_parser             import http_parser, struct_field;
from ivrsim.messages                import tas_command_type;


class Test(unittest.TestCase):
    def test_start_ivr_success_1(self):
        request = http_parser.parse(http_method.HTTP_POST, "/telephony/v1/account/1000001/routing-services/ivrs/0000001/instances");
        
        assert tas_command_type.IVR_START == request[struct_field.id];
        assert "1000001" == request[struct_field.account_id];
        assert "0000001" == request[struct_field.script_id];


    def test_start_ivr_fail_1(self):
        request = http_parser.parse(http_method.HTTP_POST, "/telephony/v1/account/1000001/routing-services/ivrs/0000001/something");
        assert None == request;


    def test_start_ivr_fail_2(self):
        request = http_parser.parse(http_method.HTTP_POST, "/telephony/v1/account/1000001/routing-services/ivrs/0000001/something");
        assert None == request;


    def test_stop_ivr_success_1(self):
        request = http_parser.parse(http_method.HTTP_DELETE, "/telephony/v1/account/1000002/routing-services/ivrs/0000002/instances/1234567");
        
        assert tas_command_type.IVR_STOP == request[struct_field.id];
        assert "1000002" == request[struct_field.account_id];
        assert "0000002" == request[struct_field.script_id];
        assert "1234567" == request[struct_field.session_id];


    def test_stop_ivr_fail_1(self):
        request = http_parser.parse(http_method.HTTP_DELETE, "/telephony/v1/account/1000001/routing-services/ivrs/0000001/instances");
        assert None == request;


    def test_execution_status_success_1(self):
        request = http_parser.parse(http_method.HTTP_GET, "/telephony/v1/account/1000003/routing-services/ivrs/0000003/instances/2345678");
        
        assert tas_command_type.EXECUTION_STATUS == request[struct_field.id];
        assert "1000003" == request[struct_field.account_id];
        assert "0000003" == request[struct_field.script_id];
        assert "2345678" == request[struct_field.session_id];


    def test_execution_status_fail_1(self):
        request = http_parser.parse(http_method.HTTP_GET, "/telephony/v1/account/1000001/routing-services/ivrs/0000001/");
        assert None == request;


    def test_service_status_success_1(self):
        request = http_parser.parse(http_method.HTTP_GET, "/telephony/v1/account/1000001/routing-services/status");
        assert tas_command_type.SERVICE_STATUS == request[struct_field.id];
        assert "1000001" == request[struct_field.account_id];


    def test_service_status_fail_1(self):
        request = http_parser.parse(http_method.HTTP_GET, "/telephony/v1/account/1000001/routing-services/ivrs/0000001/");
        assert None == request;


if __name__ == "__main__":
    unittest.main();