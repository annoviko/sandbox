import unittest;

from sct import signalling;


class Test(unittest.TestCase):
    DEFAULT_SCRIPT_ID = "90235125229";
    __HTTP_TAS_SERVER_STUB = None;


    @classmethod
    def setUpClass(cls):
        print("Run HTTP TAS Server stub.");
        Test.__HTTP_TAS_SERVER_STUB = signalling.http_tas_stub_server();
        Test.__HTTP_TAS_SERVER_STUB.start();


    @classmethod
    def tearDownClass(cls):
        print("Stop HTTP TAS Server stub.");
        Test.__HTTP_TAS_SERVER_STUB.stop();


    def setUp(self):
        signalling.signalling_queue.clear();


    def tearDown(self):
        signalling.signalling_queue.clear();


    def template_send_start_ivr(self, script_id = DEFAULT_SCRIPT_ID):
        (status, content) = signalling.send_start_ivr(script_id);
          
        self.assertEqual(201, status);
        self.assertNotEqual({ }, content);
        self.assertNotEqual(None, content.get("id", None));
          
        return content["id"];


    def template_send_stop_ivr(self, ivr_session_id, script_id = DEFAULT_SCRIPT_ID, reply_code = 202):
        (status, content) = signalling.send_stop_ivr(script_id, ivr_session_id);
          
        self.assertEqual(reply_code, status);
        self.assertNotEqual({ }, content);
        self.assertNotEqual(None, content.get("content", None));


    def template_process_play(self, ivr_session_id, play_result_response_code = 200):
        self.assertIsNotNone(signalling.receive_play());
        (status, content) = signalling.send_result_play(Test.DEFAULT_SCRIPT_ID, ivr_session_id, "123456789");   # Action ID is not taken into account by IVR Simulator
        self.assertEqual(play_result_response_code, status);


    def test_start_stop(self):
        ivr_session_id = self.template_send_start_ivr();
        self.template_send_stop_ivr(ivr_session_id);


    def test_start_wrong_script_id(self):
        (status, content) = signalling.send_start_ivr("101010101010101");
               
        self.assertEqual(404, status);


    def test_stop_wrong_script_id(self):
        ivr_session_id = self.template_send_start_ivr();
        incorrect_session_id = int(ivr_session_id) + 1;
               
        (status, content) = signalling.send_stop_ivr(Test.DEFAULT_SCRIPT_ID, str(incorrect_session_id));
        self.assertEqual(404, status);
               
        # Terminate anyway
        self.template_send_stop_ivr(ivr_session_id);


    def test_start_several_sessions(self):
        ivr_session_id1 = self.template_send_start_ivr();
        ivr_session_id2 = self.template_send_start_ivr();
        ivr_session_id3 = self.template_send_start_ivr();
               
        self.template_send_stop_ivr(ivr_session_id1);
        self.template_send_stop_ivr(ivr_session_id2);
        self.template_send_stop_ivr(ivr_session_id3);


    def test_stop_without_session(self):
        (status, content) = signalling.send_stop_ivr(Test.DEFAULT_SCRIPT_ID, "1234567890");
        self.assertEqual(404, status);


    def test_execute_status(self):
        ivr_session_id = self.template_send_start_ivr();
        (status, content) = signalling.send_execution_status(Test.DEFAULT_SCRIPT_ID, ivr_session_id);
                  
        self.assertEqual(200, status);
        self.assertNotEqual({ }, content);
        self.assertNotEqual(None, content.get("state", None));
              
        self.template_send_stop_ivr(ivr_session_id);


    def test_service_status(self):
        (status, content) = signalling.send_service_status();
         
        self.assertEqual(200, status);
        self.assertNotEqual({ }, content);
        self.assertNotEqual(None, content.get("health", None));
        self.assertNotEqual(None, content.get("counters", None));


    def test_result_play_say(self):
        ivr_session_id = self.template_send_start_ivr();
               
        self.assertIsNotNone(signalling.receive_play(timeout = 3));   # receive play command from script
        (status, content) = signalling.send_result_play(Test.DEFAULT_SCRIPT_ID, ivr_session_id, "123456789");   # Action ID is not taken into account by IVR Simulator
        self.assertEqual(200, status);
               
        self.assertIsNotNone(signalling.receive_say(timeout = 3));    # receive say command from script
        (status, content) = signalling.send_result_say(Test.DEFAULT_SCRIPT_ID, ivr_session_id, "123456789");    # Action ID is not taken into account by IVR Simulator
        self.assertEqual(200, status);


    def test_result_play_say_collect(self):
        ivr_session_id = self.template_send_start_ivr(script_id = "90235125230");
               
        self.assertIsNotNone(signalling.receive_play(timeout = 3));       # receive play command from script
        (status, content) = signalling.send_result_play("90235125230", ivr_session_id, "123456789");   # Action ID is not taken into account by IVR Simulator
        self.assertEqual(200, status);
               
        self.assertIsNotNone(signalling.receive_say(timeout = 3));        # receive say command from script
        (status, content) = signalling.send_result_say("90235125230", ivr_session_id, "123456789");    # Action ID is not taken into account by IVR Simulator
        self.assertEqual(200, status);
       
        self.assertIsNotNone(signalling.receive_collect(timeout = 3));    # receive collect command from script
        (status, content) = signalling.send_result_collect("90235125230", ivr_session_id, "123456789");    # Action ID is not taken into account by IVR Simulator
        self.assertEqual(200, status);


    def test_stop_actions(self):
        ivr_session_id = self.template_send_start_ivr(script_id = "90235125231");
             
        self.assertIsNotNone(signalling.receive_play(timeout = 3));
        self.assertIsNotNone(signalling.receive_stop_play(timeout = 3));
        (status, content) = signalling.send_result_play("90235125231", ivr_session_id, "123456789");   # Action ID is not taken into account by IVR Simulator
        self.assertEqual(200, status);
     
        self.assertIsNotNone(signalling.receive_say(timeout = 3));
        self.assertIsNotNone(signalling.receive_stop_say(timeout = 3));
        (status, content) = signalling.send_result_say("90235125231", ivr_session_id, "123456789");    # Action ID is not taken into account by IVR Simulator
        self.assertEqual(200, status);
      
        self.assertIsNotNone(signalling.receive_collect(timeout = 3));
        self.assertIsNotNone(signalling.receive_stop_collect(timeout = 3));
        (status, content) = signalling.send_result_collect("90235125231", ivr_session_id, "123456789");    # Action ID is not taken into account by IVR Simulator
        self.assertEqual(200, status);


    def test_forward_command(self):
        self.template_send_start_ivr(script_id = "90235125232");
        self.assertIsNotNone(signalling.receive_forward());


    def test_idle_active_session(self):
        ivr_session_id_active = self.template_send_start_ivr(script_id = "90235125229");
        ivr_session_id_idle = self.template_send_start_ivr(script_id = "90235125229");
 
        self.assertIsNotNone(signalling.receive_play());
        self.assertIsNotNone(signalling.receive_play());
          
        (status, content) = signalling.send_result_play(Test.DEFAULT_SCRIPT_ID, ivr_session_id_active, "123456789");   # Action ID is not taken into account by IVR Simulator
        self.assertEqual(200, status);
 
        self.assertIsNotNone(signalling.receive_say(timeout = 3));    # receive say command from script
        (status, content) = signalling.send_result_say(Test.DEFAULT_SCRIPT_ID, ivr_session_id_active, "123456789");    # Action ID is not taken into account by IVR Simulator
        self.assertEqual(200, status);
         
        self.template_send_stop_ivr(ivr_session_id_idle, "90235125229");


    def test_play_forward(self):
        ivr_session_id = self.template_send_start_ivr(script_id = "90235125233");

        self.assertIsNotNone(signalling.receive_play());
        
        (http_header, json_body) = signalling.receive_forward();
        self.assertIsNotNone(http_header);
        self.assertIsNotNone(json_body);
        self.assertEqual("DropCall", json_body['type']);
        
        self.template_send_stop_ivr(ivr_session_id, "90235125229");


    def test_stop_collect_specify_id(self):
        ivr_session_id = self.template_send_start_ivr(script_id = "90235125234");
        
        (header, json_body) = signalling.receive_collect();
        collect_id = header.group(signalling.action_group.ACTION_ID);
        
        self.assertEqual("45678", collect_id);
        
        (header, json_body) = signalling.receive_collect();
        collect_id = header.group(signalling.action_group.ACTION_ID);
        
        self.assertEqual("12345", collect_id);


    def test_trigger_stop_ivr_01(self):
        ivr_session_id = self.template_send_start_ivr(script_id = "90235125235");
        
        self.template_process_play(ivr_session_id);
        
        # Send stop IVR before receiving forward
        self.template_send_stop_ivr(ivr_session_id, "90235125235");
        self.assertIsNotNone(signalling.receive_forward());

    
    def test_trigger_stop_ivr_02(self):
        # reply code is checking here
        ivr_session_id = self.template_send_start_ivr(script_id = "90235125237");

        self.template_process_play(ivr_session_id);

        self.template_send_stop_ivr(ivr_session_id, "90235125237", 404);
        self.assertIsNotNone(signalling.receive_forward());


    def test_conditional_script_01(self):
        else_branch_cause = "{ \"code\":   \"202\", \"description\": \"Success\" }";
        ivr_session_id = self.template_send_start_ivr(script_id = "90235125236");
        
        self.assertIsNotNone(signalling.receive_play());
        signalling.send_result_play(Test.DEFAULT_SCRIPT_ID, ivr_session_id, "123456789");   # Action ID is not taken into account by IVR Simulator
        
        self.assertIsNotNone(signalling.receive_collect());
        signalling.send_result_collect(Test.DEFAULT_SCRIPT_ID, ivr_session_id, "123456789", else_branch_cause);

        (header, json_body) = signalling.receive_play();
        self.assertEqual(json_body["files"][0], "bye.wav");
        
        self.template_send_stop_ivr(ivr_session_id, "90235125236");


if __name__ == "__main__":
    unittest.main();