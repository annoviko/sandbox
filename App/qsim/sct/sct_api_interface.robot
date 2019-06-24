*** Settings ***

Library         Collections
Library         OperatingSystem
Library         HttpCtrl.Client
Library         HttpCtrl.Server
Library         HttpCtrl.Json


Test Setup      Start HTTP Client and Server
Test Teardown   Stop Server



*** Test Cases ***

Start and Stop Task Without Scenario
    ${scenario id}=   Set Variable   10000001

    ${task id}=   Start Task       ${scenario id}
    Stop Task     ${scenario id}   ${task id}


Start and Stop Task Continuously
    ${scenario id}=   Set Variable   10000001

    ${task id}=   Start Task       ${scenario id}
    Stop Task     ${scenario id}   ${task id}

    ${task id}=   Start Task       ${scenario id}
    Stop Task     ${scenario id}   ${task id}


Start Several Tasks
    ${scenario id}=   Set Variable   10000001

    ${task id 1}=   Start Task     ${scenario id}   tas_session_1
    ${task id 2}=   Start Task     ${scenario id}   tas_session_2
    ${task id 3}=   Start Task     ${scenario id}   tas_session_3

    Stop Task     ${scenario id}   ${task id 1}
    Stop Task     ${scenario id}   ${task id 2}
    Stop Task     ${scenario id}   ${task id 3}


Start Play Scenario
    ${scenario id}=   Set Variable   10000002
    ${command id}=    Set Variable   20000001

    ${task id}=        Start Task    ${scenario id}
    Wait For Play Request
    Reply Success To Play Request    ${command id}

    Send On Command Update   ${command id}


Start Play Scenario with Stop Play
    ${scenario id}=   Set Variable   10000003
    ${command id}=    Set Variable   20000002

    ${task id}=        Start Task    ${scenario id}
    Wait For Play Request
    Reply Success To Play Request   ${command id}

    Wait For Stop Play Request      ${command id}
    Reply By   204

    Send On Command Update          ${command id}


Start Task With Negative and Positive Responses
    ${scenario id}=   Set Variable   10000004

    ${expected status}=   Convert To Integer   403
    Start Task With Negative Expectation   ${scenario id}   POST   ${expected status}

    ${expected status}=   Convert To Integer   405
    Start Task With Negative Expectation   ${scenario id}   POST   ${expected status}

    ${task id}=   Start Task       ${scenario id}   default_session
    Stop Task     ${scenario id}   ${task id}


Stop Task With Negative and Positive Responses
    ${scenario id}=   Set Variable   10000005
    ${task id}=   Start Task       ${scenario id}

    ${expected status}=   Convert To Integer   404
    Stop Task     ${scenario id}   ${task id}   ${expected status}

    ${expected status}=   Convert To Integer   409
    Stop Task     ${scenario id}   ${task id}   ${expected status}

    ${expected status}=   Convert To Integer   204
    Stop Task     ${scenario id}   ${task id}   ${expected status}


Start Task with Wrong URL
    ${request body}=   Get File   data/start_task.json
    ${url}=            Set Variable   /account/400130275008/services/queue/10000001/voice/tasks

    Set Request Header    Content-Type   application/json-rpc
    Send HTTP Request     POST   ${url}   ${request body}

    ${response status}=   Get Response Status
    ${expected status}=   Convert To Integer   404
    Should Be Equal   ${response status}   ${expected status}


Start Task with Not Implemented Method
    ${expected status}=   Convert To Integer   501
    Start Task With Negative Expectation   10000001   PUT   ${expected status}


Start Task with Not Existed Scenario
    ${expected status}=   Convert To Integer   404
    Start Task With Negative Expectation   10000001_not_existed   POST   ${expected status}


Start Task and Check Custom Response Headers
    ${scenario id}=   Set Variable   10000006

    ${task id}=        Start Task    ${scenario id}
    ${headers}=        Get Response Headers

    Dictionary Should Contain Item   ${headers}   location   127.0.0.1
    Dictionary Should Contain Item   ${headers}   rckey1     RCValue1
    Dictionary Should Contain Item   ${headers}   rckey2     RCValue2

    Stop Task     ${scenario id}   ${task id}


