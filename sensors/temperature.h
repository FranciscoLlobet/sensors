#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @def RESISTANCE_SCALING_FACTOR_U16
 * Scaling factor for resistance. 8192 scales to 1.00.
 */
#define RESISTANCE_SCALING_FACTOR_U16   ((uint16_t)0x2000)

/**
 * @def RESISTANCE_FRACTIONAL_MASK
 * Fractional mask for resistance calculation.
 */
#define RESISTANCE_FRACTIONAL_MASK      ((uint16_t)0x1FFF) 

/**
 * @def RESISTANCE_INTEGER_MASK
 * Integer mask for resistance calculation.
 */
#define RESISTANCE_INTEGER_MASK         ((uint16_t)0xE000)

/**
 * @def TEMPERATURE_SCALING_FACTOR_U16
 * Scaling factor for temperature.
 */
#define TEMPERATURE_SCALING_FACTOR_U16  ((int16_t)0x20)

/**
 * @brief Calculate resistance based on temperature in Celsius.
 *
 * @param temperature_celsius Temperature in Celsius.
 * @return Normalized resistance.
 */
float calculate_resistance_given_temperature(float temperature_celsius);

/**
 * @brief Calculate scaled resistance from temperature.
 *
 * @param scaled_temperature Scaled temperature.
 * @return Scaled resistance.
 */
uint16_t calculate_scaled_resistance_from_temperature(int16_t scaled_temperature);

/**
 * @brief Calculate resistance based on scaled temperature.
 *
 * @param scaled_temperature Scaled temperature.
 * @return Calculated resistance.
 */
uint16_t calculate_resistance(int16_t scaled_temperature);


/**
 * @brief Perform resistance scaling on normalized resistance.
 *
 * @param normalized_resistance Normalized resistance.
 * @return Scaled resistance.
 */
uint16_t perform_resistance_scaling(float normalized_resistance);

/**
 * @brief Perform temperature scaling on temperature in Celsius.
 *
 * @param temperature_celsius Temperature in Celsius.
 * @return Scaled temperature.
 */
int16_t perform_temperature_scaling(float temperature_celsius);

/**
 * @brief Calculate the next temperature step.
 *
 * @param initial_temperature Initial temperature for the step.
 * @param scaled_resistance Scaled resistance for the step.
 * @return Temperature step.
 */
int16_t calculate_temperature_step(int16_t initial_temperature, int16_t scaled_resistance);

/**
 * @brief Calculate the temperature based on scaled resistance.
 *
 * @param scaled_resistance Scaled resistance.
 * @return Calculated temperature.
 */
int16_t calculate_temperature(uint16_t scaled_resistance);

#ifdef __cplusplus
}
#endif
