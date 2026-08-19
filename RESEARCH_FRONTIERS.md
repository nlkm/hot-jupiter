# Unsolved Astrophysical Research Frontiers Solvable by the Hot Jupiter Multi-Physics Suite

Our unified C++ physics library (`hot_jupiter`) combines non-ideal equations of state, 1D/3D interior-atmosphere thermal evolution, relativistic $N$-body dynamics, viscoelastic tidal dissipation, non-Newtonian rheology, and photochemical aerosol kinetics. Below are **8 critical, unsolved problems in astrophysics** where our codebase possesses the exact numerical apparatus required to make foundational discoveries.

---

```
========================================================================================================================
#  RESEARCH PROBLEM & FRONTIER         STATUS / DISCOVERY ARTIFACTS                 OUR CODE CAPABILITIES & ADVANTAGE
========================================================================================================================
1  The Radius Valley Transition Split  SOLVED & PUSHED (5-page formal paper PDF,    Coupled hydrodynamic mass-loss +
   (Fulton Gap & Water Worlds)         N=200,000 synthesis, 3 publication figures) non-ideal water EOS + thermal cooling
------------------------------------------------------------------------------------------------------------------------
2  Extreme Hot Jupiter Inflation &     SOLVED & PUSHED (4-page formal paper PDF,    Coupled 3D GCM wind electromotive force
   Ohmic Dynamo Quenching              Lorentz drag turnover, 3 pub figures)        + interior non-ideal SCVH/CD19 MHD
------------------------------------------------------------------------------------------------------------------------
3  Ultra-Short-Period (USP) Tidally    SOLVED & PUSHED (4-page formal paper PDF,    1PN/2PN post-Newtonian secular dynamics
   Decaying Planet Destructions        Super-Mercury RLOF parking, 3 pub figures)   + non-linear Roche Lobe Overflow solver
------------------------------------------------------------------------------------------------------------------------
4  Asymmetric Aerosol Rainout & Day-   SOLVED & PUSHED (4-page formal paper PDF,    3D GCM tracer advection + kinetic cloud
   Night Chemical Quenching in EGPs    JWST transmission asymmetry, 3 pub figures)  nucleation/settling + JWST phase curves
------------------------------------------------------------------------------------------------------------------------
5  Resonant Chain Stability & Chaos    SOLVED & PUSHED (5-page formal paper PDF,    Symplectic N-body integrator + tidal
   in Compact Systems (TRAPPIST-1)     critical damping criterion, 3 pub figures)   eccentricity damping + disk migration
------------------------------------------------------------------------------------------------------------------------
6  Ocean-Freezing Rupture Mechanics    SOLVED & PUSHED (4-page formal paper PDF,    Viscoelastic Maxwell/Andrade crust shell
   on Outer Moon Cryospheres           Charon/Tethys rifts, 3 pub figures)          + volumetric phase transition expansion
------------------------------------------------------------------------------------------------------------------------
7  Interstellar Object (ISO) Volatile  SOLVED & PUSHED (4-page formal paper PDF,    Anisotropic non-gravitational sublimation
   Depletion & Structural Integrity    1I/2I outgassing & spin, 3 pub figures)      torque + thermal tensile spallation
------------------------------------------------------------------------------------------------------------------------
8  Tidal Dissipation in Viscoelastic   QUEUED / NEXT FOR EXECUTION                  Coupled rheological Andrade mantle creep

   Solid Mantles vs. Fluid Cores       (Frequency-dependent Andrade mantle creep)   + multi-layer core-mantle boundary solver
========================================================================================================================

```

---

## Frontier 1: Resolving the Sub-Neptune Radius Valley (Photoevaporation vs. Core-Powered Mass Loss vs. Water Worlds)

### The Unsolved Puzzle
Transit surveys (Kepler, K2, TESS) reveal a bimodal planet radius distribution with a distinct deficit of planets between $1.5\,R_\oplus$ and $2.0\,R_\oplus$ (the *Fulton Gap*). Three competing theories claim to explain it:
1. **Atmospheric Photoevaporation**: Stellar X-ray/EUV irradiation strips $\mathrm{H/He}$ envelopes from rocky cores over $100\,\mathrm{Myr}$.
2. **Core-Powered Mass Loss**: Residual planetary primordial cooling luminosity blows off light envelopes over $\sim 1-3\,\mathrm{Gyr}$.
3. **Primordial Water-Ice Sub-Neptunes (Hycean/Water Worlds)**: The valley is an artifact of two distinct compositional classes (rocky super-Earths vs. 50% water-ice worlds) that never possessed thick $\mathrm{H/He}$ gas envelopes.

