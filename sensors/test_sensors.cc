#include <gtest/gtest.h>

#include "temperature.h"



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