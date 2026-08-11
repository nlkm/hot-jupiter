# Replication Specification: Fortney et al. (2020)
**Title**: Beyond the Standard Model: Climate Modeling of Ultra-Hot Jupiters with Thermal Dissociation  
**Authors**: J. J. Fortney, T. D. Robinson, et al.  
**Journal**: AJ, 160, 288 (2020) | **arXiv**: `2009.11725`

---

## Executive Summary & Core Equations

Fortney et al. (2020) model $H_2$ and $H_2O$ thermal dissociation kinetics and $H^-$ opacity continuum effects in ultra-hot Jupiter atmospheres.

### 1. Thermal Dissociation Fraction
$$\alpha_{\text{dissoc}}(T, P) = \left[1 + \frac{4 P}{K_p(T)}\right]^{-1/2}$$
$$T(P) = T_{\text{eq}} \left(\frac{P}{P_0}\right)^{\nabla_{\text{ad}} (1 - \gamma_{\text{dissoc}})}$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: $H_2$ dissociation fraction $\alpha_{\text{dissoc}}$ vs pressure $P$ ($10^{-4}$ to $10^2$ bar) for $T \in [2000, 2500, 3000, 3500]$ K.
2. **Figure 2**: Thermal profile $T(P)$ vs pressure $P$ ($10^{-5}$ to $10^2$ bar) with and without $H^-$ opacity continuum.
