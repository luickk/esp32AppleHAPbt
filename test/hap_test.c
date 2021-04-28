#include "hap_test.h"

void test_uuid_16_to_apple_hap_uuid() {
  struct uuid_128 final_uuid = uuid_16_to_apple_hap_uuid(0x12);
  printf("test_uuid_16_to_apple_hap_uuid: " );
  for (int i=0; i <= 3; i++) {
    printf("%d \n", final_uuid.data[i]);
  }
}
