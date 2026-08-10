# Replication Specification: Ogilvie (2014)
**Title**: Tidal Dissipation in Stars and Fluid Planets  
**Author**: Gordon I. Ogilvie  
**Journal**: Annual Review of Astronomy and Astrophysics (ARA&A), 52, 171–212 (2014) | **arXiv**: `1405.0003`

---

## Executive Summary & Core Equations

This paper presents the canonical theory of tidal dissipation in fluid bodies, formulating inertial wave excitation in convective zones and tidal quality factor frequency dependence $Q_\star'(\omega)$.

### 1. Vector Tidal Friction & Dissipation Torque
The quadrupolar tidal torque exerted by a planet of mass $M_p$ at distance $a$ on a rotating star of radius $R_\star$ and mass $M_\star$ is:

$$\boldsymbol{\tau}_{\text{tide}} = -\frac{9}{2} \left(\frac{k_{2,\star}}{Q_\star'}\right) G M_p^2 \frac{R_\star^5}{a^6} \mathrm{sgn}(n_{\text{orb}} - \Omega_\star) \hat{\mathbf{z}}$$

where $n_{\text{orb}} = \sqrt{G(M_\star + M_p)/a^3}$ is the orbital mean motion and $\Omega_\star$ is the stellar spin angular velocity.

### 2. Frequency-Dependent Tidal Dissipation $Q_\star'(\omega)$
Inertial wave dissipation in stellar convection zones predicts a frequency-dependent tidal quality factor:

$$Q_\star'(\omega) = Q_{0}' \left[ 1 + \left(\frac{\omega - 2\Omega_\star}{\omega_0}\right)^2 \right]^{1/2}$$

where $\omega = 2|n_{\text{orb}} - \Omega_\star|$ is the tidal forcing frequency.

### 3. Orbital Decay Differential Equation
$$\frac{\mathrm{d}a}{\mathrm{d}t} = -9 \left(\frac{k_{2,\star}}{Q_\star'(\omega)}\right) \left(\frac{M_p}{M_\star}\right) \left(\frac{R_\star}{a}\right)^5 n_{\text{orb}} \, a$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: Tidal quality factor $Q_\star'$ vs forcing frequency ratio $\omega / \Omega_\star$.
2. **Figure 2**: Tidal orbital decay rate $|\mathrm{d}a/\mathrm{d}t|$ vs semi-major axis $a$ for $Q_\star' = 10^5, 10^6, 10^7, 10^8$.
3. **Figure 3**: 5-Gyr Orbital Period Evolution $P_{\text{orb}}(t)$ for WASP-19b, WASP-12b, and WASP-43b analogs.
4. **Figure 4**: Spin synchronization & alignment timescale $\tau_{\text{sync}}$ vs planet mass $M_p$.
