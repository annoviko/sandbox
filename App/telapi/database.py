class phone:
    def __init__(self, accountId, mailboxId, phoneNumber, deviceId, extension):
        self.accountId = accountId
        self.mailboxId = mailboxId
        self.number = phoneNumber
        self.deviceId = deviceId
        self.extension = extension

class db:
    phone1 = phone('400130737008', '400130737008', '+12127310124', '400018336008', '101')  # Cisco SPA525G
    phone2 = phone('400130737008', '400130739008', '+18084240446', '400018337008', '102')  # Polycom VVX201
    phone3 = phone('400130737008', '400130740008', '+13038060059', '400018338008', '103')  # Polycom VVX411

    phone4 = phone('400131372008', '400131372008', '+12139430595', '400018837008', '101')  # Polycom VVX400

    softphone1 = phone('400130737008', '400130737008', '+12127310124', '400018536008', '101')

    pstn = phone(None, None, '+12223335566', None, None)

    parkOrbitId = '400131171008'
