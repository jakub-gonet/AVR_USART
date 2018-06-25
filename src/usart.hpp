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

  /**
   * @brief Start the proccess of sending data from `to_send` queue. It should
   * end after reaching delimiter or if queue is empty
   *
   */
  inline void start_sending_data() const {
    wait_until_transmit_buffer_is_ready();
    enable_transmit_buffer_empty_interrupts();
  }

  inline void wait_until_transmit_buffer_is_ready() const {
    while (!(UCSRA & (1 << UDRE)))
      ;
  }
  inline void enable_transmit_buffer_empty_interrupts() const {
    UCSRB |= (1 << UDRIE);
  }

  inline void disable_transmit_buffer_empty_interrupts() const {
    UCSRB &= ~(1 << UDRIE);
  }
};

#endif  // USART_HPP