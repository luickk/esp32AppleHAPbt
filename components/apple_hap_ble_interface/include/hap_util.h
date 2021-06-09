#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef HAP_UTIL_H_
# define HAP_UTIL_H_

extern uint8_t apple_128_default_suffix[16];

typedef union {
  struct {
    unsigned char ch1;
    unsigned char ch2;
  };
  uint16_t uint16;
} cu16_t;

// uint64 mustn't be larger than 48 bit
typedef union {
    uint8_t uint8[6];
    uint16_t uint64;
} u48_t;

uint8_t *generate_device_id48();

uint8_t *uuid_16_to_apple_hap_uuid(uint32_t uuid16);

void print_apple_hap_uuid(uint8_t *input_uuid);

void print_addr(const void *addr);

void print_bytes(const uint8_t *bytes, int len);

#endif
