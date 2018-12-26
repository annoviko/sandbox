*** Settings ***

Library         OperatingSystem
Library         HttpLibrary.HTTP

Resource        variables.robot

Test Setup      Create HTTP Context     ${QSIM_HOST}


*** Test Cases ***

Start and Stop Task Without Scenario
    ${task id}=   Start Task   sct_10000001
    Stop Task     ${task id}


*** Keywords ***
Get Start Task URI
    [Arguments]   ${scenario id}
    ${uri}=    Catenate   /telephony/v1/account/400130275008/services/queue/   ${scenario id}
    ${uri}=    Catenate   ${uri}   /voice/tasks
    [Return]   ${uri}


Get Stop Task URI
    [Arguments]   ${scenario id}   ${task id}
    ${uri}=    Get Start Task URI   ${scenario id}
    ${uri}=    Catenate   ${uri}    /${task id}
    [Return]   ${uri}


Start Task
    [Arguments]   ${scenario id}
    ${request content}=   Get File   data/start_task_json
    Set Request Body      ${request content}
    Set Request Header    Content-Type   application/json-rpc
    
    Next Request Should Have Status Code   201
    ${uri}=   Get Start Task URI   sct_10000001
    POST      ${uri}

    ${response content}=   Get Response Body
    Should Contain         ${response content}   ${scenario id}

    ${json response}=      Parse Json            ${response content}
    ${task id}=            Get Json Value   ${json response}   /id

    [Return]   ${task id}


Stop Task
    [Arguments]   ${scenario id}   ${task id}
    Next Request Should Have Status Code   204
    ${uri}=   Get Stop Task URI   ${scenario id}   ${task id}
    DELETE    ${uri}
