"""
Benchmark Example: Thermal Evolution of Jupiter over 4.56 Billion Years.
"""

import os
import matplotlib.pyplot as plt

from thermal_evolution.constants import M_JUP, M_EARTH, R_JUP, BAR, YEAR, GYR
from thermal_evolution.eos import AnalyticalHHeEOS, TabularEOS
from thermal_evolution.structure import InteriorSolver
from thermal_evolution.atmosphere import GuillotAtmosphere
from thermal_evolution.heating import ZeroHeating
from thermal_evolution.evolution import ThermalEvolutionIntegrator
from thermal_evolution.visualization import plot_evolution_track, plot_internal_profile


def main():
    print("--- Simulating Jupiter Thermal Evolution (1 Myr to 4.56 Gyr) ---")

    # 1. Setup Tabular EOS using cached synthetic grid
    eos = TabularEOS.create_synthetic_grid()
    solver = InteriorSolver(envelope_eos=eos)
    atmosphere = GuillotAtmosphere(envelope_eos=eos)

    integrator = ThermalEvolutionIntegrator(
        interior_solver=solver,
        atmosphere_model=atmosphere,
        heating_source=ZeroHeating(),
    )

    # 2. Jupiter mass M_p = 1.0 M_Jup, core mass M_c = 10.0 M_Earth
    M_p = 1.0 * M_JUP
    M_c = 10.0 * M_EARTH

    # Initial high entropy at 1 Myr (T_1bar ~ 600 K)
    S_initial = eos.specific_entropy(1.0 * BAR, 600.0)

    # Evolve from 1 Myr to 4.56 Gyr
    t_span = (1.0e6 * YEAR, 4.56e9 * YEAR)
    result = integrator.evolve(
        M_p=M_p,
        M_c=M_c,
        S_initial=S_initial,
        t_span=t_span,
        num_eval=5,
    )

    print(f"Initial Radius (1 Myr):  {result.R_p_jup[0]:.2f} R_Jup")
    print(f"Final Radius (4.56 Gyr): {result.R_p_jup[-1]:.2f} R_Jup")
    print(f"Final T_eff (4.56 Gyr):  {result.T_eff[-1]:.1f} K")

    # 3. Save evolutionary track plot
    output_dir = "outputs"
    os.makedirs(output_dir, exist_ok=True)
    fig_evo = plot_evolution_track(
        result,
        title="Jupiter 4.56 Gyr Thermal Cooling Track",
        savepath=os.path.join(output_dir, "jupiter_cooling_track.png"),
    )
    plt.close(fig_evo)

    # 4. Compute final 1D interior profile at 4.56 Gyr
    final_struct = solver.solve_structure(M_p=M_p, M_c=M_c, S_env=result.S[-1])
    fig_prof = plot_internal_profile(
        final_struct,
        title="Jupiter Interior Hydrostatic Profile at 4.56 Gyr",
        savepath=os.path.join(output_dir, "jupiter_internal_profile.png"),
    )
    plt.close(fig_prof)

    print(f"Plots saved to {output_dir}/ directory.")


if __name__ == "__main__":
    main()
