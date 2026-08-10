# Replication Specification: Hut (1981)
**Title**: Tidal Evolution in Close Binary Systems  
**Author**: Piet Hut  
**Journal**: Astronomy and Astrophysics (A&A), 99, 126 (1981)

---

## Executive Summary & Core Equations

Hut (1981) presents the canonical equilibrium tide weak-friction orbital evolution equations for binary systems and hot Jupiters.

### 1. Semi-Major Axis & Eccentricity Evolution
$$\frac{da}{dt} = -6 k_2 \Delta t \frac{G M_p^2}{M_\star} \frac{R_p^5}{a^7} \frac{1}{(1-e^2)^{15/2}} \left[ f_1(e^2) - (1-e^2)^{3/2} f_2(e^2) \frac{\Omega_p}{n} \right]$$

$$\frac{de}{dt} = -27 k_2 \Delta t \frac{G M_p^2}{M_\star} \frac{R_p^5}{a^8} \frac{e}{(1-e^2)^{13/2}} \left[ f_3(e^2) - \frac{11}{18} (1-e^2)^{3/2} f_4(e^2) \frac{\Omega_p}{n} \right]$$

### 2. Pseudo-Synchronous Spin Frequency
$$\frac{\Omega_{\text{ps}}}{n} = \frac{f_2(e^2)}{(1-e^2)^{3/2} f_5(e^2)}$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: Semi-major axis $a(t)$ and eccentricity $e(t)$ time series under tidal decay.
2. **Figure 2**: Ratio of pseudo-synchronous spin rate $\Omega_{\text{ps}} / n$ vs eccentricity $e$.
