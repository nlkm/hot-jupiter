# Giant Planet Thermal Evolution Model

A modular numerical physics library for modeling the interior hydrostatic structure, atmospheric boundary conditions, tidal dissipation, and long-term cooling history of giant planets ($M_p \sim 0.1 - 10 M_{\text{Jup}}$).

---

## 🌟 Features

* **Modular Equation of State (EOS) Interface**:
  - `BaseEOS`: Abstract Python interface for Hydrogen-Helium envelope physics.
  - `TabularEOS`: High-performance 2D interpolator for tabular EOS data (SCVH 1995, CMS 2019). Includes built-in `create_synthetic_grid()` generator.
  - `AnalyticalHHeEOS`: Fast analytical gas EOS combining ideal gas mixture and non-relativistic electron degeneracy.
  - `BirchMurnaghanCoreEOS`: 3rd-order Birch-Murnaghan EOS for high-pressure heavy-element (rock/ice) cores.

* **1D Hydrostatic Interior Solver (`InteriorSolver`)**:
  - Integrates hydrostatic equilibrium outwards from central pressure $P_c$ to surface $P_{\text{surf}}$.
  - Calculates planet radius $R_p$, core radius $R_c$, core-envelope boundary pressure $P_{\text{cb}}$, internal energy $E_{\text{int}}$, gravitational potential energy $U$, and thermal capacity integral $\int_0^{M_p} T(m) dm$.

* **Atmospheric Boundary Models (`GuillotAtmosphere`)**:
  - Guillot (2010) semi-analytical irradiated radiative-convective atmosphere model.
  - Connects envelope specific entropy $S_{\text{env}}$ to intrinsic effective temperature $T_{\text{int}}$, total effective temperature $T_{\text{eff}}$, and net radiated power $L_{\text{int}}$.

* **Tidal Dissipation & Interior Heating (`TidalEccentricityHeating`)**:
  - Calculates power injection $P_{\text{tidal}}$ into the interior via orbital eccentricity damping.

* **Thermal Evolution Integrator (`ThermalEvolutionIntegrator`)**:
  - Solves the global energy conservation differential equation:
    $$\frac{dS}{dt} = -\frac{L_{\text{int}} - P_{\text{tidal}}}{\int_0^{M_p} T(m) dm}$$
  - Tracks $R_p(t)$, $L_{\text{int}}(t)$, $T_{\text{eff}}(t)$, $S(t)$ over gigayear timescales ($t \sim 10^6 - 10^{10} \text{ years}$).

---

## 🚀 Quickstart Example

```python
from hot_jupiter.constants import M_JUP, M_EARTH, BAR, YEAR
from hot_jupiter.eos import TabularEOS
from hot_jupiter.structure import InteriorSolver
from hot_jupiter.atmosphere import GuillotAtmosphere
from hot_jupiter.heating import ZeroHeating
from hot_jupiter.evolution import ThermalEvolutionIntegrator
from hot_jupiter.visualization import plot_evolution_track

# 1. Initialize EOS, Hydrostatic Solver, and Atmosphere
eos = TabularEOS.create_synthetic_grid(n_P=100, n_T=100)
solver = InteriorSolver(envelope_eos=eos)
atmosphere = GuillotAtmosphere(envelope_eos=eos)

integrator = ThermalEvolutionIntegrator(
    interior_solver=solver,
    atmosphere_model=atmosphere,
    heating_source=ZeroHeating(),
)

# 2. Define Planet Parameters (1 Jupiter Mass, 10 Earth Mass Core)
M_p = 1.0 * M_JUP
M_c = 10.0 * M_EARTH
S_initial = eos.specific_entropy(1.0 * BAR, 600.0)

# 3. Evolve Planet Cooling over 4.56 Gyr
result = integrator.evolve(
    M_p=M_p,
    M_c=M_c,
    S_initial=S_initial,
    t_span=(1.0e6 * YEAR, 4.56e9 * YEAR),
    num_eval=15,
)

print(f"Initial Radius (1 Myr):  {result.R_p_jup[0]:.2f} R_Jup")
print(f"Final Radius (4.56 Gyr): {result.R_p_jup[-1]:.2f} R_Jup")

# 4. Plot Evolutionary Track
fig = plot_evolution_track(result, title="Jupiter 4.56 Gyr Cooling Track")
fig.savefig("jupiter_cooling_track.png")
```

---

## 🛠 Running Example Scripts & Tests

```bash
# Run unit tests
python3 -m pytest tests/

# Run Jupiter cooling benchmark
python3 examples/jupiter_cooling.py

# Run Hot Jupiter tidal inflation benchmark
python3 examples/hot_jupiter_inflation.py
```
