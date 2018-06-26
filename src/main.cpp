extern "C" {
#include <avr/interrupt.h>
#include <avr/io.h>
}
#include "usart.hpp"

Usart usart(4800);
char buffer[64];
int main() {
  sei();
  while (true) {
    usart.send_string(usart.receive_string(buffer, 64));
  }
}

ISR(USART_UDRE_vect) {
  usart.handle_send_interrupt();
}

ISR(USART_RXC_vect) {
  usart.handle_receive_interrupt();
}