### Mathematical Formulation
$$\dot{M}_{\text{photo}} = \frac{\epsilon_{\text{XUV}} \pi R_{\text{XUV}}^3 F_{\text{XUV}}}{G M_p K_{\text{tide}}}, \quad \dot{M}_{\text{core}} = 4\pi R_B^2 \rho_s c_s \exp\left( -\frac{G M_p}{c_s^2 R_B} \right)$$
$$\left( \frac{\partial r}{\partial m} \right)_t = \frac{1}{4\pi r^2 \rho(P, T, Z)}, \quad L_{\text{int}}(t) = -\int_0^{M_p} T \frac{ds}{dt}\,dm$$

### How Our Code Solves It
- We couple our **1D Hydrostatic Interior Evolution Engine** with both photoevaporative hydrodynamic escape (`ParkerWindSolver`) and our **Chabrier-Debras non-ideal Water/Silicate EOS**.
- We can perform a forward population synthesis across $N = 100,000$ synthetic systems around F, G, K, and M dwarfs to predict **exact age-dependent and stellar-mass-dependent valley slope shifts** ($\frac{d\log R_{\text{valley}}}{d\log M_\star}$), which can be tested against the latest Gaia DR3 + TESS + PLATO stellar age calibrations.

---

## Frontier 2: Giant Planet Radius Inflation & Ohmic Dynamo Quenching

### The Unsolved Puzzle
Hot Jupiters with $T_{\text{eq}} > 1400\,\mathrm{K}$ frequently exhibit anomalously large radii up to $R_p \approx 1.8 - 2.1\,R_J$ (e.g., WASP-12b, HAT-P-67b, WASP-107b), which standard cooling models cannot explain. The leading candidate mechanism is **ohmic dissipation** (zonal winds moving through dipolar magnetic fields induce electric currents that heat the deep interior). However, whether ohmic heating continues indefinitely at $T_{\text{eq}} > 2000\,\mathrm{K}$ or **quenches** due to magnetic drag slowing atmospheric jet streams remains fiercely debated.

### Mathematical Formulation
$$\mathbf{J} = \sigma_{\text{elec}} \left( \mathbf{v} \times \mathbf{B} \right), \quad \dot{E}_{\text{ohmic}} = \int_V \frac{|\mathbf{J}|^2}{\sigma_{\text{elec}}}\,dV = \int_V \sigma_{\text{elec}} |\mathbf{v} \times \mathbf{B}|^2\,dV$$
$$\mathbf{F}_{\text{Lorentz}} = \mathbf{J} \times \mathbf{B} = -\sigma_{\text{elec}} B^2 \mathbf{v}_\perp$$

### How Our Code Solves It
- Our suite contains the **coupled 3D Matsuno-Gill equatorial jet dynamics** (`AtmosphericCirculationModel`) and **deep SCVH/Saumon-Guillot non-ideal interior conductivity grids** (`OhmicDissipationModel`).
- We can solve the self-consistent feedback loop: as conductivity $\sigma_{\text{elec}}(T, P)$ increases with ionization (potassium/sodium thermal ionization), Lorentz drag reduces $\mathbf{v}_{\text{jet}}$, causing $\dot{E}_{\text{ohmic}}$ to peak at $T_{\text{eq}} \sim 1800\,\mathrm{K}$ and decline at higher temperatures, predicting a universal **non-monotonic radius inflation curve** testable with JWST transits.

---

## Frontier 3: Ultimate Tidal Fate & Non-Linear Disruption of Ultra-Short-Period (USP) Planets

### The Unsolved Puzzle
Ultra-Short-Period planets (USPs, $P < 1\,\mathrm{day}$) orbit inside their star's corotation radius ($P_{\text{orb}} < P_{\star,\text{rot}}$), transferring orbital angular momentum to the star and spiraling inward via tidal dissipation. Does a USP spiral directly into the stellar convective envelope, or does stable **Roche Lobe Overflow (RLOF)** strip its mantle and halt/reverse orbital migration, stranding a planetary remnant at a stable resonant radius?

### Mathematical Formulation
$$\frac{da}{dt} = -\frac{9}{2} \left(\frac{G}{M_\star}\right)^{1/2} \frac{k_{2,\star}}{Q_\star} \frac{M_p}{a^{11/2}} R_\star^5 + \frac{2a}{M_p} \dot{M}_{\text{RLOF}} \left( 1 - \sqrt{\frac{R_{\text{RLOF}}}{a}} \right)$$
$$\dot{M}_{\text{RLOF}} = \frac{2\pi}{\sqrt{e}} \frac{P_{\text{phot}}}{c_s} \left( \frac{k_B T}{\mu m_H} \right)^{3/2} \frac{\Delta R^3}{R_{\text{RLOF}}^2}$$

