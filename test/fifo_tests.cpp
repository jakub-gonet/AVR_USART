extern "C" {
#include <stdint.h>
}
#include "../src/util/fifo.hpp"
#include "catch.hpp"

#include <iostream>

SCENARIO("Data can be inserted to and retrieved from FIFO queue",
         "[FifoQueue]") {
  GIVEN("An empty FIFO queue") {
    FifoQueue<2> queue;
    REQUIRE(queue.is_empty() == true);

    WHEN("Data is inserted") {
      REQUIRE(queue.put(1) == true);

      THEN("Queue is not empty") { REQUIRE(queue.is_empty() == false); }
      THEN("Data can be retrieved") { REQUIRE(queue.get() == 1); }
    }
  }
}