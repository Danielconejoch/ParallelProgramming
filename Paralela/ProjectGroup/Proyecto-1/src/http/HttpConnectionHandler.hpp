// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo
// Universidad de Costa Rica

#ifndef HTTPCONNECTIONHANDLER_HPP
#define HTTPCONNECTIONHANDLER_HPP

#include <string>
#include <vector>
#include <iostream>

#include "Assembler.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "HttpApp.hpp"
#include "Log.hpp"
#include "NetworkAddress.hpp"
#include "NumberStruct.hpp"
#include "Socket.hpp"
#include "Structs.hpp"


class HttpConnectionHandler : public virtual Assembler<Socket,
requestResponse_t> {
 public:
    /**
     * @brief Consumes and handles incoming HTTP requests.
     *
     * @param client The socket representing the client connection.
     */
    void consume(Socket client) override;

    /**
     * @brief Start handling HTTP connections.
     *
     * @return The exit status of the handler.
     */
    int run() override;

    /**
     * @brief Handles an HTTP request for a resource not found (404).
     *
     * @param httpRequest The HTTP request object.
     * @param httpResponse The HTTP response object.
     * @return true if the response was generated successfully, false otherwise.
     */
    bool serveNotFound(HttpRequest& httpRequest, HttpResponse& httpResponse);
};

#endif
