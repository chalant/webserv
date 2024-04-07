/* the Sessions class coordinates request processing.
 * It utilizes the ClientHandler to communicate with the client,
 * the RequestParser to parse the request, the Router to select the appropriate ARequestHandler,
 * and finally, the ARequestHandler to generate a Response.
 * The generated Response is then sent back to the client by the ClientHandler.*/
 