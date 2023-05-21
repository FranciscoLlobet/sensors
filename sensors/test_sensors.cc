#include <gtest/gtest.h>

extern "C" {

#include <stdint.h>

#define COEFF_A ((float)3.9083E-03F)
#define COEFF_B ((float)-5.775E-07F)
#define COEFF_C ((float)-4.183E-12F)

#define NORM_COEFF_A ((float)1.019648F)
#define NORM_COEFF_B ((float)-4.62E-06F)

float calculate_resistance_given_temperature(float temperature_celsius)
{
    float normalized_resistance = 0.0F;

    /* Horner Schema: 1 + A * (T (1 + B * T)*/
    normalized_resistance = COEFF_B * temperature_celsius;
    normalized_resistance = normalized_resistance + COEFF_A;
    normalized_resistance = normalized_resistance * temperature_celsius;
    normalized_resistance = normalized_resistance + 1.0F;

    return normalized_resistance;
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


#define RESISTANCE_SCALING_FACTOR_U16   ((uint16_t)0x2000) /* 8192 scales to 1.00 */
#define RESISTANCE_FRACTIONAL_MASK      ((uint16_t)0x1FFF) 
#define RESISTANCE_INTEGER_MASK         ((uint16_t)0xE000)

#define TEMPERATURE_SCALING_FACTOR_U16 ((int16_t)0x20)

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
    return calculate_temperature_step(scaled_resistance - (int16_t)8192, scaled_resistance);
}

}



TEST(hello_world, resistance_calculator)
{
    EXPECT_NEAR(calculate_resistance_given_temperature(0.0F), 1.000F, 0.0005);
    EXPECT_NEAR(calculate_resistance_given_temperature(10.0F), 1.039, 0.0005);
    EXPECT_NEAR(calculate_resistance_given_temperature(100.0F), 1.385, 0.0005);
    EXPECT_NEAR(calculate_resistance_given_temperature(850.0F), 3.905, 0.0005); /* Expected max Temperature is 850ºC */
}


TEST(hello_world, test_resistance_scaling)
{
    EXPECT_EQ(perform_resistance_scaling(1.00F), RESISTANCE_SCALING_FACTOR_U16);
    EXPECT_EQ(perform_resistance_scaling(4.00F), 0x8000);

    EXPECT_EQ(perform_resistance_scaling(3.905F), 31989); /* Resistance at 850 degC */
    EXPECT_EQ(perform_resistance_scaling(1.039F), 8511); /* Resistance at 10 degC */
    EXPECT_EQ(perform_resistance_scaling(1.385F), 11345); /* Resistance at 100 degC*/
}

TEST(hello_world, test_temperature_scaling)
{
    EXPECT_EQ(perform_temperature_scaling(-1.00F), (-1)*TEMPERATURE_SCALING_FACTOR_U16);
    EXPECT_EQ(perform_temperature_scaling(1.00F), TEMPERATURE_SCALING_FACTOR_U16);
    EXPECT_EQ(perform_temperature_scaling(100.00F), 100*TEMPERATURE_SCALING_FACTOR_U16);
    EXPECT_EQ(perform_temperature_scaling(850.0F), 27200);
}

TEST(hello_world, calculate_scaled_resistance_from_temperature)
{
    EXPECT_EQ(calculate_scaled_resistance_from_temperature(0), 8192); 
    EXPECT_EQ(calculate_scaled_resistance_from_temperature(perform_temperature_scaling(100.00F)), 11407); // 11345 (0.5% Error)
}

TEST(hello_world, calculate_resistance)
{
    EXPECT_EQ(calculate_resistance(0), 8192);
    EXPECT_EQ(calculate_resistance(perform_temperature_scaling(10.00F)), 8517); // 8511 (0.07% Error)
    EXPECT_EQ(calculate_resistance(perform_temperature_scaling(100.00F)), 11407); // 11345 (0.5% Error)
}

TEST(hello_world, calculate_temperature_step)
{
    /* Test Invariant property */
    EXPECT_EQ(calculate_temperature_step(0, 8192), 0);
    EXPECT_EQ(calculate_temperature_step(27200, 31989), 26525);

}

TEST(hello_world, calculate_temperature)
{
    EXPECT_EQ(calculate_temperature(8192), 0);
    EXPECT_EQ(calculate_temperature(31989), 26526);
}