# Replication Specification: Eggleton et al. (1998)
**Title**: Vector Formulation of Tidal Friction  
**Authors**: Peter P. Eggleton, Lev G. Kiseleva, Rosemary A. Hut  
**Journal**: The Astrophysical Journal (ApJ), 499, 853 (1998) | **arXiv**: `astro-ph/9804245`

---

## Executive Summary & Core Equations

Eggleton et al. (1998) present the vector formulation of equilibrium tides, expressing orbital element rates $\mathrm{d}a/\mathrm{d}t$, $\mathrm{d}\mathbf{e}/\mathrm{d}t$, and spin vector rates $\mathrm{d}\boldsymbol{\Omega}/\mathrm{d}t$ without small-eccentricity expansions.

### 1. Vector Eccentricity Rate
$$\frac{\mathrm{d}\mathbf{e}}{\mathrm{d}t} = -\gamma \left[ f_1(e) \mathbf{e} - f_2(e) \frac{\boldsymbol{\Omega}}{n_{\text{orb}}} \right]$$

where:
$$f_1(e) = (1 - e^2)^{-11/2} \left(1 + \frac{15}{4}e^2 + \frac{15}{8}e^4 + \frac{5}{64}e^6\right)$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: Eccentricity decay trajectory $e(t)$ over 1 Gyr.
2. **Figure 2**: Stellar spin obliquity angle $\theta(t)$ alignment.
3. **Figure 3**: Semi-major axis decay $a(t)$ vs time.
