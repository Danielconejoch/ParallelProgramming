// Copyright 2023 Daniel Conejo, Dariel Alvarez, Marco Angulo, Werner Naranjo.
// Universidad de Costa Rica

#ifndef GOLDBACHAPP_HPP
#define GOLDBACHAPP_HPP

#include <vector>

#include "HttpApp.hpp"
#include "GoldAnaUri.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "AssemblerTest.hpp"
#include "Solver.hpp"
#include "HTMLMaker.hpp"

using std::vector;

/**
* @brief A web application that calculates prime factors
*/
class GoldbachApp : public HttpApp {
  /// Objects of this class cannot be copied
  DISABLE_COPY(GoldbachApp);

 public:
  GoldAnaUri* anaUri;
  std::vector<Solver*> solvers;
  AssemblerTest* assembler;
  HTMLMaker* htmlMaker;
  /**
   * @brief Constructor
   */
  GoldbachApp();

  /**
   * @brief Destructor
   */
  ~GoldbachApp();

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
   * @brief Handle a HTTP request that starts with "/gold".
   *
   * @param httpRequest The HTTP request to be handled
   * @param httpResponse The HTTP response to be generated
   *
   * @return true if the Goldbach sum was handled, false if it must be
   * handled by another application
   */
  bool serveGoldbach(HttpRequest& httpRequest, HttpResponse& httpResponse);

  /**
   * @brief Update the HTTP response page with calculated numbers and goldbach sums.
   *
   * @param httpResponse The HTTP response to be updated
   * @param numbers A vector of integers to display
   * @param allSums A vector of vectors containing Goldbach sums
   */
  void updateGoldPage(HttpResponse& httpResponse, vector<int> numbers,
    vector<vector<int>> allSums);
};

#endif  // GOLDBACHAPP_HPP
