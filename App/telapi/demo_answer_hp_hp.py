"""

DEMO SCENARIO:

1. Make a call from HardPhone to HardPhone using REST API CallOut. HardPhone is ringing.
2. Answer on HardPhone using REST API Answer.

"""

import time

from telapi import tas_api
from database import db

api_caller = tas_api(db.phone1.accountId, db.phone1.mailboxId)
session_id, caller_party_id = api_caller.callout(db.phone1.deviceId, db.phone2.number)

callee_party_id = api_caller.get_party_by_state(session_id, 'Proceeding')

print("\n\nSession Status. HardPhone is Ringing.")
print(api_caller.get_session(session_id))

input("\n\nPress Enter to answer on HardPhone using REST API Answer command.")

api_callee = tas_api(db.phone2.accountId, db.phone2.mailboxId)
response = api_callee.answer(session_id, callee_party_id, db.phone2.deviceId)

print("\n\nAnswer response:")

time.sleep(1)
print(api_caller.get_session(session_id))
