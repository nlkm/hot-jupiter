#ifndef HOT_JUPITER_CONSTANTS_HPP
#define HOT_JUPITER_CONSTANTS_HPP

namespace hot_jupiter {

// Gravitational Constant [m^3 kg^-1 s^-2]
constexpr double G = 6.67430e-11;

// Astronomical & Planetary Constants
constexpr double M_SUN = 1.98847e30;    // Sun mass [kg]
constexpr double R_SUN = 6.95700e8;     // Sun radius [m]
constexpr double L_SUN = 3.82800e26;    // Sun luminosity [W]

constexpr double M_JUP = 1.89813e27;    // Jupiter mass [kg]
constexpr double R_JUP = 7.14920e7;     // Jupiter equatorial radius [m]

constexpr double M_EARTH = 5.97219e24;  // Earth mass [kg]
constexpr double R_EARTH = 6.37100e6;   // Earth radius [m]

constexpr double AU = 1.495978707e11;   // Astronomical Unit [m]
constexpr double BAR = 1.0e5;           // Bar [Pa]

// Time Constants
constexpr double DAY = 86400.0;         // Day [s]
constexpr double HOUR = 3600.0;         // Hour [s]
constexpr double YEAR = 31557600.0;     // Julian year [s]
constexpr double GYR = 1.0e9 * YEAR;    // Gigayear [s]

// Fundamental Constants
constexpr double KB = 1.380649e-23;     // Boltzmann constant [J/K]
constexpr double HBAR = 1.054571817e-34;// Reduced Planck constant [J s]
constexpr double MASS_E = 9.1093837015e-31;// Electron mass [kg]
constexpr double MASS_P = 1.67262192369e-27;// Proton mass [kg]
constexpr double SIGMA_SB = 5.670374419e-8;// Stefan-Boltzmann constant [W m^-2 K^-4]

} // namespace hot_jupiter

#endif // HOT_JUPITER_CONSTANTS_HPP
