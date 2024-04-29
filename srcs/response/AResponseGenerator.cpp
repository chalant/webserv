/*AResponseGenerator: Produces a 'Response'. Input is a 'IRequest',
this class is the base class of:
    -'StaticFileResponseGenerator'
    -'UploadResponseGenerator'
    -'GetMethodResponseGenerator'
    -'PostMethodResponseGenerator'
    -'DeleteMethodResponseGenerator'
    -'CGIResponseGenerator'
    -'ErrorPageResponseGenerator'
'Router' selects the appropriate 'AResponseGenerator'*/