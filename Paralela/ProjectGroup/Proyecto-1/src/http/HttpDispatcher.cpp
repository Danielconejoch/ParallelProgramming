// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo
// Universidad de Costa Rica

#include "HttpDispatcher.hpp"

int HttpDispatcher::run() {
  this->consumeForever();
  return EXIT_SUCCESS;
}

std::string HttpDispatcher::extractKey(const requestResponse_t& data) const {
  std::string uri = data.httpRequest.getURI();
  std::string key = uri.substr(1, 4);
  return key;
}
