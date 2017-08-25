#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_event_loop.h"
#include "nvs_flash.h"
#include "driver/gpio.h"

bool key_status = 1;
bool key_old = 1;
unsigned char longkey = 0;
unsigned char key_down_time=0;
unsigned char key_up_time=0;
unsigned int key_long_time = 0;

#define  LED_PIN_26  GPIO_NUM_26
#define  LED_PIN_27  GPIO_NUM_27

#define REMOTE_PIN   GPIO_NUM_15

void gpio_init(void)
{
	gpio_config_t inpin_conf;
	
	gpio_pad_select_gpio(LED_PIN_26);
	gpio_set_direction(LED_PIN_26,GPIO_MODE_OUTPUT);
	
	gpio_pad_select_gpio(LED_PIN_27);
	gpio_set_direction(LED_PIN_27,GPIO_MODE_OUTPUT);
	
	inpin_conf.intr_type = GPIO_PIN_INTR_DISABLE;
	inpin_conf.mode = GPIO_MODE_INPUT;
	inpin_conf.pin_bit_mask = REMOTE_PIN;
	inpin_conf.pull_up_en = 1;
	gpio_config(&inpin_conf);
	
}

void debounce_handle(void *arg)
{	
	while(1)
	{
		if(gpio_get_level(REMOTE_PIN)==0)
		{
			key_down_time++;
			key_long_time++;
		}
		else{
			key_up_time++;
		}
		
		if(key_long_time == 700)
		{
			longkey=1;
		}
		
		if(key_down_time>=20)
		{
			key_down_time = 0;
			key_status = 0;
		}
		else if(key_up_time>=30)
		{
			key_up_time = 0;
			key_status = 1;
			if(key_long_time>=700)
			{
				key_long_time = 0;
				longkey = 0;
				key_old = 1;
			}
		}
		vTaskDelay(1 / portTICK_RATE_MS);
	}
}

void app_main(void)
{
    nvs_flash_init();
	gpio_init();

	xTaskCreate(debounce_handle, "debounce_handle", 1024, NULL, 1, NULL);
	while(1)
	{
		if(key_status != key_old)
		{
			if(key_status == 1 && key_long_time<500)
			{
				key_long_time = 0;
				gpio_set_level(LED_PIN_27,1);
				vTaskDelay(50 / portTICK_RATE_MS);
				gpio_set_level(LED_PIN_27,0);
			}
			key_old = key_status;
		}
		if(longkey)
		{
			longkey = 0;
			gpio_set_level(LED_PIN_26,1);
			vTaskDelay(200 / portTICK_RATE_MS);
			gpio_set_level(LED_PIN_26,0);
		}
	}
}

