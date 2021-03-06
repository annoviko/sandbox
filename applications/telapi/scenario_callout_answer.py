import time

from telapi import tas_api
from database import db

api_caller = tas_api(db.phone2.accountId, db.phone2.mailboxId)
api_callee = tas_api(db.phone1.accountId, db.phone1.mailboxId)

session_id, caller_party_id = api_caller.callout(db.phone2.deviceId, db.phone1.number, assertion=True)

callee_party_id = api_caller.get_party_by_number(session_id, 2)
if api_callee.get_amount_devices() > 1:
    time.sleep(5)

if callee_party_id is None:
    print("Impossible to obtain callee party ID.")
    exit(-1)

print("Callee party:", session_id, callee_party_id)

time.sleep(1)

print(api_caller.get_session(session_id))

body = api_callee.answer(session_id, callee_party_id, db.phone1.deviceId, assertion=True)
print(body)

print(api_caller.get_session(session_id))
