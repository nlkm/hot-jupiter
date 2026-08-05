"""
Benchmark Example: Hot Jupiter Irradiated Cooling & Tidal Heating Inflation.
"""

import os
import matplotlib.pyplot as plt

from hot_jupiter.constants import M_JUP, M_EARTH, R_JUP, BAR, YEAR, AU, M_SUN
from hot_jupiter.eos import AnalyticalHHeEOS
from hot_jupiter.structure import InteriorSolver
from hot_jupiter.atmosphere import GuillotAtmosphere
from hot_jupiter.heating import TidalEccentricityHeating
from hot_jupiter.evolution import ThermalEvolutionIntegrator
from hot_jupiter.visualization import plot_evolution_track


def main():
    print("--- Simulating Hot Jupiter with Stellar Irradiation & Tidal Inflation ---")

    eos = AnalyticalHHeEOS()
    solver = InteriorSolver(envelope_eos=eos)
    atmosphere = GuillotAtmosphere(envelope_eos=eos)
    
    # Tidal dissipation heating model
    tidal_heating = TidalEccentricityHeating(
        M_star=1.0 * M_SUN,
        a=0.04 * AU,            # Close-in Hot Jupiter orbit
        eccentricity=0.04,
        k2_over_Q=2.0e-5,
    )

    integrator = ThermalEvolutionIntegrator(
        interior_solver=solver,
        atmosphere_model=atmosphere,
        heating_source=tidal_heating,
    )

    M_p = 1.2 * M_JUP
    M_c = 10.0 * M_EARTH
    S_initial = eos.specific_entropy(1.0 * BAR, 800.0)

    # Incident flux at 0.04 AU from Sun-like star: F_inc = L_sun / (4 * pi * a^2)
    a_orbit = 0.04 * AU
    F_inc = 3.828e26 / (4.0 * 3.14159 * a_orbit**2)  # ~ 1.9e5 W/m^2

    # Evolve from 1 Myr to 3.0 Gyr
    t_span = (1.0e6 * YEAR, 3.0e9 * YEAR)
    result = integrator.evolve(
        M_p=M_p,
        M_c=M_c,
        S_initial=S_initial,
        t_span=t_span,
        F_inc=F_inc,
        num_eval=15,
    )

    print(f"Initial Radius (1 Myr):  {result.R_p_jup[0]:.2f} R_Jup")
    print(f"Final Radius (3.0 Gyr):  {result.R_p_jup[-1]:.2f} R_Jup (Inflated Radius)")
    print(f"Final T_eff (3.0 Gyr):   {result.T_eff[-1]:.1f} K")
    print(f"Tidal Power Deposited:  {result.P_tidal[-1]:.2e} W")

    output_dir = "outputs"
    os.makedirs(output_dir, exist_ok=True)
    fig_evo = plot_evolution_track(
        result,
        title="Hot Jupiter Irradiated & Tidally Inflated Evolution Track",
        savepath=os.path.join(output_dir, "hot_jupiter_inflation_track.png"),
    )
    plt.close(fig_evo)
    print(f"Plot saved to {output_dir}/hot_jupiter_inflation_track.png.")


if __name__ == "__main__":
    main()
