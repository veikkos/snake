#ifndef _COLOR_H_
#define _COLOR_H_

#include <stdint.h>

class Color {
public:
  Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

  Color(uint8_t r, uint8_t g, uint8_t b);

  inline uint8_t GetR() const
  {
    return r;
  }

  inline uint8_t GetG() const
  {
    return g;
  }

  inline uint8_t GetB() const
  {
    return b;
  }

  inline uint8_t GetA() const
  {
    return a;
  }

private:
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

#endif
