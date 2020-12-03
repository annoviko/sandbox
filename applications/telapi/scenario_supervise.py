import time

from telapi import tas_api


api_caller = tas_api('400378006008', '400378008008')
session_id, caller_party_id = api_caller.callout('400175955008', '+15012091118')

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

api_callee = tas_api('400378006008', '400378009008')
api_callee.answer(session_id, callee_party_id, '400175956008')

time.sleep(2)

api_supervisor = tas_api('400378006008', '400378006008')
api_supervisor.supervise(session_id, '', '102')
