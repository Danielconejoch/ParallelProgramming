// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo.
// Universidad de Costa Rica

#include "HTMLMaker.hpp"

#include <iostream>
void HTMLMaker::createHTML(std::vector<num_t> data, std::vector<int> numbers,
  std::vector<std::vector<int>> data_of_numbers) {
  if (data[0].fact) {
    updateFactPage(data[0].httpResponse, numbers, data_of_numbers);
  }
  if (!data[0].fact) {
    updateGoldPage(data[0].httpResponse, numbers, data_of_numbers);
  }
}

void HTMLMaker::consume(std::vector<num_t> data) {
  std::vector<int> numbers;
  std::vector<std::vector<int>> data_of_numbers;

  for (std::vector<num_t>::size_type i = 0; i < data.size(); ++i) {
    numbers.push_back(data[i].num);
    data_of_numbers.push_back(data[i].num_array);
  }
  createHTML(data, numbers, data_of_numbers);
}

bool HTMLMaker::updateFactPage(HttpResponse& httpResponse,
  std::vector<int> numbers, std::vector<std::vector<int>> allFactors) {
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

    return httpResponse.send();
}

bool HTMLMaker::updateGoldPage(HttpResponse& httpResponse, std::vector<int>
  numbers, std::vector<std::vector<int>> allSums) {
  httpResponse.setHeader("Server", "AttoServer v1.1");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");

  std::string title = "Goldbach sums of ";
  std::string secondTitle = "";
  size_t increment;

  for (size_t i = 0; i < numbers.size(); i++) {
    increment = numbers[i] % 2 == 0 ? 2 : 3;
    secondTitle += "\n<h2>";
    secondTitle += std::to_string(numbers[i]);
    secondTitle +="</h2>\n";
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
  << "<div>" << secondTitle << "</div>"
  << "  <hr><p><a href=\"/gold\">Back</a></p>\n"
  << "</html>\n";

  return httpResponse.send();
}
