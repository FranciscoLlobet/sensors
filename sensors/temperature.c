#ifdef __cplusplus
extern "C" {
#endif

#include "temperature.h"

#define COEFF_A ((float)3.9083E-03F)
#define COEFF_B ((float)-5.775E-07F)
#define COEFF_C ((float)-4.183E-12F)

#define NORM_COEFF_A ((float)1.019648F)
#define NORM_COEFF_B ((float)-4.62E-06F)

float calculate_resistance_given_temperature(float temperature_celsius)
{
    float r = 0.0F;

    r = COEFF_B * temperature_celsius;
    r = r + COEFF_A;
    r = r * temperature_celsius;
    r = r + 1.0F;

    return r;
}

uint16_t calculate_scaled_resistance_from_temperature(int16_t scaled_temperature)
{   
    double r = 0.0F;

    r = NORM_COEFF_B * (double)scaled_temperature;
    r = r + NORM_COEFF_A;
    r = r * (double)scaled_temperature;
    r = r + 8192.0F;

    return (uint16_t)r;
}

uint16_t calculate_resistance(int16_t scaled_temperature)
{
    int32_t r = 0;

    r = -1240*scaled_temperature; // Result is max 25Bit Value, shall be shifted by 27 Bits right
    r = (((uint32_t)(r + (int32_t)273709676)) >> 12); // Value is now 16 Bits
    r = (r * scaled_temperature) >> 16;
    r = r + 8192;

    return r;
}

uint16_t calculate_resistance_slope(int16_t scaled_temperature)
{
    int32_t r_slope = ((int32_t)273672960 - (int32_t)2480*(int32_t)scaled_temperature);

    return (uint32_t)r_slope >> 16; // 12 Bit value 
}

uint16_t perform_resistance_scaling(float normalized_resistance)
{
    return (uint16_t) (normalized_resistance * (float)RESISTANCE_SCALING_FACTOR_U16);
}

int16_t perform_temperature_scaling(float temperature_celsius)
{
    return (int16_t) (temperature_celsius * (float)TEMPERATURE_SCALING_FACTOR_U16);
}

int16_t calculate_temperature_step(int16_t initial_temperature, int16_t scaled_resistance)
{
    return initial_temperature - (((calculate_resistance(initial_temperature) - scaled_resistance) * 4096) / calculate_resistance_slope(initial_temperature));
}

int16_t calculate_temperature(int16_t scaled_resistance)
{
    int32_t temp = calculate_temperature_step(scaled_resistance - (int16_t)8192, scaled_resistance);
    return calculate_temperature_step(temp, scaled_resistance);
}

#ifdef __cplusplus
}
#endif


