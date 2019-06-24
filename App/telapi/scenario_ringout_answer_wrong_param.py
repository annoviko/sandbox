from telapi import tas_api
from database import db

api_admin = tas_api(db.phone1.accountId, db.phone1.mailboxId)
session_id = api_admin.ringout(db.phone1.number, db.phone2.extension)

print("[ACTION REQUIRED] Press Enter to ANSWER to call.")
input()

party_id = api_admin.get_parties(session_id)[1]
print(session_id, party_id)

api_callee = tas_api(db.phone2.accountId, db.phone2.mailboxId)
result = api_callee.answer(session_id, party_id, 'wrong_device_id')
print(result)
