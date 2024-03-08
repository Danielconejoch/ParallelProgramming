// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo
// Universidad de Costa Rica

#ifndef HTTPDISPATCHER_HPP
#define HTTPDISPATCHER_HPP

#include <string>
#include <iostream>

#include "Dispatcher.hpp"
#include"HttpRequest.hpp"
#include"HttpResponse.hpp"
#include "Queue.hpp"
#include "Socket.hpp"
#include "Structs.hpp"


/**
 * @brief Implementation of the HttpDispatcher class.
 */
class HttpDispatcher : public Dispatcher<std::string, requestResponse_t> {
  // DISABLE_COPY(HttpDispatcher);

 public:
    using Dispatcher::Dispatcher;

    /**
     * @brief Initiates the dispatcher's execution.
     *
     * @return Returns EXIT_SUCCESS when execution finishes.
     */
    int run();

    /**
     * @brief This method is used to extract the key from a data package, which 
     * will be used to route the package to the appropriate queue.
     *
     * @param data The package from which the key will be extracted.
     * @return The key extracted from the package.
     */
    std::string extractKey(const requestResponse_t& data) const override;
};

#endif  // HTTPDISPATCHER_HPP
