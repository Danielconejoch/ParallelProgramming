// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo
// Universidad de Costa Rica

#ifndef FACTANAURI_HPP
#define FACTANAURI_HPP

#include <regex>
#include <iostream>
#include <string>
#include <vector>

#include "Assembler.hpp"
#include "NumberStruct.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Queue.hpp"
#include "Socket.hpp"
#include "Structs.hpp"


/**
 * @file FactAnaUri.hpp
 * @brief Contains the FactAnaUri class, which consumes and handles incoming HTTP requests.
 */

/**
 * @class FactAnaUri
 * @brief A class that consumes and handles incoming HTTP requests.
 * @param requestResponse_t The type of the request and response objects.
 * @param requestResponse_t The type of the request and response objects.
 */
class FactAnaUri : public Assembler<requestResponse_t, num_t> {
 public:
  int contador;
  int number_order;
  /**
  * @brief Consumes and handles incoming HTTP requests.
  *
  * @param client The socket representing the client connection.
  */
  void consume(requestResponse_t client) override;

  /**
  * @brief Start handling HTTP connections.
  *
  * @return The exit status of the handler.
  */
  int run() override;

  /**
  * @brief Serves the homepage.
  *
  * @param httpResponse The HTTP response object.
  * @return True if the homepage was served successfully, false otherwise.
  */
  bool serveHomepage(HttpResponse& httpResponse);

  /**
  * @brief Analize the Uri and get the numbers to be send to the solvers
  * 
  * @param httpRequest The HTTP request object.
  * @param httpResponse The HTTP response object.
  */
  void uriAnalizer(HttpRequest& httpRequest, HttpResponse& httpResponse);
};

#endif  // FACTANAURI_HPP
