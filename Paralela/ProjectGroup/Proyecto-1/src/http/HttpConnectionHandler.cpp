// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo
// Universidad de Costa Rica

#include "HttpConnectionHandler.hpp"

void HttpConnectionHandler::consume(Socket client) {
    while (true) {
    // Create an object that parses the HTTP request from the socket
    HttpRequest httpRequest(client);

    // If the request is not valid or an error happened
    if (!httpRequest.parse()) {
      // Non-valid requests are normal after a previous valid request. Do not
      // close the connection yet, because the valid request may take time to
      // be processed. Just stop waiting for more requests
      break;
    }

    // A complete HTTP client request was received. Create an object for the
    // server responds to that client's request
    HttpResponse httpResponse(client);

    // creates the struct that has the request and the response
    requestResponse_t package(httpRequest, httpResponse);

    // If subclass did not handle the request or the client used HTTP/1.0
    if (httpRequest.getHttpVersion() == "HTTP/1.0") {
      // The socket will not be more used, close the connection
      client.close();
      break;
    }
    // produces a package with the request and response
    produce(package);
  }
}

int HttpConnectionHandler::run() {
  this->consumeForever();
  return EXIT_SUCCESS;
}

bool HttpConnectionHandler::serveNotFound(HttpRequest& httpRequest
  , HttpResponse& httpResponse) {
  (void)httpRequest;

  // Set HTTP response metadata (headers)
  httpResponse.setStatusCode(404);
  httpResponse.setHeader("Server", "AttoServer v1.0");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");

  // Build the body of the response
  std::string title = "Not found";
  httpResponse.body() << "<!DOCTYPE html>\n"
    << "<html lang=\"en\">\n"
    << "  <meta charset=\"ascii\"/>\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>body {font-family: monospace} h1 {color: red}</style>\n"
    << "  <h1>" << title << "</h1>\n"
    << "  <p>The requested resouce was not found on this server.</p>\n"
    << "  <hr><p><a href=\"/\">Homepage</a></p>\n"
    << "</html>\n";

  // Send the response to the client (user agent)
  return httpResponse.send();
}
