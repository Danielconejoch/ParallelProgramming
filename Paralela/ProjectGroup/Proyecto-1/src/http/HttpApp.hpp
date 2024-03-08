// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef HTTPAPP_H
#define HTTPAPP_H

#include <string>
#include <vector>

#include "common.hpp"
#include "Queue.hpp"
#include "Structs.hpp"
#include "NumberStruct.hpp"
class HttpRequest;
class HttpResponse;
/**
@brief Base class for all web applications that can be registered with the
web server.
*/
class HttpApp {
  /// Web application objects are usually complex. This base class does not
  /// require child classes to allow copying
  DISABLE_COPY(HttpApp);

 public:
  // Como las apps heredan de httpApp entonces ambas van a tener un key
  // y una cola por defecto
  std::string key;
  // Queue of requestResponse_t structs
  Queue<requestResponse_t>* entranceQueue;
  // Queue of num_t structs
  Queue<num_t>* assembled_numberQueue;
  // Queue of num_t structs
  Queue<num_t>* calculated_numbersQueue;
  // Queue of std::vector<num_t> structs
  Queue<std::vector<num_t>>* assembled_calculationsQueue;
  /// Constructor
  HttpApp() = default;
  /// Destructor
  ~HttpApp() = default;
  /// Called by the web server when the web server is started
  virtual void start();
  /// Handle HTTP requests. @see HttpServer::handleHttpRequest()
  /// @return true If this application handled the request, false otherwise
  /// and another chained application should handle it
  virtual bool handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse) = 0;
  /// Called when the web server stops, in order to allow the web application
  /// clean up and finish as well
  virtual void stop();
};

#endif  // HTTPAPP_H
