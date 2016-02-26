
#include "juma_sdk_vectors.h"

const system_descriptor_t* sys;

void on_init(const system_descriptor_t* system)
{
    sys = system;
}

__weak void on_ready(void) {}
__weak void gpio_on_change(uint32_t pins_state) {}
__weak void timer_on_fired(void) {}
__weak void ble_device_on_connect(void) {}
__weak void ble_device_on_disconnect(uint8_t reason) {}
__weak void ble_device_on_message(uint8_t type, uint16_t length, uint8_t* value) {}
__weak void serial_on_data(uint8_t data) {}
__weak void data_storage_on_finish(uint8_t op_code) {}
__weak void watchDog_on_timerout(uint32_t juma_wdt_statue) {}
void on_init(const system_descriptor_t* sys);
const app_descriptor_t app_descriptor __attribute__((section("descriptor"))) = {
	on_init,
	on_ready,
	gpio_on_change,
	timer_on_fired,
	ble_device_on_connect,
	ble_device_on_disconnect,
	ble_device_on_message,
	serial_on_data,
	data_storage_on_finish,
	watchDog_on_timerout,
};
inline void run_when_idle(function_t func, void* args)
{
	sys->run_when_idle(func, args);
}
inline void run_at_time(function_t func, void* args, uint32_t time)
{
	sys->run_at_time(func, args, time);
}
inline uint32_t current_time(void)
{
	return sys->current_time();
}
inline void timer_init(uint8_t prescalar, uint8_t timer_type)
{
	sys->timer_init(prescalar, timer_type);
}
inline uint32_t timer_start(uint16_t value)
{
	return sys->timer_start(value);
}
inline void timer_stop(void)
{
	sys->timer_stop();
}
inline void gpio_setup(uint8_t pin, uint8_t mode)
{
	sys->gpio_setup(pin, mode);
}
inline void gpio_write(uint8_t pin, uint8_t state)
{
	sys->gpio_write(pin, state);
}
inline uint8_t gpio_read(uint8_t pin)
{
	return sys->gpio_read(pin);
}
inline void gpio_watch(uint8_t pin, uint8_t change_direction)
{
	sys->gpio_watch(pin, change_direction);
}
inline void gpio_unwatch(uint8_t pin)
{
	sys->gpio_unwatch(pin);
}
inline void adc_measure(uint8_t pin, uint8_t resolution, function_t on_complete)
{
	sys->adc_measure(pin, resolution, on_complete);
}
inline void vcc_measure(function_t on_complete)
{
	sys->vcc_measure(on_complete);
}
inline int8_t get_temperature(void)
{
	return sys->get_temperature();
}
inline uint8_t ble_device_is_connected(void)
{
	return sys->ble_device_is_connected();
}
inline void ble_device_get_id(uint8_t* id, uint8_t len)
{
	sys->ble_device_get_id(id, len);
}
inline void ble_device_select_address(uint8_t id)
{
	sys->ble_device_select_address(id);
}
inline void ble_device_set_name(const char* device_name)
{
	sys->ble_device_set_name(device_name);
}
inline void ble_device_set_advertising_interval(uint16_t interval)
{
	sys->ble_device_set_advertising_interval(interval);
}
inline void ble_device_set_tx_power(int8_t tx_power)
{
	sys->ble_device_set_tx_power(tx_power);
}
inline void ble_device_start_advertising(void)
{
	sys->ble_device_start_advertising();
}
inline void ble_device_stop_advertising(void)
{
	sys->ble_device_stop_advertising();
}
inline void ble_device_disconnect(void)
{
	sys->ble_device_disconnect();
}
inline void ble_device_send(uint8_t type, uint32_t length, uint8_t* value)
{
	sys->ble_device_send(type, length, value);
}
inline uint8_t random_generate(void)
{
	return sys->random_generate();
}
inline void serial_send(uint8_t* data, uint32_t length)
{
	sys->serial_send(data, length);
}
inline void serial_setup(uint8_t rx_pin, uint8_t tx_pin, uint32_t baudrate)
{
	sys->serial_setup(rx_pin, tx_pin, baudrate);
}
inline void spi_setup(spi_init_struct_t * spi_struct)
{
	sys->spi_setup(spi_struct);
}
inline void spi_transmit_receive(uint8_t * tx_buff, uint8_t * rx_buff, uint32_t buff_len)
{
	sys->spi_transmit_receive(tx_buff, rx_buff, buff_len);
}
inline void play_sound(uint8_t pin)
{
	sys->play_sound(pin);
}
inline void light_setup(uint8_t* pins, uint8_t is_active_high)
{
	sys->light_setup(pins, is_active_high);
}
inline uint32_t light_on(void)
{
	return sys->light_on();
}
inline void light_off(void)
{
	sys->light_off();
}
inline void light_set_color(const uint8_t* rgb_values)
{
	sys->light_set_color(rgb_values);
}
inline uint32_t data_storage_write(uint8_t data_id, uint8_t data_len, uint8_t * data)
{
	return sys->data_storage_write(data_id, data_len, data);
}
inline uint32_t data_storage_read(uint8_t data_id, uint8_t * data_len, uint8_t * data)
{
	return sys->data_storage_read(data_id, data_len, data);
}
inline uint32_t ble_infrared_config(ble_infrared_init_struct * Init_struct)
{
	return sys->ble_infrared_config(Init_struct);
}
inline uint32_t ble_infrared_send(const uint8_t * signal_data, uint8_t lenth)
{
	return sys->ble_infrared_send(signal_data, lenth);
}
inline uint32_t ble_infrared_get_statue(void)
{
	return sys->ble_infrared_get_statue();
}
inline void gfx_init_screen(uint8_t* buffer, uint16_t width, uint16_t height, uint8_t flags)
{
	sys->gfx_init_screen(buffer, width, height, flags);
}
inline void gfx_clear(void)
{
	sys->gfx_clear();
}
inline void gfx_draw_pixel(uint16_t x, uint16_t y, uint8_t ppo)
{
	sys->gfx_draw_pixel(x, y, ppo);
}
inline void gfx_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint8_t ppo)
{
	sys->gfx_draw_line(x0, y0, x1, y1, ppo);
}
inline void gfx_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t ppo)
{
	sys->gfx_draw_rect(x, y, w, h, ppo);
}
inline void gfx_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t ppo)
{
	sys->gfx_fill_rect(x, y, w, h, ppo);
}
inline void gfx_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint8_t ppo)
{
	sys->gfx_draw_circle(x, y, r, ppo);
}
inline void gfx_fill_circle(uint16_t x, uint16_t y, uint16_t r, uint8_t ppo)
{
	sys->gfx_fill_circle(x, y, r, ppo);
}
inline void gfx_set_font(const gfx_font_t* font)
{
	sys->gfx_set_font(font);
}
inline uint16_t gfx_draw_char(uint16_t x, uint16_t y, char c, uint8_t ppo)
{
	return sys->gfx_draw_char(x, y, c, ppo);
}
inline void gfx_draw_string(uint16_t x, uint16_t y, char* str, uint8_t ppo)
{
	sys->gfx_draw_string(x, y, str, ppo);
}
inline void gfx_draw_image(uint16_t x, uint16_t y, const uint8_t* image, uint16_t width, uint16_t height, uint8_t ppo)
{
	sys->gfx_draw_image(x, y, image, width, height, ppo);
}
inline uint32_t watchDog_Config(uint32_t juma_wdt_en)
{
	return sys->watchDog_Config(juma_wdt_en);
}
inline uint32_t watchDog_Start(uint32_t juma_wdt_timer_out_value)
{
	return sys->watchDog_Start(juma_wdt_timer_out_value);
}
inline void watchDog_user_dog1_RR(void)
{
	sys->watchDog_user_dog1_RR();
}
inline void watchDog_user_dog2_RR(void)
{
	sys->watchDog_user_dog2_RR();
}
inline void watchDog_user_dog3_RR(void)
{
	sys->watchDog_user_dog3_RR();
}
inline void watchDog_user_dog4_RR(void)
{
	sys->watchDog_user_dog4_RR();
}
