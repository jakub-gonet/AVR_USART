#ifndef FIFO_HPP
#define FIFO_HPP

#include <stdint.h>

template <uint8_t Size = 32>
class FifoQueue {
 private:
  uint8_t fifo_size = Size;
  uint8_t sending_queue[Size];
  uint8_t next_queue_element_index = 0;
  uint8_t last_queue_element_index = 0;
};
#endif  // FIFO_HPP