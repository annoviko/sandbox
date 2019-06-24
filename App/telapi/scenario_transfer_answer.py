import time

from telapi import tas_api
from database import db

api_caller = tas_api(db.phone2.accountId, db.phone2.mailboxId)
session_id, caller_party_id = api_caller.callout(db.phone2.deviceId, db.phone1.number)

callee_party_id = api_caller.get_party_by_number(session_id, 2)
if callee_party_id is None:
    print("Impossible to obtain callee party ID.")
    exit(-1)

print("Callee party:", session_id, callee_party_id)

time.sleep(1)

api_callee = tas_api(db.phone1.accountId, db.phone1.mailboxId)
api_callee.answer(session_id, callee_party_id, db.phone1.deviceId)

print("Answered on the phone.")

time.sleep(1)

api_callee.transfer_to_phone_number(callee_party_id, session_id, db.phone3.number)

print("Transferred to phone number.")

time.sleep(1)

trasfer_callee_party_id = api_caller.get_party_by_number(session_id, 3)
if trasfer_callee_party_id is None:
    print("Impossible to obtain callee party ID.")
    exit(-1)

api_transfer_callee = tas_api(db.phone3.accountId, db.phone3.mailboxId)
api_transfer_callee.answer(session_id, trasfer_callee_party_id, db.phone3.deviceId)
