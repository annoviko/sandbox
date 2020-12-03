import time

from telapi import tas_api
from database import db

api_caller = tas_api(db.phone1.accountId, db.phone1.mailboxId)
session_id, caller_party_id = api_caller.callout(db.phone1.deviceId, db.phone2.number)

callee_party_id = None
for i in range(10):
    parties = api_caller.get_parties(session_id)
    time.sleep(1)

    if len(parties) != 2: continue

    callee_party_id = parties[1]
    break

if callee_party_id is None:
    print("Impossible to obtain callee party ID.")
    exit(-1)

print("Callee party:", session_id, callee_party_id)

time.sleep(1)

api_callee = tas_api(db.phone2.accountId, db.phone2.mailboxId)

input("[ACTION REQUIRED] Drop the call on the phone and press ENTER simultaneously.")

body = api_callee.answer(session_id, callee_party_id, db.phone2.deviceId)
print(body)
