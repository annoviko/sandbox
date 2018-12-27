*** Settings ***

Library         OperatingSystem
Library         HttpCtrl.Client

Test Setup      Initialize Client   192.168.55.77   8000


*** Test Cases ***

Start and Stop Task Without Scenario
    ${scenario id}=   Set Variable   10000001

    ${task id}=   Start Task       ${scenario id}
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
