list(APPEND include_dirs
  "include")

list(APPEND srcs "custom_ble_sm.c")


# requirements can't depend on config
idf_component_register(SRCS "${srcs}"
                INCLUDE_DIRS "${include_dirs}"
              PRIV_REQUIRES bt)
