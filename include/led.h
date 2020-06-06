#ifndef _LED_H_
#define _LED_H_

extern int led_status;

void init_led();
void destroy_led();
void led_on();
void led_off();

#endif	// !_LED_H_