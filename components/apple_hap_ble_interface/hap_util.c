#include "hap_util.h"

#include "esp_system.h"

uint8_t apple_128_default_suffix[16] = {0x0, 0x0, 0x0, 0x0, 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0x26, 0xBB, 0x76, 0x52, 0x91};

uint8_t *generate_device_id48() {
  uint8_t *ret = malloc(sizeof(uint8_t)*6);
  if(!ret) {
    return NULL;
  }
  esp_fill_random(ret, sizeof(uint8_t)*6);

  return ret;
}

uint8_t *uuid_16_to_apple_hap_uuid(uint32_t uuid16) {
  uint8_t *ret = malloc(sizeof(apple_128_default_suffix));
  if(!ret) {
    return NULL;
  }
  memcpy(ret, apple_128_default_suffix, sizeof(apple_128_default_suffix));
  uint8_t* uuid16_bytes = (uint8_t*)(&uuid16);
  ret[0] = uuid16_bytes[3];
  ret[1] = uuid16_bytes[2];
  ret[2] = uuid16_bytes[1];
  ret[3] = uuid16_bytes[0];

  return ret;
}

void print_apple_hap_uuid(uint8_t *input_uuid) {
  for(int i = 0; i < 16; i++) {
    if (i==3||i==7||i==13) {
      printf("-");
    }
    printf("%.2x", input_uuid[i]);
  }
}

void
print_bytes(const uint8_t *bytes, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        printf("%s0x%02x", i != 0 ? ":" : "", bytes[i]);
    }
}

void
print_addr(const void *addr)
{
    const uint8_t *u8p;

    u8p = addr;
    printf("%02x:%02x:%02x:%02x:%02x:%02x", u8p[5], u8p[4], u8p[3], u8p[2], u8p[1], u8p[0]);
}
