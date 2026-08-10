# Replication Specification: Showman et al. (2009)
**Title**: Atmospheric Circulation of Exoplanets: Atmospheric Dynamics of Hot Jupiters  
**Authors**: Adam P. Showman, Jonathan J. Fortney, Y. K. Cho, Curtis S. Cooper, Mark S. Marley, K. Lodders  
**Journal**: The Astrophysical Journal (ApJ), 699, 564 (2009) | **arXiv**: `0809.2089`

---

## Executive Summary & Core Equations

Showman et al. (2009) present 3D general circulation models (GCM) of tidally locked Hot Jupiters, predicting superrotating equatorial jets and eastward hotspot offsets.

### 1. Radiative Timescale Formula
$$\tau_{\text{rad}} = \frac{c_p P}{4 \sigma T^3 g}$$

### 2. Day-Night Temperature Contrast
$$\Delta T_{\text{day-night}} = \frac{\tau_{\text{rad}}}{\tau_{\text{rad}} + \tau_{\text{ad}}} \Delta T_{\text{eq}}$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: Day-night temperature profile $T(\lambda)$ [K] at 100 mbar showing the eastward hotspot shift $\Delta \lambda \approx 20^\circ-60^\circ$.
2. **Figure 2**: Zonal-mean wind speed $\bar{u}(\phi)$ [m/s] showing equatorial superrotation ($\bar{u} \approx 1500\,\text{m/s}$).
