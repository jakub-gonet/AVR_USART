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
   * NOTE: it will exit immediately if sending queue is full
   *
   * @param string
   */
  void send_string(char const* const string) {
    uint8_t data;
    uint16_t i = 0;
    while ((data = static_cast<uint8_t>(string[i++]))) {
      for (bool started = false; !to_send.put(data);) {
        if (!started) {
          start_sending_data();
          started = true;
        }
      }
    }

    start_sending_data();
  }

  /**
   * @brief Receives string by taking data from queue
   *
   * @param buffer
   * @param buffer_size
   * @return char*
   */
  char* receive_string(char* const buffer, uint8_t buffer_size) {
    for (uint8_t i = 0; i < buffer_size && !received.is_empty(); ++i) {
      buffer[i] = received.get();
    }
    return buffer;
  }

  /**
   * @brief Function designed to be used in ISR(USART_UDRE_vect).
   *
   */
  inline void handle_send_interrupt() {
    uint8_t data = to_send.get();
    if (data != static_cast<uint8_t>(-1) && data) {
      UDR = data;
    } else {
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