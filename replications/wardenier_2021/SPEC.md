# Replication Specification: Wardenier et al. (2021)
**Title**: Deconstructing the Transmission Spectra of Hot Jupiters: Asymmetries and Thermal Profiles  
**Authors**: Joost P. Wardenier, Vivien Parmentier, et al.  
**Journal**: MNRAS, 506, 1258 (2021) | **arXiv**: `2105.02981`

---

## Executive Summary & Core Equations

Wardenier et al. (2021) model 3D morning-evening limb thermal and chemical asymmetries and their spectral manifestations in hot Jupiters like WASP-76b.

### 1. Limb Transmission Asymmetry
$$\left(\frac{R_p}{R_\star}\right)^2_{\text{m,e}}(\lambda) = \left(\frac{R_0}{R_\star}\right)^2 + \frac{2 R_0 H_{\text{m,e}}}{R_\star^2} \left[\tau_0 + \ln \kappa(\lambda, T_{\text{m,e}})\right]$$
$$\Delta \left(\frac{R_p}{R_\star}\right)^2(\lambda) = \left(\frac{R_p}{R_\star}\right)^2_{\text{evening}}(\lambda) - \left(\frac{R_p}{R_\star}\right)^2_{\text{morning}}(\lambda)$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: Morning vs evening limb transmission spectrum $(R_p/R_\star)^2$ vs wavelength $\lambda$ (0.3 to 5.0 $\mu$m).
2. **Figure 2**: Morning ($T_{\text{m}}$) vs evening ($T_{\text{e}}$) limb thermal profile $T(P)$ vs pressure $P$ ($10^{-5}$ to $1.0$ bar).
