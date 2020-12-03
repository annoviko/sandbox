from telapi import tas_api
from database import db


api_admin = tas_api(db.phone1.accountId, db.phone1.mailboxId)
session_id = api_admin.ringout(db.phone1.number, db.phone2.number)
print(session_id)