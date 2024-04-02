/*WORKFLOW: 

'At launch, Configuration' reads the config file onto itself and is used to 
instantiate 'Server' and 'Router'.
'Server' sets up a tcp connection and waits. When contacted he puts out a 
'ClientHandler' to communicate with the client. 
The 'ClientHandler' gets the raw request off the client 
and the 'RequestParser' parses this raw request into a 'Request'.
Based on info inside the 'Request', the 'Router' appoints the 
required 'RequestHandler' who then takes the 'Request' and produces a 
'Response'. This 'Response' is then used by the 'ClientHandler' to 
send back a response to the client.*/

int main(int argc, char **argv){
    /*'Configuration': Verifies, reads, parses and stores the config 
    file data. This data is used to initiate the 'Server' and the 'Router'*/
    Configuration configuration(argc, argv);

    /*'Server': Sets up tcp connectivity, produces a 'ClientHandler' 
    when contacted by a client*/
    Server server(configuration);

    /*Router: Selects the right 'RequestHandler' based on URI (etc.) 
    in the 'Request' (each locationblock in the conf file corresponds 
    to a 'RequestHandler' mapping, ie the Router selects the correct 
    locationblock)*/
    Router router(configuration);

    /*'ClientHandler': Reads the incoming data, and puts out a string 
    containing the raw request. Later on is given a 'Response' which 
    it uses to send back a response to the client*/ 
    ClientHandler clientHandler;

    /*RequestParser: Produces a 'Request'. Input is a string containing 
    the raw request*/
    RequestParser requestParser;

    /*ARequestHandler: Produces a 'Response'. Input is a 'Request',
    this class is the base class of:
        -StaticFileRequestHandler
        -UploadRequestHandler
        -GetMethodRequestHandler
        -PostMethodRequestHandler
        -DeleteMethodRequestHandler
        -CGIRequestHandler
        -ErrorPageRequestHandler
    'Router selects the appropriate 'ARequestHandler'*/
    ARequestHandler * reguestHandler;

    /*Request: Contains the parsed request*/
    Request request;

    /*Response: Contains the elements of the response*/
    Response response;

    while (true) {
        //'Server' is contacted, he produces a 'ClientHandler'
        clientHandler = server.accept();

        //'Parser' produces a 'Request' from the a raw request string 
        request = requestParser.parse(clientHandler.readRequest());

        //'Router' selects the right 'RequestHandler' for the job
        requestHandler = router.route(request);

        //'ARequesthandler' produces the 'Response'
        response = requestHandler.respond(request);

        //'ClientHandler' sends out a response to the client
        clientHandler.sendResponse(response);
    }
}
