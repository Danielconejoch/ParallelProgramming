// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo
// Universidad de Costa Rica

#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <inttypes.h>
#include <vector>
#include <iostream>
#include <string>

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Socket.hpp"

/**
 * @file Structs.hpp
 * @brief Contains the definition of the requestResponse_t struct.
 */

/**
 * @struct requestResponse_t
 * @brief Struct that contains an HTTP request, an HTTP response and a key.
 */
typedef struct requestResponse {
  HttpRequest httpRequest; /**< The HTTP request. */
  HttpResponse httpResponse; /**< The HTTP response. */
  std::string key; /**< The key. */

  public:
    /**
     * @brief Constructor for the requestResponse_t struct.
     * @param request The HTTP request.
     * @param response The HTTP response.
     */
    requestResponse(HttpRequest request = HttpRequest(Socket())
      , HttpResponse response = HttpResponse(Socket()))
      : httpRequest(request)
      , httpResponse(response) {}

    /**
     * @brief Overloaded operator for the requestResponse_t struct.
     * @param other The other requestResponse_t struct to compare to.
     * @return True if the two structs are equal, false otherwise.
     */
    inline bool operator == (const requestResponse& other) const {
      return this->httpRequest == other.httpRequest
      && this->httpResponse == other.httpResponse;
    }
} requestResponse_t;

#endif  // STRUCTS_HPP
