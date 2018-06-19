#ifndef USART_HPP
#define USART_HPP

#include <avr/io.h>

class Usart {
 public:
  Usart(const uint16_t baud_rate);
  const bool send_string(char const* const buffer,
                         const uint8_t buffer_size) const;
  const char* const receive_string(char* const buffer,
                                   const uint8_t buffer_size) const;
  constexpr const char get_string_delimiter() const { return '\0'; }

 private:
  /**
   * @brief FIFO queue used to buffer data to send
   *
   */
  FifoQueue<32> to_send;
  /**
   * @brief FIFO queue used to buffer received data
   *
   */
  FifoQueue<32> received;
};

#endif  // USART_HPP