Send Collect with Custom Headers
    ${scenario id}=   Set Variable   10000007
    ${task id}=       Start Task    ${scenario id}

    Wait For Collect Request
    ${headers}=        Get Request Headers

    Dictionary Should Contain Item   ${headers}   origin   192.168.55.77
    Dictionary Should Contain Item   ${headers}   header-key   Header-Value

    ${collect id}=   Set Variable   20000002
    Reply Success To Collect Request   ${collect id}

    Send On Command Update   ${collect id}

    Stop Task     ${scenario id}   ${task id}


Send Get Collect and Check Response
    ${scenario id}=   Set Variable   10000008
    ${task id}=       Start Task    ${scenario id}

    Wait For Collect Request

    ${collect id}=   Set Variable   20000002
    Reply Success To Collect Request   ${collect id}

    Wait For Get Collect Request   ${collect id}
    Reply Success To Get Collect Request   ${collect id}

    Stop Task     ${scenario id}   ${task id}


Receive Forward with Specific PartyId after Other Commands
    ${scenario id}=   Set Variable   10000009
    ${task id}=       Start Task     ${scenario id}

    Wait For Play Request
    ${play id}=        Set Variable   20000002
    Reply Success To Play Request     ${play id}
    Send On Command Update            ${play id}

    Wait For Collect Request
    ${collect id}=     Set Variable    30000003
    Reply Success To Collect Request   ${collect id}

    Wait For Forward Group Request
    Reply Success To Forward Group Request   40000004

    Send On Command Update             ${collect id}

    ${expected party id}=   Set Variable   cs00000000000987654321-3
    ${expected mailbox}=    Set Variable   400131053008
    Wait For Forward to VM Request   ${expected party id}   ${expected mailbox}
    Reply Success To Forward To VM Eequest

    Stop Task     ${scenario id}   ${task id}


Send Patch Forward Group
    ${scenario id}=   Set Variable   10000010
    ${task id}=       Start Task     ${scenario id}

    Wait For Forward Group Request
    Reply Success To Forward Group Request   40000004

    Wait For Patch Forward Group Request   40000004
    Reply By   200

    Stop Task     ${scenario id}   ${task id}


Custom Response to Collect Result
    ${scenario id}=   Set Variable   10000011
    ${task id}=       Start Task    ${scenario id}

    Wait For Collect Request
    ${headers}=        Get Request Headers

    ${collect id}=   Set Variable   20000002
    Reply Success To Collect Request   ${collect id}

    Send On Command Update   ${collect id}  403

    Stop Task     ${scenario id}   ${task id}



*** Keywords ***
Get Start Task URL
    [Arguments]   ${scenario id}
    ${url}=    Catenate   SEPARATOR=   /telephony/v1/account/400130275008/services/queue/   ${scenario id}
    ${url}=    Catenate   SEPARATOR=   ${url}   /voice/tasks
    [Return]   ${url}


Get Stop Task URL
    [Arguments]   ${scenario id}   ${task id}
    ${url}=    Get Start Task URL   ${scenario id}
    ${url}=    Catenate   SEPARATOR=   ${url}    /${task id}
    [Return]   ${url}


Start Task
    [Arguments]   ${scenario id}   ${session id}=default_session_id
    ${request body}=   Get File   data/start_task.json
    ${url}=            Get Start Task URL   ${scenario id}

    ${request body}=   Set Json Value In String   ${request body}   sessionId   ${session id}

    Set Request Header    Content-Type   application/json-rpc

    Send HTTP Request   POST   ${url}   ${request body}

    ${response status}=   Get Response Status
    ${expected status}=   Convert To Integer   201
    Should Be Equal   ${response status}   ${expected status}

    ${response content}=   Get Response Body
    ${task id}=            Get Json Value From String   ${response content}   id

    [Return]   ${task id}


Start Task With Negative Expectation
    [Arguments]   ${scenario id}   ${method}   ${expected status}

    ${request body}=   Get File   data/start_task.json
    ${url}=            Get Start Task URL   ${scenario id}

    Set Request Header    Content-Type   application/json-rpc
    Send HTTP Request     ${method}   ${url}   ${request body}

    ${response status}=   Get Response Status
    Should Be Equal   ${response status}   ${expected status}


Stop Task
    [Arguments]   ${scenario id}   ${task id}   ${expected status}=204
    ${url}=   Get Stop Task URL   ${scenario id}   ${task id}

    Send HTTP Request   DELETE   ${url}

    ${response status}=   Get Response Status
    ${expected status}=   Convert To Integer   ${expected status}
    Should be Equal   ${response status}   ${expected status}


