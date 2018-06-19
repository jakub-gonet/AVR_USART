#ifndef USART_HPP
#define USART_HPP

class Usart {
 public:
  Usart(const uint16_t baud_rate);
  const bool send_string(char const* const buffer,
                         const uint8_t buffer_size) const;
  const char* const receive_string(char* const buffer,
                                   const uint8_t buffer_size) const;

 private:
  const bool send_byte(const uint8_t byte) const;
  const uint8_t receive_byte() const;
};

#endif  // USART_HPP