#include "usart.hpp"
#include <avr/io.h>

Usart::Usart(uint16_t baud_rate) {
  UBRRH = (baud_rate >> 8);
  UBRRL = baud_rate;

  // enable Tx and Rx
  UCSRB = (1 << TXEN) | (1 << RXEN);

  // set data format to 8 bits, 1 stop bit, no parity check
  UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
}