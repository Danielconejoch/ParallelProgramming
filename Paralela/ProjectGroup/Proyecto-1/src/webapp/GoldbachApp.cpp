// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo.
// Universidad de Costa Rica

#include <algorithm>
#include <cassert>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <cstdint>     // for uint64_t
#include <thread>

#include "GoldbachApp.hpp"

#include "goldbach.hpp"


GoldbachApp::GoldbachApp() {
}

GoldbachApp::~GoldbachApp() {
}

void GoldbachApp::start() {
  // key
  this->key = "gold";

  // URI analizer
  this->anaUri = new GoldAnaUri();
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
}

void GoldbachApp::stop() {
  // TODO(you): Stop producers, consumers, assemblers...
}
bool GoldbachApp::handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse) {
  // If the home page was asked
  // "/" Is invisible
  if (httpRequest.getMethod() == "GET" && httpRequest.getURI() == "/gold") {
    return this->serveHomepage(httpRequest, httpResponse);
  }

  // If the request starts with "gold/" is for this web app
  if (httpRequest.getURI().rfind("/gold/gold", 0) == 0) {
    return this->serveGoldbach(httpRequest, httpResponse);
  }

  // Unrecognized request
  return false;
}

bool GoldbachApp::serveHomepage(HttpRequest& httpRequest
  , HttpResponse& httpResponse) {
  // Ignore Request
  (void)httpRequest;

  // Set HTTP response metadata (headers)
  httpResponse.setHeader("Server", "AttoServer v1.1");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");

  // Build the body of the response
  std::string title = "Goldbach Sums";
  httpResponse.body() << "<!DOCTYPE html>\n"
    << "<html lang=\"en\">\n"
    << "  <meta charset=\"ascii\"/>\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>body {font-family: monospace}</style>\n"
    << "  <h1>" << title << "</h1>\n"
    << "  <form method=\"get\" action=\"/gold/gold\">\n"
    << "    <label for=\"number\">Number</label>\n"
    << "    <input type=\"text\" name=\"number\" required/>\n"
    << "    <button type=\"submit\">Goldbach</button>\n"
    << "  </form>\n"
    << "</html>\n";

  // Send the response to the client (user agent)
  return httpResponse.send();
}

void GoldbachApp::updateGoldPage(HttpResponse& httpResponse, vector<int>
 numbers, vector<vector<int>> allSums) {
  std::string title = "Goldbach sums of ";
  std::string secondTitle = "";
  size_t increment;

  for (size_t i = 0; i < numbers.size(); i++) {
    increment = numbers[i] % 2 == 0 ? 2 : 3;
    secondTitle += "\n\n<h2>";
    secondTitle += std::to_string(numbers[i]);
    secondTitle +="</h2>\n\n";
    title += std::to_string(numbers[i]);
    if (i < numbers.size() - 1) {
      title += ", ";
    }
    secondTitle += std::to_string(numbers[i]);
    secondTitle += ": ";
    if (allSums[i].size() >= increment) {
      secondTitle += std::to_string(allSums[i].size()/increment);
      secondTitle += " sums";
      if (numbers[i] >= 0) {
        secondTitle += ": ";
      }
    }
    for (size_t j = 0; j < allSums[i].size(); j+=increment) {
      if (allSums[i].size() >= increment) {
        if (numbers[i] >= 0) {
          secondTitle += std::to_string(allSums[i][j]);
          secondTitle += " + ";
          secondTitle += std::to_string(allSums[i][j + 1]);
          if (increment == 3) {
            secondTitle += " + ";
            secondTitle += std::to_string(allSums[i][j + 2]);
          }
          if (j + increment < allSums[i].size()) {
            secondTitle += ", ";
          }
        }
      }
    }
    if (allSums[i].size() < 2) {
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
  << "  <hr><p><a href=\"/gold\">Back</a></p>\n"
  << "</html>\n";
}

bool GoldbachApp::serveGoldbach(HttpRequest& httpRequest
  , HttpResponse& httpResponse) {
  (void)httpRequest;

  // Set HTTP response metadata (headers)
  httpResponse.setHeader("Server", "AttoServer v1.0");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");

  // If a number was asked in the form "/gold/1223"
  // or "/gold?number=1223"
  // TODO(you): Use arbitrary precision for numbers larger than int64_t
  std::smatch matches;
  std::regex inQuery("^/gold/gold(/|\\?number=)([-\\d(%2C)]+)$");
  std::cout << "GetURI" << httpRequest.getURI() << std::endl;

  // Crea el vector de int goldbachNumbers que vamos a llenar con la entrada de
  // datos
  std::vector<int> goldbachNumbers;
  // Guarda en input el valor retornado de getURI
  std::string input = httpRequest.getURI();
  // Buscamos el '=' en el input recibido del getURI para leer los numeros que
  // vienen despues en el formato numero%2Cnumero%2cnumero...
  size_t findEqual = input.find("=");
  // npos se utiliza para verificar que no se encontro el string buscado,
  // mientras se siga encontrando el valor buscado, se ejecutara la instruccion
  if (findEqual != std::string::npos) {
    std::string numbers = input.substr(findEqual + 1);  // Obtener la parte
    // despues de "="

    // Reemplazamos "%2C" por ','
    size_t position = numbers.find("%2C");  // numbers es el string leido
    // despues del "="
    while (position != std::string::npos) {
        numbers.replace(position, 3, ",");  // Reemplazar "%2C" por ','
        position = numbers.find("%2C", position + 1);  // Buscar la proxima vez
        // que aparezca %2C para cambiarlo
    }

    // Utilizar ',' como condicion en el getline() en std::getline
    std::istringstream condition(numbers);
    std::string numero;
    while (std::getline(condition, numero, ',')) {
      int num = std::stoi(numero);
      goldbachNumbers.push_back(num);
    }
  }

  if (std::regex_search(httpRequest.getURI(), matches, inQuery)) {
    assert(matches.length() >= 3);
    Goldbach gold;
    vector<vector<int>> A = gold.goldbachAllNumbers(goldbachNumbers);
    // Build the body of the response
    this->updateGoldPage(httpResponse, goldbachNumbers, A);
  } else {
    // Build the body for an invalid request
    std::string title = "Invalid request";
    httpResponse.body() << "<!DOCTYPE html>\n"
      << "<html lang=\"en\">\n"
      << "  <meta charset=\"ascii\"/>\n"
      << "  <title>" << title << "</title>\n"
      << "  <style>body {font-family: monospace} .err {color: red}</style>\n"
      << "  <h1 class=\"err\">" << title << "</h1>\n"
      << "  <p>Invalid request for factorization</p>\n"
      << "  <hr><p><a href=\"/gold\">Back</a></p>\n"
      << "</html>\n";
  }

  // Send the response to the client (user agent)
  return httpResponse.send();
}
