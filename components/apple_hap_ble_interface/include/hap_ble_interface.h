#include "esp_log.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOSConfig.h"

/* BLE */
#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"

#include "host/ble_hs.h"
#include "host/util/util.h"
#include "console/console.h"
#include "services/gap/ble_svc_gap.h"

#include "gatt_cfg.h"

#include "hap_util.h"
#include "hap_test.h"

int init_ble();
