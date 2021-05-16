list(APPEND srcs "hap_ble_interface.c"
  "hap_util.c"
  "hap_test.c"
  "gatt_svr.c")

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS "include"
                    REQUIRES bt)
