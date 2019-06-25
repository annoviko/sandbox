import time

from telapi import tas_api
from database import db

api_callee = tas_api(db.phone1.accountId, db.phone1.mailboxId)
api_caller = tas_api(db.phone2.accountId, db.phone2.mailboxId)
api_forwarded = tas_api(db.phone3.accountId, db.phone3.mailboxId)

session_id, caller_party_id = api_caller.callout(db.phone2.deviceId, db.phone1.number)

callee_party_id = api_caller.get_party_by_state(session_id, 'Proceeding')
if api_callee.get_amount_devices() > 1:
    time.sleep(5)

api_callee.answer(session_id, callee_party_id, db.phone1.deviceId, assertion=True)

time.sleep(1)

api_callee.flip(session_id, callee_party_id, '*3', assertion=True)

time.sleep(1)

#input("Answer and drop on 101. After that press ENTER.")
#print(api_caller.get_session(session_id))

callee_forward_party_id = api_caller.get_party_by_state(session_id, 'Proceeding')
body = api_forwarded.answer(session_id, callee_forward_party_id, db.phone3.deviceId, assertion=True)
print(body)
