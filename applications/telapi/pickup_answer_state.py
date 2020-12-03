import time

from telapi import tas_api
from database import db

# ======== call from 102 to 103 ============
print("1. call from 102 to 103.")

api_102 = tas_api(db.phone2.accountId, db.phone2.mailboxId)
api_103 = tas_api(db.phone3.accountId, db.phone3.mailboxId)

session_id_1, caller_party_102_id = api_102.callout(db.phone2.deviceId, db.phone3.number, assertion=True)

callee_party_103_id = api_102.get_party_by_state(session_id_1, 'Proceeding', assertion=True)
api_103.answer(session_id_1, callee_party_103_id, db.phone3.deviceId, assertion=True)

# ======== call from 101 to 104 ============
print("2. call from 101 to 104.")

api_101 = tas_api(db.phone1.accountId, db.phone1.mailboxId)
api_104 = tas_api(db.phone4.accountId, db.phone4.mailboxId)

session_id_2, caller_party_101_id = api_101.callout(db.phone1.deviceId, db.phone4.number, assertion=True)

callee_party_104_id = api_101.get_party_by_state(session_id_2, 'Proceeding', assertion=True)
api_104.answer(session_id_2, callee_party_104_id, db.phone4.deviceId, assertion=True)

# ======== park 102 by 103 ============
print("2. park 102 by 103.")

api_103.transfer_to_park_orbit(callee_party_103_id, session_id_1, db.parkOrbitId)

# ======== pickup 103 by 104 ============
