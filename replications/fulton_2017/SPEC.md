# Replication Specification: Fulton et al. (2017)
**Title**: The California-Kepler Survey. III. A Gap in the Radius Distribution of Small Planets  
**Authors**: Benjamin J. Fulton et al.  
**Journal**: The Astronomical Journal (AJ), 154, 109 (2017) | **arXiv**: `1703.0004`

---

## Executive Summary & Core Equations

Fulton et al. (2017) present high-precision CKS spectroscopy revealing a distinct gap in the radius distribution of small exoplanets separating super-Earths ($R_p \sim 1.3\,R_\oplus$) from sub-Neptunes ($R_p \sim 2.4\,R_\oplus$).

### 1. Kernel Density Estimation (KDE)
$$f(R_p) = \frac{1}{N h} \sum_{i=1}^N K\left(\frac{R_p - R_{p,i}}{h}\right)$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: CKS radius distribution $f(R_p)$ with deep valley at $R_p = 1.8\,R_\oplus$.
2. **Figure 2**: Planetary radius $R_p$ [$R_\oplus$] vs incident stellar flux $S$ [$S_\oplus$].
