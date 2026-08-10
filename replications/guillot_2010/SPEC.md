# Replication Specification: Guillot (2010)
**Title**: On the Radiative Equilibrium of Irradiated Planetary Atmospheres  
**Author**: Tristan Guillot  
**Journal**: Astronomy & Astrophysics (A&A), 520, A27 (2010) | **arXiv**: `1005.0371`

---

## Executive Summary & Core Equations

Guillot (2010) derives the exact analytical solution for double-gray 2-stream radiative equilibrium in irradiated exoplanet atmospheres.

### 1. Double-Gray Radiative Temperature Profile
$$T^4(\tau) = \frac{3 T_{\text{int}}^4}{4} \left(\tau + \frac{2}{3}\right) + \frac{3 T_{\text{eq}}^4}{4} \left[ \frac{2}{3} + \frac{1}{\gamma \sqrt{3}} + \left( \frac{\gamma}{\sqrt{3}} - \frac{1}{\gamma \sqrt{3}} \right) e^{-\gamma \tau \sqrt{3}} \right]$$

where $\gamma = \kappa_{\text{vis}} / \kappa_{\text{IR}}$.

---

## Benchmark Figures to Replicate

1. **Figure 1**: Atmospheric temperature $T(\tau)$ vs optical depth $\tau$ for $\gamma = 0.01, 0.1, 1.0, 10.0$.
2. **Figure 2**: $T(P)$ temperature-pressure profiles for HD 209458b ($T_{\text{eq}} = 1450\,\text{K}$) and HD 189733b ($T_{\text{eq}} = 1200\,\text{K}$).
