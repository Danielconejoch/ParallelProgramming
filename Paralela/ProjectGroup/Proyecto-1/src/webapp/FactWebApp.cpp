// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo.
// Universidad de Costa Rica

#include <algorithm>
#include <cassert>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <cstdint>      // for uint64_t
#include <thread>

#include "FactWebApp.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"


#include "Factoring.hpp"
#include "Solver.hpp"
#include "AssemblerTest.hpp"


FactWebApp::FactWebApp() {
}

FactWebApp::~FactWebApp() {
}

void FactWebApp::start() {
  // Key
  this->key = "fact";

  // URI analizer
  this->anaUri = new FactAnaUri();
  this->anaUri->createOwnQueue();

  // Sorter
  this->assembler = new AssemblerTest();
  this->assembler->createOwnQueue();

  // HTML creator
  this->htmlMaker = new HTMLMaker();

  /*Set the queues for each instance*/

  // Set the entranceQueue to the consumingQueue of anaUri
  this->entranceQueue = this->anaUri->getConsumingQueue();

  // Set the calculated_numbersQueue to the consumingQueue of assembler
  this->calculated_numbersQueue = this->assembler->getConsumingQueue();

  // Create a new Queue to store assembled calculations
  this->assembled_calculationsQueue = new Queue<std::vector<num_t>>();

  // Set the producingQueue of assembler to the Queue assembled calculations
  this->assembler->setProducingQueue(this->assembled_calculationsQueue);

  // Set the consumingQueue of htmlMaker to the Queue for assembled calculations
  this->htmlMaker->setConsumingQueue(this->assembled_calculationsQueue);

  // Set the total amount of threads equal to the number of cores
  uint64_t thread_count = std::thread::hardware_concurrency();

  // Resize the solvers vector to the specified thread_count
  this->solvers.resize(thread_count);

  // Iterate through the solvers vector
  for (size_t i = 0; i < this->solvers.size(); ++i) {
    // Create a new Solver instance
    this->solvers[i] = new Solver();
    // If it's the first solver (index 0), create its own queue
    if (i == 0) {
      this->solvers[i]->createOwnQueue();
    // For solvers other than the first one, sets the consuming
    // queue to the first solver's queue
    } else {
    // Set the producing queue for each solver to the assembler's consuming
    // queue
      this->solvers[i]->setConsumingQueue(
      this->solvers[0]->getConsumingQueue());
    }
    // Set the producing queue for each solver to the assembler's consuming
    // queue
    this->solvers[i]->setProducingQueue(this->assembler->getConsumingQueue());
  }
  // Set the producing queue for anaUri to the consuming queue of the first
  // solver
  this->anaUri->setProducingQueue(this->solvers[0]->getConsumingQueue());

  // Start FactAnaUri
  this->anaUri->startThread();

  // Start Assembler
  this->assembler->startThread();

  // Start Solvers
  for (size_t i = 0; i < this->solvers.size(); ++i) {
    this->solvers[i]->startThread();
  }

  // Start HTMLMaker
  this->htmlMaker->startThread();

  // stop();
}

void FactWebApp::stop() {
  // Stop FactAnaUri
  this->anaUri->waitToFinish();

  // Stop Assembler
  this->assembler->waitToFinish();

  // Stop Solvers
  for (size_t i = 0; i < this->solvers.size(); ++i) {
    this->solvers[i]->waitToFinish();
  }

  // Stop HTMLMaker
  this->htmlMaker->waitToFinish();
}

bool FactWebApp::handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse) {
  // If the home page was asked
  // "/" Is invisible
  if (httpRequest.getMethod() == "GET" && httpRequest.getURI() == "/fact") {
    return this->serveHomepage(httpRequest, httpResponse);
  }

  // // If the request starts with "fact/" is for this web app
  // if (httpRequest.getURI().rfind("/fact/fact", 0) == 0) {
  //   return this->serveFactorization(httpRequest, httpResponse);
  // }

  // Unrecognized request
  return false;
}

bool FactWebApp::serveHomepage(HttpRequest& httpRequest
  , HttpResponse& httpResponse) {
  // Ignore Request
  (void)httpRequest;
  // Set HTTP response metadata (headers)
  httpResponse.setHeader("Server", "AttoServer v1.1");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");

  // Build the body of the response
  std::string title = "Prime Factorization";
  httpResponse.body() << "<!DOCTYPE html>\n"
    << "<html lang=\"en\">\n"
    << "  <meta charset=\"ascii\"/>\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>body {font-family: monospace}</style>\n"
    << "  <h1>" << title << "</h1>\n"
    << "  <form method=\"get\" action=\"/fact/fact\">\n"
    << "    <label for=\"number\">Number</label>\n"
    << "    <input type=\"text\" name=\"number\" required/>\n"
    << "    <button type=\"submit\">Factorize</button>\n"
    << "  </form>\n"
    << "</html>\n";

  // Send the response to the client (user agent)
  return httpResponse.send();
}

void FactWebApp::updateFactPage(HttpResponse& httpResponse,
  vector<int> numbers, vector<vector<int>> allFactors) {
  httpResponse.setHeader("Server", "AttoServer v1.1");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");
  std::string title = "Prime factorization of ";
  std::string secondTitle = "";

  for (size_t i = 0; i < numbers.size(); i++) {
    secondTitle += "\n\n<h2>";
    secondTitle += std::to_string(numbers[i]);
    secondTitle +="</h2>\n\n";
    title += std::to_string(numbers[i]);
    if (i < numbers.size() - 1) {
      title += ", ";
    }
    secondTitle += std::to_string(numbers[i]);
    secondTitle += " = ";
    for (size_t j = 0; j < allFactors[i].size(); j+=2) {
      if (numbers[i] >= 2 && allFactors[i].size() >= 2) {
        secondTitle += std::to_string(allFactors[i][j]);
        if (allFactors[i][j + 1] > 1) {
          secondTitle += "<sup>";
          secondTitle += std::to_string(allFactors[i][j + 1]);
          secondTitle += "</sup>";
        }
        if (j + 2 < allFactors[i].size()) {
          secondTitle += " x ";
        }
      }
    }
    if (numbers[i] < 2) {
      secondTitle += "NA";
    }
  }
  title += ".";

  httpResponse.body() << "<!DOCTYPE html>\n"
  << "<html lang=\"en\">\n"
  << "  <meta charset=\"ascii\"/>\n"
  << "  <title>" << title << "</title>\n"
  << "  <style>body {font-family: monospace} .err {color: red}</style>\n"
  << "  <h1>" << title << "</h1>\n"
  << secondTitle
  << "  <hr><p><a href=\"/fact\">Back</a></p>\n"
  << "</html>\n";
}
