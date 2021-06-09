#include "hap_ble_interface.h"

static const char *DEBUG_INFO_TAG = "gatt_central_init";

static bool notify_state;

static uint16_t conn_handle;

static const char *device_name = "apple home test device";

static int blehr_gap_event(struct ble_gap_event *event, void *arg);

static uint8_t blehr_addr_type;


/*
 * Enables advertising with parameters:
 *     o General discoverable mode
 *     o Undirected connectable mode
 */
static void
blehr_advertise(void)
{
    struct ble_gap_adv_params adv_params;
    struct ble_hs_adv_fields fields;
    int rc;

    /*
     * Advertise two flags:
     *      o Discoverability in forthcoming advertisement (general)
     *      o BLE-only (BR/EDR unsupported)
     */
    fields.flags = ADV_DATA_FLAG;

    cu16_t accessory_category;
    accessory_category.uint16 = AC_FANS;

    uint8_t *device_id = generate_device_id48();

    // very stupid, only for dev purposes..... will be replaced by bitmask
    typedef union
    {
      struct
      {
        unsigned char b1 : 1;
        unsigned char b2 : 1;
        unsigned char b3 : 1;
        unsigned char b4 : 1;
        unsigned char b5 : 1;
        unsigned char b6 : 1;
        unsigned char b7 : 1;
        unsigned char b8 : 1;
      }c_bits;
      unsigned char c;
    }accessory_pairing_state;
    accessory_pairing_state sf;
    // sf.c_bits.b1 = 0;
    sf.c_bits.b2 = 0;
    sf.c_bits.b3 = 0;
    sf.c_bits.b4 = 0;
    sf.c_bits.b5 = 0;
    sf.c_bits.b6 = 0;
    sf.c_bits.b7 = 0;
    sf.c_bits.b8 = 0;
    typedef union
    {
      struct
      {
        unsigned char b1 : 1;
        unsigned char b2 : 1;
        unsigned char b3 : 1;
        unsigned char b4 : 1;
        unsigned char b5 : 1;
        unsigned char b6 : 1;
        unsigned char b7 : 1;
        unsigned char b8 : 1;
      }c_bits;
      unsigned char c;
    }subtype_len;
    subtype_len stl;
    // sf.c_bits.b1 = 0;
    stl.c_bits.b2 = 1;
    stl.c_bits.b3 = 1;
    stl.c_bits.b4 = 1;
    stl.c_bits.b5 = 1;
    stl.c_bits.b6 = 0;
    stl.c_bits.b7 = 0;
    stl.c_bits.b8 = 1;

    uint8_t adv_manufacturer_data[] = { APPLE_COMPANY_IDENTIFIER, 0x0, ADV_DATA_MANUFACTURER_DATA_TYPE,
      stl.c, sf.c, device_id[0], device_id[1], device_id[2], device_id[3],
      device_id[4], device_id[5], accessory_category.ch1, accessory_category.ch2, 1, 0, 0x1, 0x02, 0x0, 0x0, 0x0, 0x0};

    print_bytes(adv_manufacturer_data, 21);
    printf("\n");

    fields.mfg_data = adv_manufacturer_data;
    fields.mfg_data_len = sizeof adv_manufacturer_data;

    fields.name = (uint8_t *)device_name;
    fields.name_len = strlen(device_name);
    fields.name_is_complete = 1;


    /*
     *  Set the advertisement data included in our advertisements:
     *     o Flags (indicates advertisement type and other general info)
     *     o Advertising tx power
     *     o Device name
     */
    memset(&fields, 0, sizeof(fields));

    rc = ble_gap_adv_set_fields(&fields);
    if (rc != 0) {
        MODLOG_DFLT(ERROR, "error setting advertisement data; rc=%d\n", rc);
        return;
    }

    /* Begin advertising */
    memset(&adv_params, 0, sizeof(adv_params));
    // adv_params.conn_mode = BLE_GAP_CONN_MODE_UND; // BLE_GAP_CONN_MODE_DIR; // undirect advertising mode
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN; // general advertising mode

    adv_params.itvl_min = 160;
    adv_params.itvl_max = 2500;

    // BLE_OWN_ADDR_PUBLIC
    // BLE_OWN_ADDR_RANDOM
    // BLE_OWN_ADDR_RPA_PUBLIC_DEFAULT
    // BLE_OWN_ADDR_RPA_RANDOM_DEFAULT
    rc = ble_gap_adv_start(BLE_OWN_ADDR_PUBLIC, NULL, BLE_HS_FOREVER,
                           &adv_params, blehr_gap_event, NULL);
    if (rc != 0) {
        MODLOG_DFLT(ERROR, "error enabling advertisement; rc=%d\n", rc);
        return;
    }
}

