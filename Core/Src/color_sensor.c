#include "color_sensor.h"
#include "main.h"
#include <math.h>
#include <stdarg.h>
#include <stdio.h>

#define MIN_CLEAR 20u

void COLOR_SENSOR_Init(tcs34725_handle_t *handle) {
    tcs34725_set_power_on(handle, TCS34725_BOOL_TRUE);
    tcs34725_set_rgbc(handle, TCS34725_BOOL_TRUE);
    tcs34725_set_gain(handle, TCS34725_GAIN_4X);
    tcs34725_set_rgbc_integration_time(handle, TCS34725_INTEGRATION_TIME_101MS);
}

static ColorResult_t classify(float r, float g, float b) {
    float cmax = fmaxf(r, fmaxf(g, b));
    float cmin = fminf(r, fminf(g, b));
    float delta = cmax - cmin;

    if (cmax == 0.0f || delta / cmax < 0.15f) return COLOR_UNKNOWN;

    float hue;
    if      (cmax == r) hue = 60.0f * fmodf((g - b) / delta, 6.0f);
    else if (cmax == g) hue = 60.0f * (((b - r) / delta) + 2.0f);
    else                hue = 60.0f * (((r - g) / delta) + 4.0f);
    if (hue < 0.0f) hue += 360.0f;

    if (hue < 15.0f || hue >= 345.0f) return COLOR_RED;
    if (hue < 40.0f)                  return COLOR_ORANGE;
    if (hue < 75.0f)                  return COLOR_YELLOW;
    if (hue < 165.0f)                 return COLOR_GREEN;
    if (hue < 265.0f)                 return COLOR_BLUE;
    else                              return COLOR_PURPLE;
}

ColorResult_t COLOR_SENSOR_Read(tcs34725_handle_t *handle) {
    uint16_t r, g, b, c;
    uint8_t rgbc_read = tcs34725_read_rgbc(handle, &r, &g, &b, &c);
    if (rgbc_read != 0) return COLOR_UNKNOWN;
    if (c < MIN_CLEAR) return COLOR_UNKNOWN;

    float rf = fminf((float)r / c, 1.0f);
    float gf = fminf((float)g / c, 1.0f);
    float bf = fminf((float)b / c, 1.0f);

    COLOR_SENSOR_PrintRGB(r, g, b, classify(rf, gf, bf));  // moved to after rf/gf/bf
    return classify(rf, gf, bf);
}

// The driver expects this signature:
// uint8_t iic_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)

uint8_t my_i2c_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len) {
    HAL_StatusTypeDef result = HAL_I2C_Mem_Read(
        &hi2c1,          // your I2C handle — hi2c1, hi2c2, etc.
        addr,            // device address (driver already has TCS34725's address built in)
        reg,             // register to read from
        I2C_MEMADD_SIZE_8BIT,
        buf,             // buffer to read into
        len,             // number of bytes
        HAL_MAX_DELAY
    );
    return (result == HAL_OK) ? 0 : 1;  // driver expects 0=success, 1=fail
}

uint8_t my_i2c_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len) {
    HAL_StatusTypeDef result = HAL_I2C_Mem_Write(
        &hi2c1,
        addr,
        reg,
        I2C_MEMADD_SIZE_8BIT,
        buf,
        len,
        HAL_MAX_DELAY
    );
    return (result == HAL_OK) ? 0 : 1;
}

uint8_t my_i2c_init(void) {
    return 0;  // STM32 CubeMX already inits I2C before main() runs
}

uint8_t my_i2c_deinit(void) {
    return 0;  // leave it running
}

void my_delay_ms(uint32_t ms) {
    HAL_Delay(ms);
}

 void my_debug_print(const char *fmt, ...) {
    char buf[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), HAL_MAX_DELAY);
}

void COLOR_SENSOR_PrintRGB(uint16_t r, uint16_t g, uint16_t b, ColorResult_t color) {
    const char *color_names[] = {"RED", "ORANGE", "YELLOW", "GREEN", "BLUE", "PURPLE", "UNKNOWN"};
    char buf[64];
    snprintf(buf, sizeof(buf), "r: %u\ng: %u\nb: %u\ncolor: %s\n", r, g, b, color_names[color]);
    HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), HAL_MAX_DELAY);
}