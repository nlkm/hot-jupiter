# Replication Specification: Jackson et al. (2017)
**Title**: Orbital Decay and Roche Lobe Overflow of Ultra-Short-Period Planets  
**Publication**: The Astronomical Journal (AJ), 154, 77 (2017)  
**arXiv ID**: `1611.08272`

---

## 1. Physical Model & Governing Equations

Jackson et al. (2017) models the coupled orbital decay, Roche lobe overflow (RLOF) mass loss, and planetary radius contraction of ultra-short-period gas giant exoplanets ($a < 0.05\,\text{AU}$).

### A. Tidal Orbital Decay Rate
Assuming a circular orbit ($e = 0$) and a slowly rotating host star ($\Omega_\star \ll n_{\text{orb}}$), stellar tidal dissipation drives inward orbital migration:
$$\frac{\mathrm{d}a}{\mathrm{d}t} = -9 \left(\frac{k_{2,\star}}{Q_\star'}\right) \left(\frac{M_p}{M_\star}\right) \left(\frac{R_\star}{a}\right)^5 n_{\text{orb}} \, a$$
where $n_{\text{orb}} = \sqrt{G M_\star / a^3}$ is the mean motion, $M_\star = 1.0\,M_\odot$, $R_\star = 1.0\,R_\odot$, and $k_{2,\star}/Q_\star' = 2 \times 10^{-5}$.

### B. Roche Lobe Radius & Filling Factor
The volume-equivalent Roche lobe radius is given by Eggleton (1983):
$$R_{\text{Roche}} = a \, \frac{0.49 q^{2/3}}{0.6 q^{2/3} + \ln(1 + q^{1/3})}, \quad q \equiv \frac{M_p}{M_\star}$$
The Roche lobe filling factor is $f_{\text{fill}} \equiv R_p / R_{\text{Roche}}$.

### C. Hydrodynamic $L_1$ Mass Loss Rate
When $f_{\text{fill}} \ge 0.95$, mass loss occurs via the $L_1$ acoustic nozzle flow (Rappaport et al. 2013):
$$\frac{\mathrm{d}M_p}{\mathrm{d}t} = -\dot{M}_0 \exp\left[ \eta_{\text{rlof}} \left(\frac{R_p}{R_{\text{Roche}}} - 1\right) \right]$$
where $\dot{M}_0 = 10^{11}\,\text{kg/s}$ and $\eta_{\text{rlof}} = 4.0$.

### D. Planetary Radius Model
The total planet radius $R_p$ is determined by adding the core radius $R_c$ and envelope thickness $R_{\text{env}}$:
$$R_c = R_\oplus \left(\frac{M_c}{M_\oplus}\right)^{0.28}, \quad R_p = R_c + (R_{\text{Jup}} - R_c) \left(\frac{M_p - M_c}{M_{\text{Jup}} - M_c}\right)^{0.6}$$

---

## 2. Benchmark Figures to Replicate
1. **Figure 3**: 2D Bifurcation Survival Map ($M_{p,\text{init}}$ vs $a_{\text{init}}$).
   - Zone I (Red): Disrupted / Engulfed remnant ($M_{\text{rem}} = 0$).
   - Zone II (Yellow): Stagnated envelope-stripped core remnant ($M_{\text{rem}} = M_c$).
   - Zone III (Green): Non-overflow cooling planet ($f_{\text{fill}} < 0.95$).
   - Critical mass boundary scaling: $M_{\text{crit}}(a_{\text{init}}) \approx 0.50 (a_{\text{init}} / 0.018\,\text{AU})^{3.0}\,M_{\text{Jup}}$.
2. **Figure 6**: Time evolution of semi-major axis $a(t)$ and planetary mass $M_p(t)$ over 10 Gyr.
