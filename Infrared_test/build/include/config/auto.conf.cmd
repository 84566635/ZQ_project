deps_config := \
	/home/huangxiaoming/esp/esp-idf/components/aws_iot/Kconfig \
	/home/huangxiaoming/esp/esp-idf/components/bt/Kconfig \
	/home/huangxiaoming/esp/esp-idf/components/esp32/Kconfig \
	/home/huangxiaoming/esp/esp-idf/components/ethernet/Kconfig \
	/home/huangxiaoming/esp/esp-idf/components/fatfs/Kconfig \
	/home/huangxiaoming/esp/esp-idf/components/freertos/Kconfig \
	/home/huangxiaoming/esp/esp-idf/components/log/Kconfig \
	/home/huangxiaoming/esp/esp-idf/components/lwip/Kconfig \
	/home/huangxiaoming/esp/esp-idf/components/mbedtls/Kconfig \
	/home/huangxiaoming/esp/esp-idf/components/openssl/Kconfig \
	/home/huangxiaoming/esp/esp-idf/components/spi_flash/Kconfig \
	/home/huangxiaoming/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/huangxiaoming/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/huangxiaoming/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/huangxiaoming/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
