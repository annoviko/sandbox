import http
import http.client
import json
import time

class tas_api:
    def __init__(self, accountId, extensionId, tas_address='192.168.55.77'):
        self.__accountId = accountId
        self.__extensionId = extensionId

        self.__headers = {'RCExtensionId': self.__extensionId,
                          'RCAccountId': self.__accountId,
                          'RCBrandId': 1210,
                          'RCClientRef': '123_DEBUG_321'}

        self.__tas_address = tas_address #'sky01-t02-tas01'

        self.__rdg_address = 'mes01-t01-rdg01'
        self.__rdg_port = 8888


    def callout(self, callerDeviceId, callee, assertion=False):
        content = {
            'from': {
                'deviceId': callerDeviceId
            },
            'to': {
                'phoneNumber': callee
            }
        }

        url = '/telephony/v1/account/%s/call-out' % self.__accountId

        headers = dict(self.__headers)
        headers['Content-Type'] = 'application/json'

        status, body = http_client(self.__tas_address, '8080').send('POST', url, json.dumps(content), headers)

        if status != 201:
            print("ERROR: CallOut was not successful (status '%s')" % str(status))
            if assertion:
                exit(-1)

            return None

        reply = json.loads(body)
        return reply['session']['id'], reply['session']['parties'][0]['id']


    def ringout(self, caller, callee):
        content = {'fromNumber': caller,
                   'toNumber': callee,
                   'callScreening': False,
                   'countryId': 1,
                   'type': 'RingOut'}

        url = '/telephony/v1/account/%s/ring-out' % self.__accountId

        headers = dict(self.__headers)
        headers['Content-Type'] = 'application/json'

        status, body = http_client(self.__tas_address, '8080').send('POST', url, json.dumps(content), headers)

        if status != 201:
            print("ERROR: RingOut was not successful (status '%s')" % str(status))
            return None

        reply = json.loads(body)
        return reply['session']['id']


    def ringme(self, caller, callee):
        content = {'fromNumber': caller,
                   'toNumber': callee}

        url = '/telephony/v1/account/%s/ring-me' % self.__accountId
        headers = dict(self.__headers)
        headers['Content-Type'] = 'application/json'

        status, body = http_client(self.__tas_address, '8080').send('POST', url, json.dumps(content), headers)

        if status != 201:
            print("ERROR: RingOut was not successful (status '%s')" % str(status))
            return None

        reply = json.loads(body)
        return reply['session']['id']


    def answer(self, sessionId, partyId, deviceId, assertion=False):
        content = {'deviceId': deviceId}
        url = '/telephony/v1/account/%s/sessions/%s/parties/%s/answer' % (self.__accountId, sessionId, partyId)

        headers = dict(self.__headers)
        headers['Content-Type'] = 'application/json'

        status, body = http_client(self.__tas_address, '8080').send('POST', url, json.dumps(content), headers)

        if status != 200:
            print("ERROR: Answer was not successful (status '%s')" % str(status))

        return body


    def forward_to_number(self, sessionId, partyId, phoneNumber, assertion=False):
        content = {'phoneNumber': phoneNumber}
        url = '/telephony/v1/account/%s/sessions/%s/parties/%s/forward' % (self.__accountId, sessionId, partyId)

        headers = dict(self.__headers)
        headers['Content-Type'] = 'application/json'

        status, body = http_client(self.__tas_address, '8080').send('POST', url, json.dumps(content), headers)

        if status != 200:
            print("ERROR: Forward was not successful (status '%s')" % str(status))
            if assertion:
                exit(-1)

        return body


    def supervise(self, sessionId, supervisorDeviceId, agentExtensionNumber):
        content = {'deviceId': supervisorDeviceId,
                   'extensionNumber': agentExtensionNumber,
                   'mode': 'Listen'}

        url = '/telephony/v1/account/%s/sessions/%s/supervise' % (self.__accountId, sessionId)

        headers = dict(self.__headers)
        headers['Content-Type'] = 'application/json'

        status, body = http_client(self.__tas_address, '8080').send('POST', url, json.dumps(content), headers)

        if status != 201:
            print("ERROR: Supervise was not successful (status '%s')" % str(status))
            return None

        return body


    def flip(self, sessionId, partyId, callFlipId, assertion=False):
        content = {'callFlipId': callFlipId}

        url = '/telephony/v1/account/%s/sessions/%s/parties/%s/flip' % (self.__accountId, sessionId, partyId)

        headers = dict(self.__headers)
        headers['Content-Type'] = 'application/json'

        status, body = http_client(self.__tas_address, '8080').send('POST', url, json.dumps(content), headers)

        if status != 200:
            print("ERROR: Flip was not successful (status '%s')" % str(status))
            if assertion:
                exit(-1)

            return None

        return body


    def pickup(self, partyId, sessionId, deviceId):
        content = {'deviceId': deviceId}

        url = '/telephony/v1/account/%s/sessions/%s/parties/%s/pickup' % (self.__accountId, sessionId, partyId)

        headers = dict(self.__headers)
        headers['Content-Type'] = 'application/json'

        status, body = http_client(self.__tas_address, '8080').send('POST', url, json.dumps(content), headers)

        if status != 201:
            print("ERROR: Pickup was not successful (status '%s')" % str(status))
            return None

        return body


    def transfer_to_park_orbit(self, partyId, sessionId, parkOrbitId):
        content = {'parkOrbit': parkOrbitId}

        url = '/telephony/v1/account/%s/sessions/%s/parties/%s/transfer' % (self.__accountId, sessionId, partyId)

        headers = dict(self.__headers)
        headers['Content-Type'] = 'application/json'

        status, body = http_client(self.__tas_address, '8080').send('POST', url, json.dumps(content), headers)

        if status != 200:
            print("ERROR: Transfer to Park Orbit was not successful (status '%s')" % str(status))
            return None

        return body


    def transfer_to_phone_number(self, partyId, sessionId, phoneNumber):
        content = {'phoneNumber': phoneNumber}

        url = '/telephony/v1/account/%s/sessions/%s/parties/%s/transfer' % (self.__accountId, sessionId, partyId)

        headers = dict(self.__headers)
        headers['Content-Type'] = 'application/json'

        status, body = http_client(self.__tas_address, '8080').send('POST', url, json.dumps(content), headers)

        if status != 200:
            print("ERROR: Transfer to Phone Number was not successful (status '%s')" % str(status))
            return None

        return body


    def get_amount_devices(self):
        url = '/regs/mailboxid/%s' % self.__extensionId
        headers = dict(self.__headers)

        status, body = http_client(self.__rdg_address, str(self.__rdg_port)).send('GET', url, None, headers)

        if status != 200:
            print("ERROR: Impossible to obtain amount of registered devices for mailbox '%s'" % self.__extensionId)
            return -1

        reply = json.loads(body)
        return len(reply['registrations'])


    def get_session(self, session_id):
        url = '/telephony/v1/account/%s/sessions/%s' % (self.__accountId, session_id)
        headers = dict(self.__headers)

        status, body = http_client(self.__tas_address, '8080').send('GET', url, None, headers)
        if status != 200:
            print("ERROR: Get Session Status was not successful (status '%s')" % str(status))
            return None

        reply = json.loads(body)
        return json.dumps(reply, indent=3, sort_keys=True)


    def get_full_described_parties(self, session_id):
        url = '/telephony/v1/account/%s/sessions/%s' % (self.__accountId, session_id)
        headers = dict(self.__headers)

        status, body = http_client(self.__tas_address, '8080').send('GET', url, None, headers)
        if status != 200:
            print("ERROR: Get Session Status was not successful (status '%s')" % str(status))
            return None

        reply = json.loads(body)
        return reply['parties']


    def get_parties(self, session_id):
        result = []
        reply = self.get_full_described_parties(session_id)
        for party in reply:
            result.append(party['id'])

        return result


    def get_party_by_number(self, session_id, party_number, attempts=10, assertion=False):
        for i in range(attempts):
            parties = self.get_parties(session_id)
            time.sleep(1)

            if len(parties) != party_number:
                continue

            return parties[party_number - 1]

        if assertion:
            exit(-1)

        return None


    def get_party_by_state(self, session_id, state, attempts=10, assertion=False):
        for i in range(attempts):
            parties = self.get_full_described_parties(session_id)

            for party in parties:
                party_state = str(party['status']['code']).lower()
                if party_state == state.lower():
                    return party['id']

            time.sleep(1)

        if assertion is True:
            exit(-1)

        return None



class http_client:
    def __init__(self, host, port):
        endpoint = "%s:%s" % (host, port)
        self.__connection = http.client.HTTPConnection(endpoint)


    def send(self, method, url, content, headers):
        self.__connection.request(method, url, content, headers)
        response = self.__connection.getresponse()

        body = response.read().decode('utf-8')
        status = response.status

        return status, body