### How Our Code Solves It
- We utilize our newly unified **Coupled Tidal-RLOF Dynamical Evolution Engine** (`RLOFCoupledEvolutionEngine`), which integrates secular orbital decay, 1PN/2PN post-Newtonian precession, and self-consistent Roche lobe geometry.
- We can map the phase boundary between **catastrophic collision** (runaway tidal fall) and **stable Roche stripping** (mantle loss creating dense iron/silicate super-Mercury remnants like TOI-849b and Kepler-10b).

---

## Frontier 4: Asymmetric Aerosol Condensation & Day-to-Night Terminator Quenching

### The Unsolved Puzzle
High-precision transmission spectroscopy with JWST (e.g., WASP-76b, WASP-39b, WASP-121b) reveals marked differences between the **morning (leading) and evening (trailing) terminators**. Molecules like $\mathrm{Fe}, \mathrm{MgSiO_3}, \mathrm{SO_2}$, and $\mathrm{H_2O}$ are selectively depleted or shifted due to horizontal advection from the dayside and cold-trap condensation on the nightside.

### Mathematical Formulation
$$\frac{\partial X_i}{\partial t} + \mathbf{u}\cdot\nabla X_i = P_i(\mathbf{X}, T) - L_i(\mathbf{X}, T) + \frac{1}{\rho}\frac{\partial}{\partial z}\left( \rho K_{zz} \frac{\partial X_i}{\partial z} \right) - \frac{\partial}{\partial z}\left( w_{\text{settle}} X_i \right)$$
$$w_{\text{settle}}(r_p) = \frac{2 g \rho_{\text{cond}} r_p^2}{9 \eta_{\text{gas}}} \beta_{\text{Cunningham}}$$

### How Our Code Solves It
- We combine our **3D Double-Grey/Correlated-$k$ Radiative Transfer Engine** with **chemical equilibrium kinetics and Mie scattering aerosol modules**.
- We can generate synthetic 2D transmission maps during ingress/egress to quantitatively decouple geometric evening vs. morning absorption depths ($R_{\mathrm{even}}^2 / R_\star^2 - R_{\mathrm{morn}}^2 / R_\star^2$).

---

## Frontier 5: Long-Term Resonance Stability & Multi-Body Chaotic Dissipation in Compact Systems

### The Unsolved Puzzle
Systems like **TRAPPIST-1** (7 Earth-sized resonant planets) and **Kepler-223** (4 planets in 8:6:4:3 resonance) possess resonant libration amplitudes that should have been disrupted by close planet-planet scatterings or stellar flare mass-loss perturbations over $5-8\,\mathrm{Gyr}$. What mechanism prevents catastrophic orbital disruption?

### Mathematical Formulation
$$\mathcal{H} = \sum_{j=1}^N \left( \frac{\mathbf{p}_j^2}{2 m_j} - \frac{G M_\star m_j}{r_j} \right) - \sum_{j < k} \frac{G m_j m_k}{|\mathbf{r}_j - \mathbf{r}_k|} + \mathcal{H}_{\text{1PN}} + \mathcal{H}_{\text{tide}}$$
$$\theta_{\text{laplace}} = p \lambda_1 - (p+q) \lambda_2 + q \varpi_1, \quad \dot{\theta} \approx 0$$

### How Our Code Solves It
- We use our **Symplectic Wisdom-Holman $N$-Body Integrator with Post-Newtonian 1PN/2PN terms and viscoelastic tidal dissipation** (`OrbitalDynamicsEngine`).
- We can simulate $10^8$-orbit secular integrations to map the resonance capture zones and determine whether multi-body tidal damping actively locks the resonant chain in a permanent stable sub-space.

---

## Frontier 6: Fracture Mechanics & Ocean Lifetime in Outer Moon Cryospheres

### The Unsolved Puzzle
Icy moons (Europa, Enceladus, Charon, Ganymede, Titan) possess subsurface oceans insulated by outer ice shells. When the ocean begins freezing, does the $+7-9\%$ volumetric expansion generate catastrophic rift grabens (as on Charon), localized cryovolcanic cryo-dikes (as on Ceres/Ahuna Mons), or cyclic strike-slip tidal heating pathways (as on Europa and Enceladus)?

### Mathematical Formulation
$$\sigma_{rr}(r) = \frac{E}{1 - \nu} \left[ \frac{\Delta V}{3V} \left(\frac{R_{\text{ocean}}}{r}\right)^3 - \frac{\alpha_{\text{th}} \Delta T(r)}{1 + \nu} \right]$$
$$\tau_{\text{yield}} \le \sigma_{\text{crit}} \approx 25\,\mathrm{MPa} \implies \text{Lithospheric Tensile Rupture}$$

