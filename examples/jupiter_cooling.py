"""
Jupiter Benchmark Validation Study.
Simulates 1D Hydrostatic Thermal Evolution of Jupiter from 1 Myr to 4.56 Gyr.
Compares present-day model output against Juno & Voyager observational measurements:
  - Radius: R_p(4.56 Gyr) ~ 1.00 R_Jup
  - Effective Temperature: T_eff ~ 124.4 K
  - Intrinsic Temperature: T_int ~ 99.6 K
"""

import os

import matplotlib.pyplot as plt

from hot_jupiter.atmosphere import GuillotAtmosphere
from hot_jupiter.constants import M_EARTH, M_JUP, YEAR
from hot_jupiter.eos import TabularEOS
from hot_jupiter.evolution import ThermalEvolutionIntegrator
from hot_jupiter.heating import RadiogenicHeating
from hot_jupiter.structure import InteriorSolver
from hot_jupiter.visualization import plot_evolution_track, plot_internal_profile


def main():
    print("==========================================================")
    print("      JUPITER THERMAL EVOLUTION BENCHMARK VALIDATION      ")
    print("==========================================================")

    # 1. Initialize tabular EOS grid matching SCVH metallic hydrogen density
    eos = TabularEOS.create_synthetic_grid(use_cache=False)
    solver = InteriorSolver(envelope_eos=eos)
    atmosphere = GuillotAtmosphere(envelope_eos=eos)

    # Core radiogenic heating from 12 Earth-mass rocky core
    heating = RadiogenicHeating(M_c=12.0 * M_EARTH)

    integrator = ThermalEvolutionIntegrator(
        interior_solver=solver,
        atmosphere_model=atmosphere,
        heating_source=heating,
    )

    # Jupiter Physical Parameters
    M_p = 1.0 * M_JUP
    M_c = 12.0 * M_EARTH  # 12 Earth-mass core (matching Juno gravimetry estimates)

    # Initial entropy at 1 Myr matching Jupiter's thermal adiabat
    S_initial = 1.34e5

    # Jupiter solar insolation at 5.204 AU: F_inc = L_sun / (4 * pi * a^2) ~ 50.3 W/m^2
    F_inc_jupiter = 50.3  # W/m^2
    A_b_jupiter = 0.34  # Bond albedo of Jupiter

    # Evolve from t = 1 Myr to t = 4.56 Gyr (Jupiter's present age)
    t_span = (1.0e6 * YEAR, 4.56e9 * YEAR)

    print(
        f"Evolving planet structure (M_p = 1.00 M_J, M_c = {M_c/M_EARTH:.1f} M_Earth, a = 5.20 AU)..."
    )
    result = integrator.evolve(
        M_p=M_p,
        M_c=M_c,
        S_initial=S_initial,
        t_span=t_span,
        F_inc=F_inc_jupiter,
        A_b=A_b_jupiter,
        num_eval=8,
        method="RK23",
    )

    # Extract present-day values at 4.56 Gyr
    R_final_jup = result.R_p_jup[-1]
    T_eff_final = result.T_eff[-1]
    T_int_final = result.T_int[-1]
    L_int_final_sun = result.L_int_sun[-1]
    S_final = result.S[-1]

    print("\n----------------------------------------------------------")
    print("PRESENT-DAY JUPITER MODEL vs OBSERVATIONAL MEASUREMENTS:")
    print("----------------------------------------------------------")
    print(f"Age:                    {result.t_gyr[-1]:.2f} Gyr")
    print(
        f"Radius R_p:             {R_final_jup:.3f} R_Jup  (Observed: 1.000 R_Jup)"
    )
    print(
        f"Effective Temp T_eff:   {T_eff_final:.1f} K       (Observed: 124.4 +/- 0.3 K)"
    )
    print(
        f"Intrinsic Temp T_int:   {T_int_final:.1f} K        (Observed:  99.6 +/- 3.0 K)"
    )
    print(
        f"Intrinsic Luminosity:   {L_int_final_sun:.2e} L_sun (Observed: 8.7e-10 L_sun)"
    )
    print(f"Final Entropy S:        {S_final:.2e} J/(kg K)")
    print("----------------------------------------------------------\n")

    output_dir = "outputs"
    os.makedirs(output_dir, exist_ok=True)

    # Save 4-panel evolutionary track vector PDF plot
    fig_track = plot_evolution_track(
        result,
        title="Jupiter Thermal Evolution Track (1 Myr to 4.56 Gyr)",
        savepath=os.path.join(output_dir, "jupiter_cooling_track.pdf"),
    )
    plt.close(fig_track)

    # Solve 1D interior hydrostatic structure at 4.56 Gyr
    final_struct = solver.solve_structure(M_p=M_p, M_c=M_c, S_env=S_final)

    # Save 1D interior profile vector PDF plot at 4.56 Gyr
    fig_prof = plot_internal_profile(
        final_struct,
        title="Jupiter Present-Day Hydrostatic Interior Profile",
        savepath=os.path.join(output_dir, "jupiter_internal_profile.pdf"),
    )
    plt.close(fig_prof)

    print(f"Vector PDF figures saved to {output_dir}/.")


if __name__ == "__main__":
    main()
