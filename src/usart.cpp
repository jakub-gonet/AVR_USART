#include "usart.hpp"
#include <avr/io.h>
#include "util/fifo.hpp"

Usart::Usart(uint16_t baud_rate) {
  UBRRH = (baud_rate >> 8);
  UBRRL = baud_rate;

  // enable Tx and Rx
  UCSRB = (1 << TXEN) | (1 << RXEN);

  // set data format to 8 bits, 1 stop bit, no parity check
  UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
}

const bool Usart::send_string(char const* const buffer,
                              const uint8_t buffer_size) {
  for (uint8_t i = 0; i < buffer_size; ++i) {
    to_send.push_to_queue(static_cast<uint8_t>(buffer[i]));
  }

  start_sending_data();
}