class http_method:
    HTTP_POST                   = 0x01
    HTTP_DELETE                 = 0x02
    HTTP_GET                    = 0x03


class http_code:
    HTTP_OK = 200
    HTTP_OK_CREATED = 201
    HTTP_OK_DELETED = 202
    HTTP_OK_NO_CONTENT = 204
    HTTP_BAD_REQUEST = 400
    HTTP_FORBIDDEN = 403
    HTTP_NOT_FOUND = 404
    HTTP_METHOD_NOT_SUPPORTED = 405
    HTTP_INTERNAL_SERVER_ERROR = 500
