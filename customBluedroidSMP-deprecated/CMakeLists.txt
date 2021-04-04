list(APPEND include_dirs
  "smp/include"
  "common/")

list(APPEND srcs "smp/hap_aes.c"
  "smp/hap_p_256_curvepara.c"
  "smp/hap_p_256_ecc_pp.c"
  "smp/hap_p_256_multprecision.c"
  "smp/hap_smp_act.c"
  "smp/hap_smp_api.c"
  "smp/hap_smp_br_main.c"
  "smp/hap_smp_cmac.c"
  "smp/hap_smp_keys.c"
  "smp/hap_smp_l2c.c"
  "smp/hap_smp_main.c"
  "smp/hap_smp_utils.c")


# requirements can't depend on config
idf_component_register(SRCS "${srcs}"
                INCLUDE_DIRS "${include_dirs}"
                REQUIRES nvs_flash soc esp_timer
              PRIV_REQUIRES esp_ipc bt)
