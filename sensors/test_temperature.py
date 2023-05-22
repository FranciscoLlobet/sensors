from ctypes import *

import numpy as np

lib = cdll.LoadLibrary('build/libtemperature.dll')
lib.calculate_resistance.argtypes = [c_int16]
lib.calculate_resistance.restype = c_uint16

lib.calculate_temperature.argtypes = [c_uint16]
lib.calculate_temperature.restype = c_int16

def calculate_resistance_scalar(temperature : float) -> float:
    return float(lib.calculate_resistance(c_int16(temperature*32)))/8192

def calculate_temperature_scalar(normalized_resistance : float) -> float:
    return float(lib.calculate_temperature(c_uint16(int(8192.0*normalized_resistance))))/float(32)

def calculate_temperature(normalized_resistance : np.array) -> np.array:
    return np.array([calculate_temperature_scalar(r) for r in normalized_resistance])

def calculate_resistance(temperature : np.array) -> np.array:
    return np.array([calculate_resistance_scalar(t) for t in temperature])

def calculate_resistance_reference(temperature : np.array) -> np.array:
    return 1 + 3.9083E-03 * temperature - 5.775E-07 * temperature**2


def test_resistance():
    test_temps : np.array = np.arange(0,100,1)
    test_reference = calculate_resistance_reference(test_temps)
    test_values = calculate_resistance(test_temps)
    
    assert( np.sqrt(np.mean( (test_reference - test_values)**2 )) < 0.005 )
    assert( np.mean(np.abs( test_reference - test_values )) < 0.004)
    assert( np.max(np.abs( test_reference - test_values )) < 0.008)
    
    #assert(1.39 == calculate_resistance(100))
    #assert(100.0 == calculate_temperature(1.3850))
    
def test_temperature():
    test_temps : np.array = np.arange(0,100,1)
    test_reference = calculate_resistance_reference(test_temps)
    
    test_values = calculate_temperature(test_reference)
    
    assert( np.sqrt(np.mean( (test_temps - test_values)**2 )) < 1.09)
    assert( np.mean(np.abs( test_temps - test_values )) < 1.0)
    assert( np.max(np.abs( test_temps - test_values )) < 2.0)