"""
Hot Jupiter Coupled Thermal-Orbital-Spin Vector Evolution Benchmark.
Simulates coupled thermal contraction, tidal semi-major axis decay,
eccentricity circularization, and spin-orbit synchronization.
"""

import os

import numpy as np

from hot_jupiter.atmosphere import GuillotAtmosphere
from hot_jupiter.constants import AU, M_EARTH, M_JUP, M_SUN, YEAR
from hot_jupiter.eos import TabularEOS
from hot_jupiter.evolution import ThermalEvolutionIntegrator
from hot_jupiter.heating import TidalEccentricityHeating
from hot_jupiter.orbit import OrbitalState, SpinVectorState
from hot_jupiter.structure import InteriorSolver
from hot_jupiter.visualization import plot_coupled_orbital_spin_evolution


def run_coupled_hot_jupiter_benchmark():
    print(
        "=========================================================================="
    )
    print(
        "      COUPLED THERMAL, ORBITAL ELEMENT & SPIN VECTOR EVOLUTION            "
    )
    print(
        "=========================================================================="
    )

    # 1. Initialize EOS, interior solver, atmosphere model, and tidal heating
    eos = TabularEOS.create_synthetic_grid(use_cache=False)
    solver = InteriorSolver(envelope_eos=eos)
    atmosphere = GuillotAtmosphere(envelope_eos=eos)
    tidal_heating = TidalEccentricityHeating(k2_over_Q=1.0e-5)

    integrator = ThermalEvolutionIntegrator(
        interior_solver=solver,
        atmosphere_model=atmosphere,
        heating_source=tidal_heating,
    )

    # 2. Planet & System Parameters
    M_p = 1.0 * M_JUP
    M_c = 12.0 * M_EARTH
    M_star = 1.0 * M_SUN
    S_initial = 1.34e5

    # Initial Orbital State (Hot Jupiter with e = 0.25 at a = 0.04 AU)
    orbit_init = OrbitalState(a=0.04 * AU, e=0.25, inc=0.0)

    # Initial Spin Vector State (Rapid initial rotation P_rot = 10 hrs, obliquity = 15 deg)
    spin_init = SpinVectorState.from_period_hours(period_hrs=10.0,
                                                  obliquity_deg=15.0)

    # Incident stellar flux at 0.04 AU (~ 850 kW/m^2)
    F_inc_base = 8.5e5  # W/m^2
    A_b = 0.34  # Bond albedo

    t_span = (1.0e6 * YEAR, 4.56e9 * YEAR)

    print("Evolving planet system over 4.56 Gyr:")
    print(f"  Initial Radius:           {1.0:.2f} R_Jup (approx)")
    print(f"  Initial Semi-major Axis:  {orbit_init.a_au:.4f} AU")
    print(f"  Initial Eccentricity:     {orbit_init.e:.4f}")
    print(f"  Initial Rotation Period:  {spin_init.period_hours:.2f} hours")
    print(
        f"  Initial Obliquity:        {np.degrees(spin_init.obliquity):.1f} deg"
    )
    print(
        "--------------------------------------------------------------------------"
    )

    # 3. Execute Coupled Integrator
    res = integrator.evolve_coupled(
        M_p=M_p,
        M_c=M_c,
        S_initial=S_initial,
        orbital_state_initial=orbit_init,
        spin_state_initial=spin_init,
        M_star=M_star,
        k2_over_Q=1.0e-5,
        t_span=t_span,
        F_inc_base=F_inc_base,
        A_b=A_b,
        num_eval=10,
        method="RK23",
    )

    # 4. Present Present-Day Benchmark Metrics
    idx_final = -1
    print(
        "\n--------------------------------------------------------------------------"
    )
    print("COUPLED DYNAMICAL SYSTEM AT 4.56 GYR:")
    print(
        "--------------------------------------------------------------------------"
    )
    print(f"Final Age:                    {res.t_gyr[idx_final]:.2f} Gyr")
    print(f"Final Planet Radius R_p:       {res.R_p_jup[idx_final]:.3f} R_Jup")
    print(f"Final Semi-major Axis a:       {res.a_au[idx_final]:.4f} AU")
    print(f"Final Eccentricity e:          {res.e[idx_final]:.4f}")
    print(
        f"Final Rotation Period P_rot:   {res.P_rot_hrs[idx_final]:.2f} hours")
    print(
        f"Final Obliquity:               {res.obliquity_deg[idx_final]:.2f} deg"
    )
    print(
        f"Final 3D Spin Vector (X,Y,Z):  ({res.spin_x[idx_final]:.3e}, {res.spin_y[idx_final]:.3e}, {res.spin_z[idx_final]:.3e}) rad/s"
    )
    print(f"Final Tidal Dissipation Power: {res.P_tidal[idx_final]:.3e} W")
    print(
        "--------------------------------------------------------------------------"
    )

    # 5. Render & Save Vector PDF Figure
    os.makedirs("outputs", exist_ok=True)
    fig_path = "outputs/hot_jupiter_coupled_orbital_spin_evolution.pdf"
    plot_coupled_orbital_spin_evolution(
        result=res,
        title=
        "Hot Jupiter Coupled Thermal, Orbital Element & Spin Vector Evolution",
        savepath=fig_path,
    )
    print(f"\nVector PDF figure saved to {fig_path}.\n")


if __name__ == "__main__":
    run_coupled_hot_jupiter_benchmark()
