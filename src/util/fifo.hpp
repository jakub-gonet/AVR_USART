#ifndef FIFO_HPP
#define FIFO_HPP
extern "C" {
#include <stdint.h>
}

template <uint8_t Size = 32>
class FifoQueue {
 public:
  /**
   * @brief Pushes new item to FIFO queue
   *
   * @param byte
   * @return true
   * @return false
   */
  bool push_to_queue(const uint8_t byte) volatile {
    if (last_queue_element_index < fifo_size) {
      queue[last_queue_element_index++] = byte;
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
  uint8_t take_from_queue() volatile {
    if (next_queue_element_index < last_queue_element_index) {
      return queue[next_queue_element_index++];
    }

    clear_queue();
    return -1;
  }

  bool is_empty() volatile {
    return next_queue_element_index == last_queue_element_index;
  }

  /**
   * @brief Clears queue
   *
   */
  void clear_queue() volatile {
    next_queue_element_index = 0;
    last_queue_element_index = 0;
  }

  private:
  const uint8_t size = Size;
  uint8_t buffer[Size];
  uint8_t head = 0;
  uint8_t tail = 0;
};
#endif  // FIFO_HPP