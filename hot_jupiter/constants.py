"""
Physical and Astronomical Constants (SI units unless specified otherwise).
"""

# Non-relativistic electron degeneracy constant for metallic hydrogen
# Calibrated to SCVH95 / CMS19 EOS tables (mean density ~1.326 g/cm^3 for Jupiter)
K_DEG = 1.08e6  # Pa m^5 kg^(-5/3)

# Gravitational Constant (m^3 kg^-1 s^-2)
G = 6.67430e-11

# Mass constants (kg)
M_JUP = 1.89813e27
M_EARTH = 5.9722e24
M_SUN = 1.98847e30

# Radius constants (m)
R_JUP = 7.1492e7
R_EARTH = 6.371e6
R_SUN = 6.957e8

# Solar Luminosity (W)
L_SUN = 3.828e26

# Astronomical Unit (m)
AU = 1.495978707e11

# Stefan-Boltzmann Constant (W m^-2 K^-4)
SIGMA_SB = 5.670374419e-8

# Thermodynamic constants
K_B = 1.380649e-23  # Boltzmann constant (J K^-1)
M_H = 1.6735575e-27  # Mass of Hydrogen atom (kg)
M_HE = 6.646476e-27  # Mass of Helium atom (kg)
N_A = 6.02214076e23  # Avogadro constant (mol^-1)
R_GAS = K_B * N_A  # Universal Gas Constant (J mol^-1 K^-1)

# Pressure units (Pa)
BAR = 1.0e5
MBAR = 1.0e11
GPa = 1.0e9

# Time units (s)
HOUR = 3600.0
DAY = 86400.0
YEAR = 3.15576e7
MYR = 1.0e6 * YEAR
GYR = 1.0e9 * YEAR
