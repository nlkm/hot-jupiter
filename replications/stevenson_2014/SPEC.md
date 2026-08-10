# Replication Specification: Stevenson et al. (2014)
**Title**: Thermal Structure of an Exoplanet Atmosphere Revealed by Thermal Emission Phase Curves  
**Authors**: Kevin B. Stevenson, Jean-Michel Désert, Michael R. Line, et al.  
**Journal**: Science, 346, 838 (2014) | **arXiv**: `1410.7041`

---

## Executive Summary & Core Equations

Stevenson et al. (2014) report spectroscopic thermal emission phase curves of the hot Jupiter WASP-43b using HST WFC3, revealing a sharp day-night temperature contrast.

### 1. Phase Curve Harmonic Expansion
$$\frac{F_p}{F_\star}(\phi) = A_0 + A_1 \cos(2\pi \phi - \phi_1) + A_2 \cos(4\pi \phi - \phi_2)$$

### 2. Longitudinal Temperature Profile
$$T_b(\phi) = T_{\text{night}} + (T_{\text{day}} - T_{\text{night}}) \cos^2\left(\frac{\phi - \phi_{\text{offset}}}{2}\right)$$
where $T_{\text{day}} = 1500\text{ K}$, $T_{\text{night}} = 500\text{ K}$, and offset $\phi_{\text{offset}} = -10^\circ$.

---

## Benchmark Figures to Replicate

1. **Figure 1**: Spectroscopic thermal emission phase curves $(F_p / F_\star)(\phi)$ [ppm] vs orbital phase $\phi$.
2. **Figure 2**: Brightness temperature $T_b(\phi)$ [K] vs orbital longitude $\phi$ [deg].
