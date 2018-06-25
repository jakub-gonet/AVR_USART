#ifndef USART_HPP
#define USART_HPP

extern "C" {
#include <avr/io.h>
}
#include "util/fifo.hpp"

class Usart {
 public:
  /**
   * @brief Construct a new Usart object with specified baud rate
   *
   * @param baud_rate
   */
  Usart(const uint16_t baud_rate);

  /**
   * @brief Sends string by queueing data in FIFO queue
   *
   * @param string
   * @return true
   * @return false
   */
  bool send_string(char const* const string);

  /**
   * @brief Function designed to be used in ISR(USART_UDRE_vect).
   *
   */
  inline void handle_send_interrupt() {
    if (!to_send.is_empty()) {
      UDR = to_send.get();
    } else {
      disable_transmit_buffer_empty_interrupts();
    }
  }

  /**
   * @brief Function designed to be used in ISR(USART_RXC_vect).
   *
   */
  inline void handle_receive_interrupt() { received.push_to_queue(UDR); }

 private:
  /**
   * @brief FIFO queue used to buffer data to send
   *
   */
  volatile FifoQueue<32> to_send;
  /**
   * @brief FIFO queue used to buffer received data
   *
   */
  volatile FifoQueue<32> received;

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