from telapi import tas_api
from database import db

api_caller = tas_api(db.phone2.accountId, db.phone2.mailboxId)
api_callee = tas_api(db.phone1.accountId, db.phone1.mailboxId)

session_id, caller_party_id = api_caller.callout(db.phone2.deviceId, db.phone1.number)

input("[ACTION REQUIRED] Answer on the ringing phone and press ENTER.")

print(api_caller.get_session(session_id))

body = api_caller.answer(session_id, caller_party_id, db.phone1.deviceId)
