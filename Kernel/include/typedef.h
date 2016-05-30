
#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>

typedef struct {
  uint16_t offset_l;
  uint16_t selector;
  uint8_t zero;
  uint8_t type_attr;
  uint16_t offset_m;
  uint32_t offset_h;
  uint32_t zero2;
} IDT_entry;

typedef struct {
  uint16_t limit;
  uint64_t base;
} IDTR;
#endif
