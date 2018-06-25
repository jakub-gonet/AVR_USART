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
  bool put(const uint8_t byte) volatile {
    if (!is_full()) {
      ++item_count;
      buffer[(++head) % size] = byte;
      return true;
    }
    return false;
  }

  /**
   * @brief Returns next element from queue
   *
   * @return uint8_t -1 indicating error
   */
  uint8_t get() volatile {
    if (!is_empty()) {
      --item_count;
      uint8_t data = buffer[(++tail) % size];
      return data;
    }
    return -1;
  }

  inline bool is_empty() volatile { return item_count == 0; }
  inline bool is_full() volatile { return item_count == size; }

  /**
   * @brief Clears queue
   *
   */
  void clear_queue() volatile {
    head = -1;
    tail = -1;
    item_count = 0;
  }

 private:
  const uint8_t size = Size;
  uint8_t item_count = 0;
  uint8_t buffer[Size];
  uint8_t head = -1;
  uint8_t tail = -1;
};
#endif  // FIFO_HPP