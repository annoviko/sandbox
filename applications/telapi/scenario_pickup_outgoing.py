import time

from telapi import tas_api
from database import db

api_caller = tas_api(db.phone1.accountId, db.phone1.mailboxId)
api_callee = tas_api(db.phone2.accountId, db.phone2.mailboxId)
session_id, caller_party_id = api_caller.callout(db.phone1.deviceId, db.phone2.number)

callee_party_id = api_caller.get_party_by_number(session_id, 2)
if callee_party_id is None:
    print("Impossible to obtain callee party ID.")
    exit(-1)

if api_callee.get_amount_devices() > 1:
    time.sleep(5)

api_callee.answer(session_id, callee_party_id, db.phone2.deviceId)

time.sleep(2)

api_caller.transfer_to_park_orbit(caller_party_id, session_id, db.parkOrbitId)
print(api_caller.get_session(session_id))

time.sleep(2)

body = api_caller.pickup(callee_party_id, session_id, db.phone1.deviceId)
print(body)
