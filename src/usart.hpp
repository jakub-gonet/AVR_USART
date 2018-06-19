#ifndef USART_HPP
#define USART_HPP

#include <avr/io.h>

class Usart {
 public:
  Usart(const uint16_t baud_rate);
  const bool send_string(char const* const buffer, const uint8_t buffer_size);
  const char* const receive_string(char* const buffer,
                                   const uint8_t buffer_size);

  constexpr const char get_string_delimiter() const { return '\0'; }

  /**
   * @brief Function designed to be used in ISR(USART_UDRE_vect).
   *
   */
  inline void handle_send_interrupt() {
    bool queue_empty = false;
    const uint8_t data = to_send.take_from_queue(queue_empty);

    if (data != get_string_delimiter() && !queue_empty) {
      UDR = data;
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