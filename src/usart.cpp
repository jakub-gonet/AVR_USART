#include "usart.hpp"

extern "C" {
#include <avr/io.h>
}
#include "util/fifo.hpp"

bool Usart::send_string(char const* const string) {
  uint8_t data, i = 0;
  while ((data = static_cast<uint8_t>(string[i++]))) {
    to_send.put(data);
  }
  start_sending_data();
}