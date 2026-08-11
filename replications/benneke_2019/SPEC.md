# Replication Specification: Benneke et al. (2019)
**Title**: Water Vapor and Clouds on the Sub-Neptune K2-18b  
**Authors**: Björn Benneke, Ian Wong, et al.  
**Journal**: Nature Astronomy, 3, 813 (2019) | **arXiv**: `1907.00449`

---

## Executive Summary & Core Equations

Benneke et al. (2019) report the detection of water vapor and cloud condensation in the atmosphere of the habitable-zone sub-Neptune K2-18b.

### 1. Water Vapor Absorption & Cloud Opacity
$$\left(\frac{R_p}{R_\star}\right)^2(\lambda) = \left(\frac{R_0}{R_\star}\right)^2 + \frac{2 R_0 H}{R_\star^2} \left[\tau_0 + \ln \left(X_{\text{H}_2\text{O}} \sigma_{\text{H}_2\text{O}}(\lambda) + X_{\text{cloud}} \sigma_{\text{cloud}}\right)\right]$$

---

## Benchmark Figures to Replicate

1. **Figure 1**: K2-18b HST WFC3 transmission spectrum $(R_p/R_\star)^2$ vs wavelength $\lambda$ (1.1 to 1.7 $\mu$m).
2. **Figure 2**: Retrieved water volume mixing ratio posterior distribution $\log_{10} X_{\text{H}_2\text{O}}$ (-4.0 to -1.0).
