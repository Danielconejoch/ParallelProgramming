// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include <unistd.h>

#include <cassert>
#include <stdexcept>
#include <string>
#include <iostream>

#include "HttpApp.hpp"
#include "HttpServer.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Log.hpp"
#include "NetworkAddress.hpp"
#include "Socket.hpp"

const char* const usage =
  "Usage: webserv [port] [handlers]\n"
  "\n"
  "  port        Network port to listen incoming HTTP requests, default "
    DEFAULT_PORT "\n"
  "  handlers     Number of connection handler theads\n";

HttpServer::HttpServer() {
  // Creates the queue of sockets
  this->clients = new Queue<Socket>();
}
  /// Destructor
HttpServer::~HttpServer() {
  delete this->clients;
}

void HttpServer::listenForever(const char* port) {
  return TcpServer::listenForever(port);
}

void HttpServer::chainWebApp(HttpApp* application) {
  assert(application);
  this->applications.push_back(application);
}

int HttpServer::run(int argc, char* argv[]) {
  bool stopApps = false;

  try {
    if (this->analyzeArguments(argc, argv)) {
      // Start the log service
      Log::getInstance().start();

      // Start all web applications
      this->startApps();
      stopApps = true;

      // Create instances

      // Array size = handlersCount
      this->connectionHandlers.resize(this->handlersCount);

      // Create connection handlers
      for (size_t i = 0; i < this->handlersCount; i++) {
        this->connectionHandlers[i] = new HttpConnectionHandler();
      }

      // Create dispatcher
      this->dispatcher = new HttpDispatcher();

      // Create consuming queue for dispatcher and producing queue for
      // connection handlers
      this->dispatcher->createOwnQueue();
      for (size_t i = 0; i < this->handlersCount; i++) {
        // consuming queue for handlers is the queue of sockets
        this->connectionHandlers[i]->setConsumingQueue(this->clients);
        // producing queue is the queue of the dispatcher
        this->connectionHandlers[i]->setProducingQueue(
          this->dispatcher->getConsumingQueue());
      }
      Queue<requestResponse_t>* enQueue = new Queue<requestResponse_t>();
      this->dispatcher->registerRedirect(
          "favi", enQueue);
      for (size_t i = 0; i < this->applications.size(); i++) {
        this->dispatcher->registerRedirect(
          this->applications[i]->key, this->applications[i]->entranceQueue);
      }
      // Start threads
      for (size_t i = 0; i < this->handlersCount; i++) {
        this->connectionHandlers[i]->startThread();
      }
      this->dispatcher->startThread();
      // Start waiting for connections
      // TODO(you): Log the main thread id
      this->listenForConnections(this->port);
      const NetworkAddress& address = this->getNetworkAddress();
      Log::append(Log::INFO, "webserver", "Listening on " + address.getIP()
        + " port " + std::to_string(address.getPort()));

      // Accept all client connections. The main process will get blocked
      // running this method and will not return. When HttpServer::stop() is
      // called from another execution thread, an exception will be launched
      // that stops the acceptAllConnections() invocation and enters in the
      // catch below. Then, the main thread can continue stopping apps,
      /// finishing the server and any further cleaning it requires.

      this->acceptAllConnections();
    }
  } catch (const std::runtime_error& error) {
    std::cerr << "error: " << error.what() << std::endl;
  }

  // If applications were started
  if (stopApps) {
    this->stopApps();
  }

  // Stop the log service
  Log::getInstance().stop();
  return EXIT_SUCCESS;
}

void HttpServer::startApps() {
  for (size_t index = 0; index < this->applications.size(); ++index) {
    this->applications[index]->start();
  }
}

void HttpServer::stopApps() {
  // Stop web applications. Give them an opportunity to clean up
  for (size_t index = 0; index < this->applications.size(); ++index) {
    this->applications[index]->stop();
  }
}

void HttpServer::stop() {
  // Stop listening for incoming client connection requests. When stopListing()
  // method is called -maybe by a secondary thread-, the web server -running
  // by the main thread- will stop executing the acceptAllConnections() method.
  this->stopListening();

  // Stop connection handler threads
  for (size_t i = 0; i < this->connectionHandlers.size(); ++i) {
    this->connectionHandlers[i]->waitToFinish();
  }
  // Stop dispatcher threads
  this->dispatcher->waitToFinish();

  // Free Memory
  for (size_t i = 0; i < this->connectionHandlers.size(); ++i) {
    delete(this->connectionHandlers[i]);
  }
  delete(this->clients);
}

bool HttpServer::analyzeArguments(int argc, char* argv[]) {
  // Traverse all arguments
  for (int index = 1; index < argc; ++index) {
    const std::string argument = argv[index];
    if (argument.find("help") != std::string::npos) {
      std::cout << usage;
      return false;
    }
  }

  if (argc >= 2) {
    this->port = argv[1];
  }
  return true;
}

void HttpServer::handleClientConnection(Socket& client) {
  this->clients->enqueue(client);
}
