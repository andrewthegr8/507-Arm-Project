/*
tcs34725_init(&handle);
tcs34725_set_power_on(&handle, TCS34725_BOOL_TRUE);
tcs34725_set_rgbc(&handle, TCS34725_BOOL_TRUE);
tcs34725_set_rgbc_integration_time(&handle, some_time);
tcs34725_set_gain(&handle, some_gain);

tcs34725_read_rgbc(&handle, &r, &g, &b, &c);

The most important functions for you are probably:

init, set_power_on, set_rgbc, set_gain, set_rgbc_integration_time, and read_rgbc
*/

/* need to update these functions
static tcs34725_handle_t color_sensor_handle =
{
    .iic_init    = platform_i2c_init,
    .iic_deinit  = platform_i2c_deinit,
    .iic_read    = platform_i2c_read,
    .iic_write   = platform_i2c_write,
    .delay_ms    = platform_delay_ms,
    .debug_print = platform_debug_print,
    .inited      = 0
}; */