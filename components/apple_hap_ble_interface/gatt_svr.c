/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "host/ble_hs.h"
#include "host/ble_uuid.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"

#include "gatt_cfg.h"
#include "hap_ids.h"

static const char *manuf_name = "fake apple";
static const char *model_num = "1.0";
uint16_t hrs_hrm_handle;

static int
gatt_svr_chr_access_pairing(uint16_t conn_handle, uint16_t attr_handle,
                               struct ble_gatt_access_ctxt *ctxt, void *arg);

static int
gatt_svr_chr_access_device_info(uint16_t conn_handle, uint16_t attr_handle,
                                struct ble_gatt_access_ctxt *ctxt, void *arg);

// full list of characteristic flags : https://github.com/apache/mynewt-nimble/blob/master/nimble/host/include/host/ble_gatt.h
static const struct ble_gatt_svc_def gatt_svr_svcs[] = {
    {
        /* Service: Heart-rate */
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID128_DECLARE(UUID_128_SERVICE_PAIRING),
        .characteristics = (struct ble_gatt_chr_def[])
        { {
                .uuid = BLE_UUID128_DECLARE(UUID_128_CHARACTERISTIC_PAIRING_SETUP),
                .access_cb = gatt_svr_chr_access_pairing,
                .val_handle = &hrs_hrm_handle,
                .flags = BLE_GATT_CHR_F_READ |
                 BLE_GATT_CHR_F_WRITE ,
            }, {
                .uuid = BLE_UUID128_DECLARE(UUID_128_CHARACTERISTIC_PAIRING_VERIFY),
                .access_cb = gatt_svr_chr_access_pairing,
                .flags = BLE_GATT_CHR_F_READ |
                 BLE_GATT_CHR_F_WRITE ,
            }, {
                .uuid = BLE_UUID128_DECLARE(UUID_128_CHARACTERISTIC_PAIRING_FEATURE),
                .access_cb = gatt_svr_chr_access_pairing,
                .flags = BLE_GATT_CHR_F_READ,
            }, {
                .uuid = BLE_UUID128_DECLARE(UUID_128_CHARACTERISTIC_PAIRING_PAIRINGS),
                .access_cb = gatt_svr_chr_access_pairing,
                .flags = BLE_GATT_CHR_F_READ_AUTHEN | BLE_GATT_CHR_F_WRITE_AUTHEN,
            }, {
                0, /* No more characteristics in this service */
            },
        }
    },

    {
        /* Service: Device Information */
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(UUID_128_SERVICE_ACCESSORY_INFORMATION),
        .characteristics = (struct ble_gatt_chr_def[])
        { {
                .uuid = BLE_UUID128_DECLARE(UUID_128_CHARACTERISTIC_FIRMWARE_REVISION),
                .access_cb = gatt_svr_chr_access_device_info,
                .flags = BLE_GATT_CHR_F_READ_AUTHEN,
            }, {
                .uuid = BLE_UUID128_DECLARE(UUID_128_CHARACTERISTIC_IDENTIFY),
                .access_cb = gatt_svr_chr_access_device_info,
                .flags = BLE_GATT_CHR_F_WRITE_AUTHEN,
            },  {
                .uuid = BLE_UUID128_DECLARE(UUID_128_CHARACTERISTIC_MANUFACTURER),
                .access_cb = gatt_svr_chr_access_device_info,
                .flags = BLE_GATT_CHR_F_READ_AUTHEN,
            },  {
                .uuid = BLE_UUID128_DECLARE(UUID_128_CHARACTERISTIC_NAME),
                .access_cb = gatt_svr_chr_access_device_info,
                .flags = BLE_GATT_CHR_F_READ_AUTHEN,
            },  {
                .uuid = BLE_UUID128_DECLARE(UUID_128_CHARACTERISTIC_SERIAL_NUMBER),
                .access_cb = gatt_svr_chr_access_device_info,
                .flags = BLE_GATT_CHR_F_READ_AUTHEN,
            }, {
                0, /* No more characteristics in this service */
            },
        }
    },

    {
        /* Service: Device Information */
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = BLE_UUID16_DECLARE(UUID_128_SERVICE_PROTOCOL_INFORMATION),
        .characteristics = (struct ble_gatt_chr_def[])
        { {
                .uuid = BLE_UUID128_DECLARE(UUID_128_CHARACTERISTIC_VERSION),
                .access_cb = gatt_svr_chr_access_device_info,
                .flags = BLE_GATT_CHR_F_READ_AUTHEN,
            },
            {
                0, /* No more characteristics in this service */
            },
        }
    },

    {
        0, /* No more services */
    },
};

