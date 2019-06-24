import time

from telapi import tas_api
from database import db

api_caller = tas_api(db.phone1.accountId, db.phone1.mailboxId)
api_callee = tas_api(db.phone2.accountId, db.phone2.mailboxId)
api_callee_forward = tas_api(db.phone3.accountId, db.phone3.mailboxId)

session_id, caller_party_id = api_caller.callout(db.phone1.deviceId, db.phone2.number)

callee_party_id = api_caller.get_party_by_number(session_id, 2, assertion=True)
if api_callee.get_amount_devices() > 1:
    time.sleep(5)

time.sleep(1)

api_callee.forward_to_number(session_id, callee_party_id, db.phone3.number, assertion=True)

time.sleep(1)

print(api_caller.get_session(session_id))

callee_forward_party_id = api_caller.get_party_by_state(session_id, 'Proceeding')
body = api_callee_forward.answer(session_id, callee_forward_party_id, db.phone3.deviceId, assertion=True)
