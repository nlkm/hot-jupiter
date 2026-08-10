# Replication Specification: Madhusudhan & Seager (2009)
**Title**: A Temperature and Abundance Retrieval Method for Exoplanet Atmospheres  
**Authors**: Nikku Madhusudhan, Sara Seager  
**Journal**: The Astrophysical Journal (ApJ), 707, 24 (2009) | **arXiv**: `0910.1347`

---

## Executive Summary & Core Equations

Madhusudhan & Seager (2009) introduce the benchmark 6-parameter atmospheric temperature-pressure $T(P)$ retrieval method.

### 1. Parametrized $T(P)$ Atmospheric Profile
$$P(T) = P_0 \, \exp \left( -\alpha \sqrt{T - T_0} \right)$$
Spectral flux integral:
$$F_\lambda = 2\pi \int_0^1 B_\lambda(T(\mu)) \mu \, d\mu$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: Retrieved atmospheric T-P profile confidence envelope (HD 189733b).
2. **Figure 2**: Secondary eclipse planet-to-star flux ratio $F_p / F_\star(\lambda)$ [$\mu$m].
