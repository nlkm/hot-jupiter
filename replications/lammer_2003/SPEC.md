# Replication Specification: Lammer et al. (2003)
**Title**: Atmospheric Loss of Exoplanets Resulting from Stellar X-ray and Extreme-Ultraviolet Heating  
**Authors**: Helmut Lammer et al.  
**Journal**: The Astrophysical Journal Letters (ApJL), 598, L121 (2003) | **arXiv**: `astro-ph/0301001`

---

## Executive Summary & Core Equations

Lammer et al. (2003) formulate the energy-limited hydrodynamic atmospheric escape rate for hot Jupiters (specifically HD 209458b) under stellar XUV irradiation.

### 1. Energy-Limited Mass Loss Rate
$$\dot{M}_{\text{XUV}} = \frac{3 \eta F_{\text{XUV}}}{4 G \rho_p K_{\text{tide}}} = \frac{\pi \eta R_p R_{\text{sub}}^2 F_{\text{XUV}}}{G M_p K_{\text{tide}}}$$

where $K_{\text{tide}} = 1 - \frac{3}{2 \xi} + \frac{1}{2 \xi^3}$ and $\xi = R_{\text{Roche}} / R_p$.

---

## Benchmark Figures to Replicate

1. **Figure 1**: Atmospheric mass loss rate $\dot{M}$ [g/s] vs stellar XUV flux $F_{\text{XUV}}$ [erg/cm$^2$/s].
2. **Figure 2**: Planetary mass evolution $M_p(t)$ [$M_{\text{J}}$] for HD 209458b over 5 Gyr.
