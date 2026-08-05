"""
High Obliquity & Asynchronous Initial Spin Scenario Benchmark.
Demonstrates the critical impact of initial spin axis tilt (obliquity epsilon_0 = 45 deg)
and fast initial rotation (P_rot = 6 hours) on tidal dissipation power and radius evolution.
"""

import os
import numpy as np

from hot_jupiter.constants import M_JUP, M_EARTH, M_SUN, AU, YEAR, GYR, HOUR, DAY
from hot_jupiter.eos import TabularEOS
from hot_jupiter.structure import InteriorSolver
from hot_jupiter.atmosphere import GuillotAtmosphere
from hot_jupiter.heating import TidalEccentricityHeating
from hot_jupiter.orbit import OrbitalState, SpinVectorState
from hot_jupiter.evolution import ThermalEvolutionIntegrator
from hot_jupiter.visualization import plot_coupled_orbital_spin_evolution


def run_obliquity_scenario():
    print("==========================================================================")
    print("   HIGH OBLIQUITY TILT & ASYNCHRONOUS INITIAL SPIN SCENARIO BENCHMARK    ")
    print("==========================================================================")

    # 1. Initialize models
    eos = TabularEOS.create_synthetic_grid(use_cache=False)
    solver = InteriorSolver(envelope_eos=eos)
    atmosphere = GuillotAtmosphere(envelope_eos=eos)
    tidal_heating = TidalEccentricityHeating(k2_over_Q=2.0e-5)

    integrator = ThermalEvolutionIntegrator(
        interior_solver=solver,
        atmosphere_model=atmosphere,
        heating_source=tidal_heating,
    )

    # 2. Case A: Tilted Spin Vector (obliquity = 45 deg, P_rot = 6.0 hrs, e = 0.10)
    print("Case A: High Obliquity Spin Tilt (epsilon_0 = 45.0 deg, P_rot = 6.0 hrs, e = 0.10)")
    orbit_A = OrbitalState(a=0.04 * AU, e=0.10)
    spin_A = SpinVectorState.from_period_hours(period_hrs=6.0, obliquity_deg=45.0)

    res_A = integrator.evolve_coupled(
        S_initial=1.34e5,
        M_p=1.0 * M_JUP,
        M_c=12.0 * M_EARTH,
        M_star=1.0 * M_SUN,
        orbital_state_initial=orbit_A,
        spin_state_initial=spin_A,
        k2_over_Q=2.0e-5,
        t_span=(1.0e6 * YEAR, 4.56e9 * YEAR),
        num_eval=10,
    )

    print(f"  Final Radius R_p:           {res_A.R_p_jup[-1]:.3f} R_Jup")
    print(f"  Final Semi-major Axis a:    {res_A.a_au[-1]:.4f} AU")
    print(f"  Final Eccentricity e:       {res_A.e[-1]:.4f}")
    print(f"  Final Rotation Period:      {res_A.P_rot_hrs[-1]:.2f} hours")
    print(f"  Final Obliquity:            {res_A.obliquity_deg[-1]:.2f} deg")

    # 3. Render Vector PDF Figure
    os.makedirs("outputs", exist_ok=True)
    fig_path = "outputs/obliquity_tilted_spin_evolution.pdf"
    plot_coupled_orbital_spin_evolution(
        result=res_A,
        title="High Initial Obliquity Tilt (45 deg) & Asynchronous Spin Evolution",
        savepath=fig_path,
    )
    print(f"\nVector PDF figure saved to {fig_path}.\n")


if __name__ == "__main__":
    run_obliquity_scenario()
