list(APPEND include_dirs
  "include")

list(APPEND srcs "ble_sm.c")


# requirements can't depend on config
idf_component_register(SRCS "${srcs}"
                INCLUDE_DIRS "${include_dirs}"
                REQUIRES nvs_flash soc esp_timer
              PRIV_REQUIRES esp_ipc bt)