static int
gatt_svr_chr_access_pairing(uint16_t conn_handle, uint16_t attr_handle,
                               struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    /* Sensor location, set to "Chest" */
    static uint8_t body_sens_loc = 0x01;
    uint16_t uuid;
    int rc;

    uuid = ble_uuid_u16(ctxt->chr->uuid);

    if (uuid == UUID_128_CHARACTERISTIC_PAIRING_SETUP) {
        // rc = os_mbuf_append(ctxt->om, &body_sens_loc, sizeof(body_sens_loc));
        printf("PAIRING_SETUP ACCESSED");
    } else if (uuid == UUID_128_CHARACTERISTIC_PAIRING_VERIFY) {
        // rc = os_mbuf_append(ctxt->om, &body_sens_loc, sizeof(body_sens_loc));
        printf("VERIFY ACCESSED");

    } else if (uuid == UUID_128_CHARACTERISTIC_PAIRING_FEATURE) {
        // rc = os_mbuf_append(ctxt->om, &body_sens_loc, sizeof(body_sens_loc));
        printf("FEATURE ACCESSED");

    } else if (uuid == UUID_128_CHARACTERISTIC_PAIRING_PAIRINGS) {
        // rc = os_mbuf_append(ctxt->om, &body_sens_loc, sizeof(body_sens_loc));
        printf("PAIRING_PAIRINGS ACCESSED");

    }

    return 0;
}

static int
gatt_svr_chr_access_device_info(uint16_t conn_handle, uint16_t attr_handle,
                                struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    uint16_t uuid;
    int rc;

    uuid = ble_uuid_u16(ctxt->chr->uuid);

    if (uuid == GATT_MODEL_NUMBER_UUID) {
        rc = os_mbuf_append(ctxt->om, model_num, strlen(model_num));
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    if (uuid == GATT_MANUFACTURER_NAME_UUID) {
        rc = os_mbuf_append(ctxt->om, manuf_name, strlen(manuf_name));
        return rc == 0 ? 0 : BLE_ATT_ERR_INSUFFICIENT_RES;
    }

    assert(0);
    return BLE_ATT_ERR_UNLIKELY;
}

void
gatt_svr_register_cb(struct ble_gatt_register_ctxt *ctxt, void *arg)
{
    char buf[BLE_UUID_STR_LEN];

    switch (ctxt->op) {
    case BLE_GATT_REGISTER_OP_SVC:
        MODLOG_DFLT(DEBUG, "registered service %s with handle=%d\n",
                    ble_uuid_to_str(ctxt->svc.svc_def->uuid, buf),
                    ctxt->svc.handle);
        break;

    case BLE_GATT_REGISTER_OP_CHR:
        MODLOG_DFLT(DEBUG, "registering characteristic %s with "
                    "def_handle=%d val_handle=%d\n",
                    ble_uuid_to_str(ctxt->chr.chr_def->uuid, buf),
                    ctxt->chr.def_handle,
                    ctxt->chr.val_handle);
        break;

    case BLE_GATT_REGISTER_OP_DSC:
        MODLOG_DFLT(DEBUG, "registering descriptor %s with handle=%d\n",
                    ble_uuid_to_str(ctxt->dsc.dsc_def->uuid, buf),
                    ctxt->dsc.handle);
        break;

    default:
        assert(0);
        break;
    }
}

int
gatt_svr_init(void)
{
    int rc;

    ble_svc_gap_init();
    ble_svc_gatt_init();

    rc = ble_gatts_count_cfg(gatt_svr_svcs);
    if (rc != 0) {
        return rc;
    }

    rc = ble_gatts_add_svcs(gatt_svr_svcs);
    if (rc != 0) {
        return rc;
    }

    return 0;
}
