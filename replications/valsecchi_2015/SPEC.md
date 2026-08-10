# Replication Specification: Valsecchi et al. (2015)
**Title**: Mass Loss and Evolution of Overfilling Gas Giants  
**Authors**: Francesca Valsecchi, Fred Rasio, Michael Rappaport  
**Journal**: The Astrophysical Journal (ApJ), 813, 101 (2015) | **arXiv**: `1506.03001`

---

## Executive Summary & Core Equations

Valsecchi et al. (2015) model the coupled tidal-orbital and mass-loss evolution of gas giant planets undergoing Roche lobe overflow.

### 1. Orbital Evolution with Mass Loss Angular Momentum Feedback
$$\frac{\dot{a}}{a} = -2 \frac{\dot{M}_p}{M_p} \left( 1 - \gamma - \frac{M_p}{2 M_\star} \right) - \frac{2}{\tau_a}$$

### 2. Planetary Roche Lobe Response
$$\zeta_{\text{RL}} = \frac{d \ln R_L}{d \ln M_p} = \frac{1}{3} + 2 \left( 1 - \gamma - \frac{M_p}{2 M_\star} \right)$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: Planetary radius $R_p(t)$ and Roche radius $R_L(t)$ [$R_{\text{Jup}}$] over 1 Gyr.
2. **Figure 2**: Semi-major axis evolution $a(t)$ [AU] showing RLOF orbital expansion.
