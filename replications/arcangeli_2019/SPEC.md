# Replication Specification: Arcangeli et al. (2019)
**Title**: Climate and Water Dissociation in the Extreme Atmosphere of WASP-18b  
**Authors**: J. Arcangeli, J. M. Goyal, et al.  
**Journal**: A&A, 625, A136 (2019) | **arXiv**: `1904.03206`

---

## Executive Summary & Core Equations

Arcangeli et al. (2019) model phase-resolved HST WFC3 emission spectra of WASP-18b, demonstrating water thermal dissociation on the dayside.

### 1. Water Dissociation & Emergent Emission
$$X_{\text{H2O}}(T) = \frac{1}{1 + \exp\left(\frac{T_0 - T}{\Delta T}\right)}$$
$$\frac{F_p}{F_\star}(\lambda, \phi) = \frac{B_\lambda(T(\phi))}{B_\lambda(T_\star)} \left(\frac{R_p}{R_\star}\right)^2$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: WASP-18b HST WFC3 secondary eclipse spectrum $F_p / F_\star$ [ppm] vs wavelength $\lambda$ ($1.1 - 1.7\,\mu\text{m}$).
2. **Figure 2**: Dayside ($2900\text{ K}$) vs nightside ($1500\text{ K}$) atmospheric temperature profiles and spectra.
