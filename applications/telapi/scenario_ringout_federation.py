from telapi import tas_api
from database import db


api_admin = tas_api(db.phone1.accountId, db.phone1.mailboxId)
session_id = api_admin.ringout('13233450893', '18772063905*101')

print("[ACTION REQUIRED] Press Enter to ANSWER to call.")

api_callee = tas_api(db.phone4.accountId, db.phone4.mailboxId)

gone_session_id = input("Enter session ID of callee:")
print(api_admin.get_session(gone_session_id))

gone_party_id = api_admin.get_parties(gone_session_id)[0]
print(session_id, gone_party_id)

#api_callee = tas_api('400378006008', '400378008008')
#api_callee.answer(session_id, party_id, '400175955008')
