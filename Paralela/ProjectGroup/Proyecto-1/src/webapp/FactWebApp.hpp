// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo.
// Universidad de Costa Rica

#ifndef FACTWEBAPP_HPP
#define FACTWEBAPP_HPP

#include <vector>

#include "HttpApp.hpp"
#include "AssemblerTest.hpp"
#include "FactAnaUri.hpp"
#include "Solver.hpp"
#include "HTMLMaker.hpp"

using std::vector;

/**
* @brief A web application that calculates prime factors
*/
class FactWebApp : public HttpApp {
  /// Objects of this class cannot be copied
  DISABLE_COPY(FactWebApp);

 public:
  FactAnaUri* anaUri;
  std::vector<Solver*> solvers;
  AssemblerTest* assembler;
  HTMLMaker* htmlMaker;

  /**
   * @brief Constructor
   */
  FactWebApp();

  /**
   * @brief Destructor
   */
  ~FactWebApp();

  /// Called by the web server when the web server is started
  void start() override;

  /**
   * @brief Handle HTTP requests. @see HttpServer::handleHttpRequest()
   *
   * @param httpRequest The HTTP request to be handled
   * @param httpResponse The HTTP response to be generated
   *
   * @return true If this application handled the request, false otherwise
   * and another chained application should handle it
   */
  bool handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse) override;
  /// Called when the web server stops, in order to allow the web application
  /// clean up and finish as well
  void stop() override;

 protected:
  /**
   * @brief Handle HTTP requests. @see HttpServer::handleHttpRequest()
   *
   * @param httpRequest The HTTP request to be handled
   * @param httpResponse The HTTP response to be generated
   *
   * @return true If this application handled the request, false otherwise
   * Sends the homepage as HTTP response
   */
  bool serveHomepage(HttpRequest& httpRequest, HttpResponse& httpResponse);

  /**
   * @brief Handle a HTTP request that starts with "/fact"
   *
   * @param httpRequest The HTTP request to be handled
   * @param httpResponse The HTTP response to be generated
   *
   * @return True if the factorization was handled, false if it must be
   * handled by another application
   */
  // bool serveFactorization(HttpRequest& httpRequest, HttpResponse&
  // httpResponse);

  /**
   * @brief Update the HTTP response page with calculated numbers and factors.
   *
   * @param httpResponse The HTTP response to be updated
   * @param numbers A vector of integers to display
   * @param allFactors A vector of vectors containing prime factors
   */
  void updateFactPage(HttpResponse& httpResponse, vector<int> numbers,
    vector<vector<int>> allFactors);
};

#endif  // FACTWEBAPP_HPP