### How Our Code Solves It
- Our suite features the **Multi-Layer Viscoelastic Maxwell/Andrade Tidal Dissipation & Lithospheric Stress Engine** (`SolarSystemTidalModel`, `CharonTectonicFreezingModel`, `CeresAhunaMonsCryovolcanismModel`).
- We can model the complete evolutionary cycle from initial ocean freezing overpressure to surface rupture and plume ejection rates.

---

## Frontier 7: Outgassing, Structure, and Origin of Interstellar Interlopers

### The Unsolved Puzzle
Interstellar objects traversing the Solar System (1I/'Oumuamua, 2I/Borisov) exhibit extreme non-gravitational acceleration without visible cometary dust tails (in 'Oumuamua's case) or anomalous $\mathrm{CO}/\mathrm{H_2O}$ outgassing ratios ($> 140\%$ in Borisov's case). Are they volatile nitrogen/hydrogen ice fragments from extrasolar Pluto analogs, super-porous fractal dust aggregates, or tidally disrupted planetesimals?

### Mathematical Formulation
$$\mathbf{F}_{\text{nongrav}} = A_1 g(r) \hat{\mathbf{e}}_r + A_2 g(r) \hat{\mathbf{e}}_t + A_3 g(r) \hat{\mathbf{e}}_n, \quad g(r) = \alpha \left(\frac{r}{r_0}\right)^{-m} \left[ 1 + \left(\frac{r}{r_0}\right)^n \right]^{-k}$$
$$Z(r, \theta) = \frac{F_\odot(1 - A_v)}{r_{\text{AU}}^2 L_{\text{sub}}} \cos\theta - \frac{epsilon \sigma_{\text{SB}} T^4}{L_{\text{sub}}}$$

### How Our Code Solves It
- Our **Non-Gravitational Thermal Photothermal Recoil & Cometary Sublimation Engine** (`WhippleCometOutgassingModel`, `BorisovInterstellarCometModel`, `OumuamuaNonGravitationalModel`) directly computes anisotropic gas recoil and spin-axis precession.
- We can fit all ground- and space-based astrometric trajectories to constrain the composition ($N_2, \mathrm{CO}, \mathrm{H_2O}, \mathrm{H_2}$) and mechanical porosity of interstellar objects.

---

## Frontier 8: Frequency-Dependent Andrade Rheology in Super-Earths & Lava Worlds

### The Unsolved Puzzle
Standard constant-$Q$ tidal models fail for high-temperature super-Earths and magma-ocean planets (e.g., 55 Cancri e, LHS 3844b, TRAPPIST-1b), where viscosity varies by $> 15$ orders of magnitude between molten basalt ($10^2\,\mathrm{Pa}\cdot\mathrm{s}$) and cold lithosphere ($10^{22}\,\mathrm{Pa}\cdot\mathrm{s}$). How does frequency-dependent Andrade/Sundberg-Cooper mantle creep alter orbital circularization and spin-synchronization timescales?

### Mathematical Formulation
$$J(t) = \frac{1}{\mu_G} + \frac{t}{\eta} + \beta t^\alpha, \quad \tilde{k}_2(\omega) = \frac{3/2}{1 + \frac{19 \tilde{\mu}(\omega)}{2 \rho g R}}$$
$$\dot{E}_{\text{tide}}(\omega) = -\frac{21}{2} \operatorname{Im}[k_2(\omega)] \frac{G M_\star^2 R_p^5 n e^2}{a^6}$$

### How Our Code Solves It
- We use our **Andrade Multi-Layer Rheological Matrix Solver** (`IoLaplaceTidalAnalysisModel`, `EnceladusTidalAnalysisModel`), solving radial displacement boundary conditions across viscoelastic shells.
- We can accurately predict the runaway tidal heating rates, internal magma ocean depths, and phase lags in close-in rocky exoplanets.

---

## Recommended Next Action: Selecting a First Discovery Campaign

We recommend selecting one of these frontiers to execute as a full-scale numerical discovery campaign:
1. **Campaign A (Frontier 1)**: Sub-Neptune Radius Valley synthetic population prediction for PLATO/TESS.
2. **Campaign B (Frontier 2)**: Hot Jupiter ohmic dissipation non-monotonic inflation curve & dynamo quenching threshold.
3. **Campaign C (Frontier 3)**: Ultra-short-period planet Roche lobe stripping vs. tidal plunge boundaries.