Wait For Play Request
    Wait For Request
    ${method}=    Get Request Method
    ${body}=      Get Request Body

    Should Be Equal   ${method}   POST

    ${request url}=   Get Request URL
    ${ret}=   Should Match Regexp   ${request url}   .*/play


Wait For Stop Play Request
    [Arguments]   ${play id}
    Wait For Request
    ${method}=   Get Request Method

    Should Be Equal   ${method}   DELETE

    ${url}=   Get Request URL
    ${match}   ${group1}=   Should Match Regexp   ${url}   .*/play/(\\S+)
    Should Be Equal   ${group1}   ${play id}


Reply Success To Play Request
    [Arguments]   ${play id}
    ${body}=   Get File   data/resp_id.json
    ${body}=   Set Json Value In String   ${body}   id   ${play id}
    Set Reply Header      Content-Type   application/json-rpc
    Reply By   200   ${body}


Wait For Collect Request
    Wait For Request
    ${method}=    Get Request Method
    ${body}=      Get Request Body

    Should Be Equal   ${method}   POST

    ${request url}=   Get Request URL
    ${ret}=   Should Match Regexp   ${request url}   .*/collect


Reply Success To Collect Request
    [Arguments]   ${collect id}
    ${body}=   Get File   data/resp_id.json
    ${body}=   Set Json Value In String   ${body}   id   ${collect id}
    Set Reply Header      Content-Type   application/json-rpc
    Reply By   200   ${body}


Wait For Get Collect Request
    [Arguments]   ${collect id}
    Wait For Request
    ${method}=    Get Request Method

    Should Be Equal   ${method}   GET

    ${request url}=   Get Request URL
    ${ret}=   Should Match Regexp   ${request url}   .*/collect/${collect id}


Reply Success To Get Collect Request
    [Arguments]   ${collect id}
    ${body}=   Get File   data/resp_id.json
    ${body}=   Set Json Value In String   ${body}   id   ${collect id}
    Set Reply Header      Content-Type   application/json-rpc
    Reply By   200   ${body}


Wait For Forward Group Request
    Wait For Request
    ${method}=    Get Request Method
    ${body}=      Get Request Body

    Should Be Equal   ${method}   POST

    ${request url}=   Get Request URL
    ${ret}=   Should Match Regexp   ${request url}   .*/forward-group


Reply Success To Forward Group Request
    [Arguments]   ${group id}
    ${body}=   Get File   data/resp_forward_group.json
    ${body}=   Set Json Value In String   ${body}   forwardGroup/id   ${group id}
    Set Reply Header      Content-Type   application/json-rpc
    Reply By   200   ${body}


Wait For Patch Forward Group Request
    [Arguments]   ${group id}
    Wait For Request
    ${method}=    Get Request Method

    Should Be Equal   ${method}   PATCH

    ${request url}=   Get Request URL
    ${ret}=   Should Match Regexp   ${request url}   .*/forward-group/*.


Wait For Forward to VM Request
    [Arguments]   ${expected party id}   ${expected mailbox}
    Wait For Request
    ${method}=    Get Request Method
    ${body}=      Get Request Body

    Should Be Equal   ${method}   POST

    ${request url}=   Get Request URL
    ${ret}=   Should Match Regexp   ${request url}   .*/${expected party id}/forward

    ${mailbox}=    Get Json Value From String   ${body}   voicemail
    Should Be Equal   ${mailbox}   ${expected mailbox}


Reply Success To Forward To VM Eequest
    Reply By   200


Send On Command Update
    [Arguments]   ${action id}   ${expected response code}=200
    ${url}=   Set Variable   /telephony/v1/account/1234567890/services/queue/on-command-update

    ${body}=   Get File   data/command_update.json
    ${body}=   Set Json Value In String   ${body}   commandId   ${action id}

    Send HTTP Request   POST   ${url}   ${body}

    ${response status}=   Get Response Status
    ${expected status}=   Convert To Integer   ${expected response code}
    Should Be Equal   ${response status}   ${expected status}


Start HTTP Client and Server
    Initialize Client   127.0.0.1   8000
    Start Server        127.0.0.1   8080
