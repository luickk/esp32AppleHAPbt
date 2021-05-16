#include "hap_test.h"

void test_uuid_16_to_apple_hap_uuid_print() {
  uint8_t *uuid128 = uuid_16_to_apple_hap_uuid(0x55);
  printf("test_uuid_16_to_apple_hap_uuid: " );
  print_apple_hap_uuid(uuid128);
  printf("\n");
}
