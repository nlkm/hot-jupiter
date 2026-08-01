"""
Example script: Simulating Jupiter Thermal Cooling Track (1 Myr to 4.56 Gyr).
Outputs vector PDF plots for paper insertion.
"""

import os
import matplotlib.pyplot as plt

from thermal_evolution.constants import M_JUP, M_EARTH, BAR, YEAR
from thermal_evolution.eos import TabularEOS
from thermal_evolution.structure import InteriorSolver
from thermal_evolution.atmosphere import GuillotAtmosphere
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
    )

    # Jupiter parameters
    M_p = 1.0 * M_JUP
    M_c = 10.0 * M_EARTH

    # Initial specific entropy at ~600 K surface temperature
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

    output_dir = "outputs"
    os.makedirs(output_dir, exist_ok=True)

    # Save 4-panel cooling track vector PDF plot
    fig_track = plot_evolution_track(
        result,
        title="Jupiter Thermal Evolution Track (1 Myr to 4.56 Gyr)",
        savepath=os.path.join(output_dir, "jupiter_cooling_track.pdf"),
    )
    plt.close(fig_track)

    # Solve final 1D interior hydrostatic structure at present age
    final_struct = solver.solve_structure(M_p=M_p, M_c=M_c, S_env=result.S[-1])

    # Save 1D interior profile vector PDF plot at current epoch
    fig_prof = plot_internal_profile(
        final_struct,
        title="Jupiter Present-Day Hydrostatic Interior Profile",
        savepath=os.path.join(output_dir, "jupiter_internal_profile.pdf"),
    )
    plt.close(fig_prof)

    print("Plots saved to outputs/ directory.\n")


if __name__ == "__main__":
    main()