static int
blehr_gap_event(struct ble_gap_event *event, void *arg)
{
    switch (event->type) {
    case BLE_GAP_EVENT_CONNECT:
        /* A new connection was established or a connection attempt failed */
        MODLOG_DFLT(INFO, "connection %s; status=%d\n",
                    event->connect.status == 0 ? "established" : "failed",
                    event->connect.status);

        if (event->connect.status != 0) {
            /* Connection failed; resume advertising */
            blehr_advertise();
        }

        conn_handle = event->connect.conn_handle;

        int rc = ble_gap_security_initiate(conn_handle);
        assert(rc == 0);

        break;

    case BLE_GAP_EVENT_DISCONNECT:
        MODLOG_DFLT(INFO, "disconnect; reason=%d\n", event->disconnect.reason);

        /* Connection terminated; resume advertising */
        blehr_advertise();
        break;

    case BLE_GAP_EVENT_ADV_COMPLETE:
        MODLOG_DFLT(INFO, "adv complete\n");
        blehr_advertise();
        break;

    case BLE_GAP_EVENT_SUBSCRIBE:
        MODLOG_DFLT(INFO, "subscribe event; cur_notify=%d\n value handle; "
                    "val_handle=%d\n",
                    event->subscribe.cur_notify, hrs_hrm_handle);
        if (event->subscribe.attr_handle == hrs_hrm_handle) {
            notify_state = event->subscribe.cur_notify;
            // blehr_tx_hrate_reset();
        } else if (event->subscribe.attr_handle != hrs_hrm_handle) {
            notify_state = event->subscribe.cur_notify;
            // blehr_tx_hrate_stop();
        }
        ESP_LOGI("BLE_GAP_SUBSCRIBE_EVENT", "conn_handle from subscribe=%d", conn_handle);
        break;

    case BLE_GAP_EVENT_MTU:
        MODLOG_DFLT(INFO, "mtu update event; conn_handle=%d mtu=%d\n",
                    event->mtu.conn_handle,
                    event->mtu.value);
        break;

    }

    return 0;
}

static void
blehr_on_sync(void)
{
    int rc;

    rc = ble_hs_id_infer_auto(0, &blehr_addr_type);
    assert(rc == 0);

    uint8_t addr_val[6] = {0};
    rc = ble_hs_id_copy_addr(blehr_addr_type, addr_val, NULL);

    MODLOG_DFLT(INFO, "Device Address: ");
    print_addr(addr_val);
    MODLOG_DFLT(INFO, "\n");

    /* Begin advertising */
    blehr_advertise();
}

static void
blehr_on_reset(int reason)
{
    MODLOG_DFLT(ERROR, "Resetting state; reason=%d\n", reason);
}

void blehr_host_task(void *param)
{
    ESP_LOGI(DEBUG_INFO_TAG, "BLE Host Task Started");
    /* This function will return only when nimble_port_stop() is executed */
    nimble_port_run();

    nimble_port_freertos_deinit();
}

/*
 * API
 */

int init_ble()
{
    int rc;

    /* Initialize NVS — it is used to store PHY calibration data */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_nimble_hci_and_controller_init());

    nimble_port_init();

    /* Initialize the NimBLE host configuration */
    ble_hs_cfg.sync_cb = blehr_on_sync;
    ble_hs_cfg.reset_cb = blehr_on_reset;

    rc = gatt_svr_init();
    if(rc != 0) {
      ESP_LOGE(DEBUG_INFO_TAG, "gatt service init failed, error code = %x", rc);
      return 1;
    }

    /* Set the default device name */
    rc = ble_svc_gap_device_name_set(device_name);
    if(rc != 0) {
      ESP_LOGE(DEBUG_INFO_TAG, "ble gap service name set failed, error code = %x", rc);
      return 1;
    }
    /* Changing the default MTU size */
    rc = ble_att_set_preferred_mtu(155);
    if(rc != 0) {
      ESP_LOGE(DEBUG_INFO_TAG, "ble gatt mtu set failed, error code = %x", rc);
      return 1;
    }

    /* Start the task */
    nimble_port_freertos_init(blehr_host_task);

    // esp_err_t local_mtu_ret = esp_ble_gatt_set_local_mtu(512);
    // if (local_mtu_ret){
    //   ESP_LOGE(DEBUG_INFO_TAG, "set local  MTU failed, error code = %x", local_mtu_ret);
    //   return 1;
    // }

    return 0;
}
