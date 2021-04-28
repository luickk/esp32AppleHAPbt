#include <stdint.h>
#include <string.h>

struct uuid_128 {
	int32_t data[4]; // 32*4 bit
};

extern int32_t apple_128_default_suffix[4];

struct uuid_128 uuid_16_to_apple_hap_uuid(int input_uuid);
