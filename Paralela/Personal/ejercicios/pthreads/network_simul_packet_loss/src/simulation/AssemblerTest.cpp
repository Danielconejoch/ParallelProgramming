/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Daniel Conejo <daniel.conejochevez>

#include "AssemblerTest.hpp"
#include "Log.hpp"
#include "Util.hpp"

AssemblerTest::AssemblerTest(int assemblerDelay, int packageLostRatio
  , size_t consumerCount)
  : assemblerDelay(assemblerDelay)
  , packageLostRatio(packageLostRatio)
  , consumerCount(consumerCount)  {
}

int AssemblerTest::run() {
  // Start the forever loop to consume all the messages that arrive
  this->consumeForever();


  this->produce(NetworkMessage());

  // If the forever loop finished, no more messages will arrive
  // Print statistics
  Log::append(Log::INFO, "Assembler", std::to_string(this->receivedMessages)
    + " received messages");
  Log::append(Log::INFO, "Assembler", std::to_string(this->lostMessages)
  + " redirected messages" );
  return EXIT_SUCCESS;
}

void AssemblerTest::consume(NetworkMessage data) {
  (void)data;
  ++this->receivedMessages;
  int random = Util::random(0,99);
  if (random > this->packageLostRatio){
    this->lostMessages++;
  } else {
    int target = 1 + Util::random(0
    , static_cast<int>(this->consumerCount - 1));
    this->produce(NetworkMessage(target, this->consumerCount, data.messageNumber));
    this->redirectedMessages++;
  }
}