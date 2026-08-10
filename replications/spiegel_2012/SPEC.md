# Replication Specification: Spiegel & Burrows (2012)
**Title**: Spectral and Thermal Implications of Thermal Inversions and Cloud Stratification in Exoplanet Atmospheres  
**Authors**: David S. Spiegel, Adam Burrows  
**Journal**: The Astrophysical Journal (ApJ), 745, 174 (2012) | **arXiv**: `1108.5172`

---

## Executive Summary & Core Equations

Spiegel & Burrows (2012) analyze atmospheric thermal inversions caused by optical absorbers such as gaseous TiO and VO.

### 1. Two-Gray Optical Depth $T(P)$ Temperature Inversion Criterion
$$T^4(\tau) = \frac{3 T_{\text{int}}^4}{4} \left( \tau + \frac{2}{3} \right) + \frac{3 T_{\text{eq}}^4}{4} \left[ \frac{2}{3} + \gamma \left( 1 + \frac{1}{2 \gamma^2} E_2(\gamma \tau) \right) \right]$$
Thermal inversions ($dT/dP < 0$) occur when $\gamma = \kappa_{\text{vis}} / \kappa_{\text{IR}} > 1$.

---

## Benchmark Figures to Replicate

1. **Figure 1**: Pressure-Temperature $T(P)$ profiles for inverted ($\gamma = 2.0$, TiO present) vs non-inverted ($\gamma = 0.1$, no TiO) atmospheres.
2. **Figure 2**: Emission flux spectrum $F_\lambda(\lambda)$ [$\mu$m] demonstrating emission lines vs absorption lines.
