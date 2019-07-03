"""

DEMO SCENARIO:

1. Make a call from DL2 to DL1 using REST API CallOut. HardPhone is ringing.
2. Answer on DL2 using REST API Answer.
3. Park DL2 using REST API Transfer to Park Orbit. DL2 is parked, DL1 is disconnected.
4. Pickup DL2 using REST API Pickup.

"""

import time

from telapi import tas_api
from database import db


api_caller = tas_api(db.phone2.accountId, db.phone2.mailboxId)
api_callee = tas_api(db.phone1.accountId, db.phone1.mailboxId)
session_id, caller_party_id = api_caller.callout(db.phone2.deviceId, db.phone1.number)

callee_party_id = api_caller.get_party_by_number(session_id, 2)
if api_callee.get_amount_devices() > 1:
    time.sleep(5)

if callee_party_id is None:
    print("Impossible to obtain callee party ID.")
    exit(-1)

input("\n\nPress Enter to answer on DL1 using REST API Answer command.")

api_callee = tas_api(db.phone1.accountId, db.phone1.mailboxId)
api_callee.answer(session_id, callee_party_id, db.phone1.deviceId)

input("\n\nPress Enter to park DL2 using REST API Transfer to ParkOrbit command.")

api_callee.transfer_to_park_orbit(callee_party_id, session_id, db.parkOrbitId)

input("\n\nPress Enter to pickup DL2 using REST API PickUp command.")

parked_party_id = api_caller.get_party_by_state(session_id, 'parked')
if parked_party_id is None:
    print("Impossible to obtain parked party ID.")
    exit(-1)

body = api_callee.pickup(parked_party_id, session_id, db.phone1.deviceId)
print(body)
