#ifndef FIFO_HPP
#define FIFO_HPP
extern "C" {
#include <stdint.h>
}

template <uint8_t Size = 32>
class FifoQueue {
 public:
  /**
   * @brief Pushes new item to FIFO sending queue
   *
   * @param byte
   * @return true
   * @return false
   */
  bool push_to_queue(const uint8_t byte) volatile {
    if (last_queue_element_index < fifo_size) {
      sending_queue[last_queue_element_index++] = byte;
      return true;
    }
    return false;
  }

  /**
   * @brief Returns next element from queue
   *
   * @param queue_empty when true, indicates that queue is empty, so it can't
   * provide next element
   * @return const uint8_t
   */
  uint8_t take_from_queue(bool& queue_empty) volatile {
    if (next_queue_element_index <= last_queue_element_index) {
      queue_empty = false;
      return sending_queue[next_queue_element_index++];
    }
    next_queue_element_index = 0;
    last_queue_element_index = 0;

    queue_empty = true;
    return 0;
  }

 private:
  const uint8_t fifo_size = Size;
  uint8_t sending_queue[Size];
  uint8_t next_queue_element_index = 0;
  uint8_t last_queue_element_index = 0;
};
#endif  // FIFO_HPP