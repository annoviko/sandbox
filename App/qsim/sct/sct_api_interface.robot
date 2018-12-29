*** Settings ***

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


Start Play Scenario
    ${scenario id}=   Set Variable   10000002

    ${task id}=        Start Task       ${scenario id}
    ${callback url}=   Wait For Play Request
    Reply Success To Play Request    20000001

    Send Play Complete Response         ${callback url}


Start Play Scenario with Stop Play
    ${scenario id}=   Set Variable   10000003

    ${task id}=        Start Task       ${scenario id}
    ${callback url}=   Wait For Play Request

    ${play id}=   Set Variable   20000002
    Reply Success To Play Request   ${play id}

    Wait For Stop Play Request          ${play id}
    Reply By   204

    Send Play Complete Response         ${callback url}


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
    [Arguments]   ${scenario id}
    ${request body}=   Get File   data/start_task_json
    ${url}=            Get Start Task URL   ${scenario id}

    Set Request Header    Content-Type   application/json-rpc

    Send HTTP Request   POST   ${url}   ${request body}

    ${response status}=   Get Response Status
    ${expected status}=   Convert To Integer   201
    Should Be Equal   ${response status}   ${expected status}

    ${response content}=   Get Response Body
    ${task id}=            Get Json Value   ${response content}   id

    [Return]   ${task id}


Stop Task
    [Arguments]   ${scenario id}   ${task id}
    ${url}=   Get Stop Task URL   ${scenario id}   ${task id}

    Send HTTP Request   DELETE   ${url}

    ${response status}=   Get Response Status
    ${expected status}=   Convert To Integer   204
    Should be Equal   ${response status}   ${expected status}


Wait For Play Request
    Wait For Request
    ${method}=    Get Request Method
    ${body}=      Get Request Body

    Should Be Equal   ${method}   POST

    ${url}=    Get Json Value   ${body}   onPlayed
    [Return]   ${url}


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
    ${body}=   Get File   data/resp_id_json
    ${body}=   Set Json Value   ${body}   id   ${play id}
    Set Reply Header      Content-Type   application/json-rpc
    Reply By   200   ${body}


Send Play Complete Response
    [Arguments]   ${callback url}
    Send HTTP Request   POST   ${callback url}

    ${response status}=   Get Response Status
    ${expected status}=   Convert To Integer   200
    Should Be Equal   ${response status}   ${expected status}


Start HTTP Client and Server
    Initialize Client   192.168.55.77   8000
    Start Server        192.168.55.77   8080
