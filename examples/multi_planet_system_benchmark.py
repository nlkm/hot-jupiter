"""
Multi-Planet System Coupled Thermal & Dynamical Evolution Benchmark.
Simulates a 3-planet system (b, c, d) with coupled 1D thermal contraction,
tidal dissipation, and Laplace-Lagrange secular planet-planet gravitational interactions.
"""

import os
import numpy as np

from hot_jupiter.constants import M_JUP, M_EARTH, M_SUN, AU, YEAR, GYR
from hot_jupiter.eos import TabularEOS
from hot_jupiter.structure import InteriorSolver
from hot_jupiter.atmosphere import GuillotAtmosphere
from hot_jupiter.heating import TidalEccentricityHeating
from hot_jupiter.orbit import OrbitalState, SpinVectorState, PlanetSystemMember, MultiPlanetSystem
from hot_jupiter.evolution import ThermalEvolutionIntegrator
from hot_jupiter.visualization import plot_multi_planet_system_evolution


def run_multi_planet_system_benchmark():
    print("==========================================================================")
    print("       MULTI-PLANET SYSTEM COUPLED DYNAMICAL BENCHMARK VALIDATION         ")
    print("==========================================================================")

    # 1. Initialize EOS, interior solver, and atmosphere model
    eos = TabularEOS.create_synthetic_grid(use_cache=False)
    solver = InteriorSolver(envelope_eos=eos)
    atmosphere = GuillotAtmosphere(envelope_eos=eos)
    tidal_heating = TidalEccentricityHeating(k2_over_Q=1.0e-5)

    integrator = ThermalEvolutionIntegrator(
        interior_solver=solver,
        atmosphere_model=atmosphere,
        heating_source=tidal_heating,
    )

    # 2. Define 3-Planet System ("Kepler-TE-1")
    system = MultiPlanetSystem(name="Kepler-TE-1", M_star=1.0 * M_SUN, Fe_H=0.1)

    # Planet b: Hot Jupiter (1.0 M_J, a = 0.05 AU, e = 0.15)
    pb = PlanetSystemMember(
        name="b",
        M_p=1.0 * M_JUP,
        M_c=12.0 * M_EARTH,
        S_initial=1.34e5,
        orbital_state=OrbitalState(a=0.05 * AU, e=0.15),
        spin_state=SpinVectorState.from_period_hours(period_hrs=10.0, obliquity_deg=5.0),
        k2_over_Q=1.0e-5,
    )

    # Planet c: Warm Saturn (0.3 M_J, a = 0.18 AU, e = 0.10)
    pc = PlanetSystemMember(
        name="c",
        M_p=0.3 * M_JUP,
        M_c=8.0 * M_EARTH,
        S_initial=1.30e5,
        orbital_state=OrbitalState(a=0.18 * AU, e=0.10),
        spin_state=SpinVectorState.from_period_hours(period_hrs=12.0, obliquity_deg=10.0),
        k2_over_Q=1.0e-5,
    )

    # Planet d: Cold Giant (0.8 M_J, a = 1.20 AU, e = 0.05)
    pd = PlanetSystemMember(
        name="d",
        M_p=0.8 * M_JUP,
        M_c=15.0 * M_EARTH,
        S_initial=1.28e5,
        orbital_state=OrbitalState(a=1.20 * AU, e=0.05),
        spin_state=SpinVectorState.from_period_hours(period_hrs=9.5, obliquity_deg=3.0),
        k2_over_Q=1.0e-5,
    )

    system.add_planet(pb)
    system.add_planet(pc)
    system.add_planet(pd)

    print(f"System '{system.name}' (M_star = {system.M_star/1.988e30:.2f} M_sun, [Fe/H] = {system.Fe_H:+.2f}):")
    for p in system.planets:
        print(f"  Planet {p.name}: M_p = {p.M_p/1.898e27:4.2f} M_J, a = {p.orbital_state.a_au:5.2f} AU, e = {p.orbital_state.e:.2f}")

    print("\nEvolving system over 4.56 Gyr with Laplace-Lagrange secular perturbations...")
    t_span = (1.0e6 * YEAR, 4.56e9 * YEAR)

    # 3. Execute Multi-Planet Integrator
    res = integrator.evolve_multi_planet_system(
        system=system,
        t_span=t_span,
        num_eval=10,
        method="RK23",
    )

    # 4. Print Present-Day Metrics at 4.56 Gyr
    idx_final = -1
    print("\n--------------------------------------------------------------------------")
    print("PRESENT-DAY MULTI-PLANET SYSTEM METRICS AT 4.56 GYR:")
    print("--------------------------------------------------------------------------")
    for name in res.planet_names:
        print(f"Planet {name}:")
        print(f"  Radius R_p:           {res.R_p_jup[name][idx_final]:.3f} R_Jup")
        print(f"  Semi-major Axis a:    {res.a_au[name][idx_final]:.4f} AU")
        print(f"  Eccentricity e:       {res.e[name][idx_final]:.4f}")
        print(f"  Rotation Period:      {res.P_rot_hrs[name][idx_final]:.2f} hours")
        print(f"  Effective Temp T_eff: {res.T_eff[name][idx_final]:.1f} K")
        print("--------------------------------------------------------------------------")

    # 5. Render & Save Vector PDF Figure
    os.makedirs("outputs", exist_ok=True)
    fig_path = "outputs/multi_planet_system_evolution.pdf"
    plot_multi_planet_system_evolution(
        result=res,
        title=f"Coupled Multi-Planet System Evolution ({system.name})",
        savepath=fig_path,
    )
    print(f"\nVector PDF figure saved to {fig_path}.\n")


if __name__ == "__main__":
    run_multi_planet_system_benchmark()
