# Replication Specification: Fortney et al. (2010)
**Title**: Transmission Spectra of Irradiated Gas Giant Exoplanets: The Impact of Alkali Metallicity, Clouds, and Thermal Structure  
**Authors**: Jonathan J. Fortney, Mark S. Marley, K. Lodders, et al.  
**Journal**: ApJ, 709, 1396 (2010) | **arXiv**: `0912.1618`

---

## Executive Summary & Core Equations

Fortney et al. (2010) present a comprehensive grid of synthetic transmission spectra for gas giants, demonstrating how metallicity, alkali absorption (Na/K), and cloud decks alter transit depths across $0.3 - 5.0 \mu\text{m}$.

### 1. Transmission Depth Grid Formula
$$\Delta \left(\frac{R_p}{R_\star}\right)^2 = \frac{2 R_p H}{R_\star^2} \ln\left(1 + \frac{P_0}{P_{\text{cloud}}} + \frac{\sum_i \kappa_i(\lambda) X_i P_0}{\mu g}\right)$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: Synthetic transmission spectra $(R_p / R_\star)^2$ [%] vs wavelength $\lambda$ [$\mu$m] for $1\times, 10\times, 30\times$ solar metallicity.
2. **Figure 2**: Effect of cloud decks ($P_{\text{cloud}} = \text{clear}, 10\,\text{mbar}, 1\,\text{mbar}$) on transmission spectra.
