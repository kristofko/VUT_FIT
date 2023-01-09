/**
 * @author : Kristof Siska (xsiska16)
 * 
 * Last edited : Dec 2022
*/

#include <driver/adc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "../components/ssd1306/font8x8_basic.h"
#include "../components/ssd1306/ssd1306.h"

void show_timer(SSD1306_t *dev, int top);

void display_starting_position(SSD1306_t *dev);

int display_next_target(SSD1306_t *dev, int top);

bool check_collisions(SSD1306_t *dev, int pos_x, int pos_y, int curr_target);

#define tag "SSD1306"

void app_main(void){
	SSD1306_t dev;
	int top = 2;

	// Config I2C interface
	ESP_LOGI(tag, "INTERFACE is i2c");
	ESP_LOGI(tag, "CONFIG_SDA_GPIO=%d",CONFIG_SDA_GPIO);
	ESP_LOGI(tag, "CONFIG_SCL_GPIO=%d",CONFIG_SCL_GPIO);
	ESP_LOGI(tag, "CONFIG_RESET_GPIO=%d",CONFIG_RESET_GPIO);
	i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
	ESP_LOGI(tag, "Panel is 128x64");
	ssd1306_init(&dev, 128, 64);

	int pages = 7;
	top = 2;
	
	int pos_x = 63;
	int pos_y = 31;
	int curr_target;
	int read_raw_x;
	int read_raw_y;
	int speed_x;
	int speed_y;
	int speed;

	adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_7,ADC_ATTEN_DB_11);
	adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);
	ssd1306_clear_screen(&dev, false);
	curr_target = display_next_target(&dev, top);
	display_starting_position(&dev);
	
	while(1){
		read_raw_x = adc1_get_raw(ADC1_CHANNEL_6);
		read_raw_y = adc1_get_raw(ADC1_CHANNEL_7);

		// normalization
		if (read_raw_x >= 3600){
			read_raw_x = 3600;
		}
		if (read_raw_y >= 3600){
			read_raw_y = 3600;
		}

		speed_x = abs(1800 - read_raw_x);
		speed_x = abs(1800 - speed_x);
		speed_y = abs(1800 - read_raw_y);
		speed_y = abs(1800 - speed_y);
		speed = ((speed_x + speed_y) / 2) / 100;
		
		if (read_raw_x >= 1700 && read_raw_x <= 1950){
			// Do nothing, the car is standing
		}
		else if (read_raw_x < 1700){
			ssd1306_wrap_arround(&dev, SCROLL_LEFT, 0, pages, speed / portTICK_PERIOD_MS);
			pos_x--;
			speed = 0;
		}
		else {
			ssd1306_wrap_arround(&dev, SCROLL_RIGHT, 0, pages, speed / portTICK_PERIOD_MS);
			pos_x++;
			speed = 0;
		}

		if (read_raw_y >= 1700 && read_raw_y <= 1950){
		}
		else if (read_raw_y < 1700){
			ssd1306_wrap_arround(&dev, SCROLL_UP, 0, 127, speed / portTICK_PERIOD_MS);
			pos_y++;
		}
		else {
			ssd1306_wrap_arround(&dev, SCROLL_DOWN, 0, 127, speed / portTICK_PERIOD_MS);
			pos_y--;
		}

		if (check_collisions(&dev, pos_x, pos_y, curr_target)){
			curr_target = display_next_target(&dev, top);
			display_starting_position(&dev);
			pos_y = 31;
			pos_x = 63;
		}
		
	}
}



// Inspired From https://github.com/nopnop2002/esp-idf-ssd1306/blob/master/TextDemo/main/main.c
void show_timer(SSD1306_t *dev, int top){
	uint8_t image[24];
	memset(image, 0, sizeof(image));
	ssd1306_display_image(dev, top, (6*8-1), image, sizeof(image));
	ssd1306_display_image(dev, top+1, (6*8-1), image, sizeof(image));
	ssd1306_display_image(dev, top+2, (6*8-1), image, sizeof(image));
	for(int font=0x33;font>0x30;font--) {
		memset(image, 0, sizeof(image));
		ssd1306_display_image(dev, top+1, (7*8-1), image, 8);
		memcpy(image, font8x8_basic_tr[font], 8);
		if ((*dev)._flip) ssd1306_flip(image, 8);
		ssd1306_display_image(dev, top+1, (7*8-1), image, 8);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
	ssd1306_clear_screen(dev, false);
}

void display_starting_position(SSD1306_t *dev){
	ssd1306_clear_screen(dev, false);
	ssd1306_display_text(dev, 3 , "       _", strlen("       _"), false);
}


int display_next_target(SSD1306_t *dev, int top){
	static int curr_target = 0;
	curr_target++;
	if (curr_target > 3){
		curr_target = 0;
	}
	char text[8] = "";
	switch(curr_target){
		case 0:
			strcpy(text, "UP");
			break;
		case 1:
			strcpy(text, "DOWN");
			break;
		case 2:
			strcpy(text, "RIGHT");
			break;
		case 3:
			strcpy(text, "LEFT");
			break;
		default:
			break;
	}

	ssd1306_clear_screen(dev, false);
	ssd1306_display_text(dev, 3 , text, strlen(text), false);
	show_timer(dev, top);


	return curr_target;
}

bool check_collisions(SSD1306_t *dev, int pos_x, int pos_y, int curr_target){
	switch(curr_target){
		case 0: // UP
			if (pos_y >= 63){
				return true;
			}
			break;
		case 1: // DOWN
			if (pos_y <= 0){
				return true;
			}
			break;
		case 2: // RIGHT
			if (pos_x >= 127){
				return true;
			}
			break;
		case 3: // LEFT
			if (pos_x <= 0){
				return true;
			}
			break;
		default:
			break;
	}

	return false;
}