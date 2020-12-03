class phone:
    def __init__(self, accountId, mailboxId, phoneNumber, deviceId, extension):
        self.accountId = accountId
        self.mailboxId = mailboxId
        self.number = phoneNumber
        self.deviceId = deviceId
        self.extension = extension

class db:
    phone1 = phone('400130051008', '400130051008', '+12405810057', '400017593008', '101')  # Cisco SPA525G
    phone2 = phone('400130051008', '400130053008', '+19282840019', '400017614008', '102')  # Polycom VVX201
    phone3 = phone('400130051008', '400130054008', '+14155640075', '400017653008', '103')  # Polycom VVX411

    phone4 = phone('400130051008', '400130111008', '+19078910042', '400017693008', '104')  # Polycom VVX400

    softphone1 = phone('400130051008', '400130051008', '+12405810057', '', '101')

    pstn = phone(None, None, '+12223335566', None, None)

    parkOrbitId = '400130091008'
