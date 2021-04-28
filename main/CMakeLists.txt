idf_component_register(SRCS "main.c" "gatt_svr.c" "hap/hap_util.c" "../test/hap_test.c"
                    INCLUDE_DIRS "hap/include" "../test/include"
                  REQUIRES bt)
