import time

from telapi import tas_api
from database import db

api_caller = tas_api(db.phone2.accountId, db.phone2.mailboxId)
api_callee = tas_api(db.phone1.accountId, db.phone1.mailboxId)
api_wrong_callee = tas_api(db.phone4.accountId, db.phone4.mailboxId)

session_id, caller_party_id = api_caller.callout(db.phone2.deviceId, db.phone1.number)

callee_party_id = api_caller.get_party_by_number(session_id, 2)
if api_callee.get_amount_devices() > 1:
    time.sleep(5)

print(api_caller.get_session(session_id))

if callee_party_id is None:
    print("Impossible to obtain callee party ID.")
    exit(-1)

print("Callee party:", session_id, callee_party_id)

time.sleep(1)

print(api_caller.get_session(session_id))

body = api_wrong_callee.answer(session_id, callee_party_id, db.phone4.deviceId)
print(body)

