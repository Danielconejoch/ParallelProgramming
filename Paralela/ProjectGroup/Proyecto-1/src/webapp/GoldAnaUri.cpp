// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo
// Universidad de Costa Rica


#include "GoldAnaUri.hpp"
#include <iostream>

void GoldAnaUri::consume(requestResponse_t client) {
    if (client.httpRequest.getMethod() == "GET" && client.httpRequest.getURI()
    == "/gold") {
    this->serveHomepage(client.httpRequest, client.httpResponse);
  }

  // If the request starts with "fact/" is for this web app
  if (client.httpRequest.getURI().rfind("/gold/gold", 0) == 0) {
    this->uriAnalizer(client.httpRequest, client.httpResponse);
  }
}

int GoldAnaUri::run() {
  this->consumeForever();
  return EXIT_SUCCESS;
}

bool GoldAnaUri::serveHomepage(HttpRequest& httpRequest
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
    << "    <label for=\"numbers\">Number</label>\n"
    << "    <input type=\"text\" name=\"numbers\" required/>\n"
    << "    <button type=\"submit\">Goldbach</button>\n"
    << "  </form>\n"
    << "</html>\n";

  // Send the response to the client (user agent)
  return httpResponse.send();
}

bool GoldAnaUri::uriAnalizer(HttpRequest& httpRequest
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
    size_t* amount_of_numbers = new size_t;
    *amount_of_numbers = 0;
    while (std::getline(condition, numero, ',')) {
      int num = std::stoi(numero);
      *amount_of_numbers += 1;
      num_t num_prueba(httpResponse, false, num, this->number_order,
      this->contador, amount_of_numbers);
      produce(num_prueba);
      this->number_order++;
    }
    this->contador++;
    this->number_order = 0;
  } else {
    // Build the body for an invalid request
    std::string title = "Invalid request";
    httpResponse.body() << "<!DOCTYPE html>\n"
      << "<html lang=\"en\">\n"
      << "  <meta charset=\"ascii\"/>\n"
      << "  <title>" << title << "</title>\n"
      << "  <style>body {font-family: monospace} .err {color: red}</style>\n"
      << "  <h1 class=\"err\">" << title << "</h1>\n"
      << "  <p>Invalid request for goldbach sums</p>\n"
      << "  <hr><p><a href=\"/gold\">Back</a></p>\n"
      << "</html>\n";
  }

  // Send the response to the client (user agent)
  return httpResponse.send();
}
