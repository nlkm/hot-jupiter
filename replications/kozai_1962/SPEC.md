# Replication Specification: Kozai (1962)
**Title**: Secular Perturbations of Asteroids with High Inclination and Eccentricity  
**Authors**: Yoshihide Kozai  
**Journal**: The Astronomical Journal (AJ), 67, 591 (1962)

---

## Executive Summary & Core Equations

Kozai (1962) derived the quadrupolar secular Hamiltonian for hierarchical 3-body systems, discovering the Kozai-Lidov resonance mechanism coupling eccentricity $e$ and inclination $i$.

### 1. Conserved Vertical Angular Momentum Component
$$H_z = \sqrt{1 - e^2} \cos i = \text{const}$$

### 2. Quadrupolar Secular Hamiltonian Trajectory
$$\Theta(e, \omega) = (2 + 3 e^2) (3 \cos^2 i - 1) + 15 e^2 \sin^2 i \cos(2\omega) = \text{const}$$

### 3. Maximum Eccentricity Formula
For $e_0 \to 0$ and initial inclination $i_0 > i_{\text{crit}} \approx 39.23^\circ$:
$$e_{\text{max}} = \sqrt{1 - \frac{5}{3} \cos^2 i_0}$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: Phase space trajectory curves in $(e, \omega)$ for $i_0 = 65^\circ$.
2. **Figure 2**: Maximum eccentricity $e_{\text{max}}$ vs initial inclination $i_0$ [deg].
