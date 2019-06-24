from telapi import tas_api

from database import db


api_admin = tas_api(db.phone1.accountId, db.phone1.mailboxId)
session_id = api_admin.ringme(db.pstn.number, db.phone1.number)

print("[ACTION REQUIRED] Press Enter to ANSWER to call.")
input()

party_id = api_admin.get_parties(session_id)[1]
print(session_id, party_id)

api_admin.answer(session_id, party_id, db.phone1.deviceId)
