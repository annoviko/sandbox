"""

DEMO SCENARIO:

1. Make a call from HardPhone to SoftPhone using REST API CallOut. SoftPhone is ringing.
2. Answer on SoftPhone using REST API Answer.

"""

from telapi import tas_api
from database import db

api_caller = tas_api(db.phone2.accountId, db.phone2.mailboxId)
api_callee = tas_api(db.softphone1.accountId, db.softphone1.mailboxId)

session_id, caller_party_id = api_caller.callout(db.phone2.deviceId, db.softphone1.number)

callee_party_id = api_caller.get_party_by_number(session_id, 2)

print("SoftPhone is Ringing after Call-Out. Session Status:")
print(api_caller.get_session(session_id))

if callee_party_id is None:
    print("Impossible to obtain callee party ID.")
    exit(-1)

input("\n\nPress enter to answer to the incoming call on SoftPhone.")

body = api_callee.answer(session_id, callee_party_id, db.softphone1.deviceId)


print("\n\nResponse to Answer command:")
print(body)

input("\n\nPress enter to see session status.")

print("\n\nSession status:")
print(api_caller.get_session(session_id))
