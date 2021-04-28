#include "hap_util.h"

int32_t apple_128_default_suffix[4] = {0x0, 0x00001000, 0x80000026, 0xBB765291};

struct uuid_128 uuid_16_to_apple_hap_uuid(int32_t input_uuid) {
  int32_t final_uuid[4] = {0};
  strncpy(final_uuid, apple_128_default_suffix, 4);
  final_uuid[0] = input_uuid;
  struct uuid_128 s = { {final_uuid} };
  return s;
}
