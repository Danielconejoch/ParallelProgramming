/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Daniel Conejo <daniel.conejochevez>

#ifndef ASSEMBLERTEST_HPP
#define ASSEMBLERTEST_HPP

#include "Assembler.hpp"
#include "NetworkMessage.hpp"

/**
 * @brief An AssemblerTest class example
 */
class AssemblerTest : public Assembler<NetworkMessage, NetworkMessage> {
  DISABLE_COPY(AssemblerTest);

 protected:
  /// Delay of consumer to consume a package, negative for max random
  int assemblerDelay = 0;
  /// Packages lost
  int packageLostRatio = 0;
  /// Number of messages received
  size_t receivedMessages = 0;
  /// Number of messages lost
  size_t lostMessages = 0;
  // Consumer count
  size_t consumerCount;
  /// Number of messages redirected
  size_t redirectedMessages = 0;

 public:
  /// Constructor
  explicit AssemblerTest(int consumerDelay, int packageLostRatio, size_t consumerCount);
  /// Consume the messages in its own execution thread
  int run() override;
  /// Override this method to process any data extracted from the queue
  void consume(NetworkMessage data) override;
};

#endif  // ASSEMBLERTEST_HPP