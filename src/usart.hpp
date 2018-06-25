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
  inline Usart(const uint16_t baud_rate) {
    const uint16_t baud_prescale = (((F_CPU / (baud_rate * 16UL))) - 1);
    UBRRH = (baud_prescale >> 8);
    UBRRL = baud_prescale;

    // enable Tx and Rx
    UCSRB = (1 << TXEN) | (1 << RXEN);

    // set data format to 8 bits, 1 stop bit, no parity check
    UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
  }

  /**
   * @brief Sends string by queueing data in FIFO queue
   *
   * @param string
   * @return uint8_t
   */
  uint8_t send_string(char const* const string) {
    uint8_t data, i = 0;
    while ((data = static_cast<uint8_t>(string[i++]))) {
      if (!to_send.put(data)) {
        return i - 1;
      }
    }
    start_sending_data();
    return 0;
  }

  /**
   * @brief Function designed to be used in ISR(USART_UDRE_vect).
   *
   */
  inline void handle_send_interrupt() {
    uint8_t data = to_send.get();
    if (!to_send.is_empty() && data) {
      UDR = data;
    } else {
      to_send.clear_queue();
      disable_transmit_buffer_empty_interrupts();
    }
  }

  /**
   * @brief Function designed to be used in ISR(USART_RXC_vect).
   *
   */
  inline void handle_receive_interrupt() { received.put(UDR); }

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