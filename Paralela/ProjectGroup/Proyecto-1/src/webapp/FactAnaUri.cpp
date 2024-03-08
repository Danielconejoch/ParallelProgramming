// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo
// Universidad de Costa Rica


#include "FactAnaUri.hpp"
#include <iostream>

void FactAnaUri::consume(requestResponse_t client) {
  if (client.httpRequest.getMethod() == "GET" && client.httpRequest.getURI() ==
  "/fact") {
    this->serveHomepage(client.httpResponse);
  }

  // If the request starts with "fact/" is for this web app
  if (client.httpRequest.getURI().rfind("/fact/fact", 0) == 0) {
    this->uriAnalizer(client.httpRequest, client.httpResponse);
  }
}

int FactAnaUri::run() {
  this->consumeForever();

  return EXIT_SUCCESS;
}

bool FactAnaUri::serveHomepage(HttpResponse& httpResponse) {
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
    << "    <label for=\"numbers\">Number</label>\n"
    << "    <input type=\"text\" name=\"numbers\" required/>\n"
    << "    <button type=\"submit\">Factorize</button>\n"
    << "  </form>\n"
    << "</html>\n";

  // Send the response to the client (user agent)
  return httpResponse.send();
}

void FactAnaUri::uriAnalizer(HttpRequest& httpRequest, HttpResponse&
httpResponse) {
  // If a number was asked in the form "/fact/1223"
  // or "/fact?number=1223"
  std::smatch matches;
  std::regex inQuery("^/fact/fact(/|\\?number=)([-\\d(%2C)]+)$");

  // Crea el vector de int factNumbers que vamos a llenar con la entrada de
  // datos
  std::vector<int> factNumbers;
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
    size_t* amount_of_numbers = new size_t;
    *amount_of_numbers = 0;
    this->number_order = 0;
    while (std::getline(condition, numero, ',')) {
      int num = std::stoi(numero);
      *amount_of_numbers += 1;
      num_t num_prueba(httpResponse, true, num, this->number_order,
      this->contador, amount_of_numbers);
      produce(num_prueba);
      this->number_order++;
    }
    this->contador++;
  }
}
