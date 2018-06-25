#include "usart.hpp"

extern "C" {
#include <avr/io.h>
}
#include "util/fifo.hpp"

Usart::Usart(const uint16_t baud_rate) {
  const uint16_t baud_prescale = (((F_CPU / (baud_rate * 16UL))) - 1);
  UBRRH = (baud_prescale >> 8);
  UBRRL = baud_prescale;

  // enable Tx and Rx
  UCSRB = (1 << TXEN) | (1 << RXEN);

  // set data format to 8 bits, 1 stop bit, no parity check
  UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
}

bool Usart::send_string(char const* const string) {
  uint8_t data, i = 0;
  while ((data = static_cast<uint8_t>(string[i++]))) {
    to_send.put(data);
  }
  start_sending_data();
}

void Usart::receive_string(char* const buffer,
                           const uint8_t buffer_size,
                           const char delimiter) {
  bool queue_empty = false;
  for (uint8_t i = 0; i < buffer_size && !queue_empty; ++i) {
    buffer[i] = received.take_from_queue(queue_empty);

    if (buffer[i] == delimiter) {
      break;
    }
  }
  received.clear_queue();
}