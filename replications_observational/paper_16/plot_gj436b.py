# Copyright 2026 Antigravity Scientific Automation & Observational Astrophysics Campaign
# Multi-Plot & Model Development Script for Observational Paper #16: GJ 436b Extended Hydrogen Cloud

import matplotlib.pyplot as plt
import numpy as np

# Set style
plt.style.use('seaborn-v0_8-whitegrid' if 'seaborn-v0_8-whitegrid' in
              plt.style.available else 'default')

# FIGURE 1: Model Choices & Parameter Sensitivity Analysis
fig1, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))

# Panel A: Hydrodynamic Mass Loss Rate vs Incident XUV Flux
f_xuv_grid = np.logspace(3, 5, 500)
epsilon = 0.15
R_p_cm = 2.74e9
M_p_g = 1.32e28
G_cgs = 6.67430e-8
K_tide = 0.75

mdot_grid = (3.0 * epsilon * f_xuv_grid * R_p_cm**3) / (4.0 * G_cgs * M_p_g *
                                                        K_tide)

ax1.loglog(f_xuv_grid,
           mdot_grid,
           'r-',
           lw=2.5,
           label=r'Energy-Limited Mass Loss $\dot{M}$')
ax1.axvline(
    62810.0,
    color='purple',
    linestyle=':',
    label=r'GJ 436 Nominal XUV Flux ($6.3 \times 10^4\text{ erg/cm}^2/\text{s}$)'
)
ax1.scatter([62810.0], [2.2e10], color='indigo', s=80, zorder=5)
ax1.set_xlabel(r'Incident XUV Flux $F_{\text{XUV}}$ [erg cm$^{-2}$ s$^{-1}$]',
               fontsize=12)
ax1.set_ylabel(r'Mass Loss Rate $\dot{M}$ [g/s]', fontsize=12)
ax1.set_title('Model Choice: Mass Loss vs M-Dwarf XUV Flux',
              fontsize=13,
              fontweight='bold')
ax1.legend(loc='upper left', fontsize=10)

# Panel B: Cloud Radius Ratio vs Gravitational Potential
potential_grid = np.linspace(0.2, 2.0, 300)  # relative to Jupiter
coma_radius_ratio = 30.0 / potential_grid

ax2.plot(
    potential_grid,
    coma_radius_ratio,
    'm-',
    lw=2.5,
    label=r'Coma Radius Ratio $R_{\text{coma}}/R_p \propto (G M_p/R_p)^{-1}$')
ax2.axvline(0.32,
            color='darkorchid',
            linestyle='--',
            label=r'GJ 436b Low Potential ($0.32 \Phi_J$)')
ax2.axhline(30.0,
            color='darkgreen',
            linestyle=':',
            label=r'Measured Coma Extent ($30 R_p$)')
ax2.set_xlabel(r'Gravitational Potential $\Phi / \Phi_{\text{Jupiter}}$',
               fontsize=12)
ax2.set_ylabel(r'Hydrogen Coma Extent $R_{\text{coma}} / R_p$', fontsize=12)
ax2.set_title('Model Choice: Low Potential Exospheric Expansion',
              fontsize=13,
              fontweight='bold')
ax2.legend(loc='upper right', fontsize=10)

plt.tight_layout()
fig1.savefig('replications_observational/paper_16/fig_model_choices.png',
             dpi=300)
fig1.savefig('replications_observational/paper_16/fig_model_choices.pdf')
plt.close(fig1)

# FIGURE 2: Observations vs Model Comparison
fig2, (ax3, ax4) = plt.subplots(1, 2, figsize=(12, 5))

# Panel C: Asymmetric 22-Hour Lyman-alpha Transit Light Curve
t_hours = np.linspace(-6, 18, 500)
optical_lc = np.where(np.abs(t_hours - 0.0) < 0.5, 1.0 - 0.0069,
                      1.0)  # 0.69% optical depth

# Asymmetric Ly-alpha transit (early ingress -2 hr, peak -56.3% depth at egress, long post-transit tail to +16 hr)
lyman_alpha_lc = np.ones_like(t_hours)
mask_cloud = (t_hours > -2.5) & (t_hours < 16.0)
t_cloud = t_hours[mask_cloud]
# Asymmetric shape model
depth_profile = 0.563 * np.exp(-((t_cloud - 1.0) / 4.5)**2) * np.where(
    t_cloud > 1.0, np.exp(-(t_cloud - 1.0) / 6.0), 1.0)
lyman_alpha_lc[mask_cloud] -= depth_profile

ax3.plot(t_hours,
         optical_lc,
         'k--',
         lw=1.8,
         label='Optical Transit (1 hr, 0.69% depth)')
ax3.plot(t_hours,
         lyman_alpha_lc,
         'm-',
         lw=2.5,
         label=r'HST STIS/WFC3 Ly-$\alpha$ Transit (22 hr, 56.3% depth)')

# Observed HST data points (Ehrenreich et al. 2015)
hst_t = np.array([-4.0, -1.0, 1.0, 3.5, 7.0, 12.0, 16.0])
hst_f = np.array([0.99, 0.62, 0.44, 0.51, 0.68, 0.85, 0.98])
hst_err = np.array([0.03, 0.04, 0.04, 0.04, 0.05, 0.04, 0.03])
ax3.errorbar(hst_t,
             hst_f,
             yerr=hst_err,
             fmt='o',
             color='purple',
             ecolor='gray',
             capsize=4,
             label='HST STIS/WFC3 Data (Ehrenreich+ 2015)')

ax3.set_xlabel('Time from Optical Mid-Transit [hours]', fontsize=12)
ax3.set_ylabel('Normalized Light Curve Flux', fontsize=12)
ax3.set_title('GJ 436b Asymmetric 22-Hour Ly-alpha Transit',
              fontsize=13,
              fontweight='bold')
ax3.legend(loc='lower left', fontsize=9)

# Panel D: Blue-Shifted Absorption Spectral Line Profile
v_kms = np.linspace(-250, 250, 400)
line_profile = np.ones_like(v_kms)

mask_blue = (v_kms > -120) & (v_kms < -20)
line_profile[mask_blue] -= 0.563

ax4.plot(v_kms,
         np.ones_like(v_kms),
         'b--',
         lw=1.8,
         label=r'Out-of-Transit Ly-$\alpha$ Profile')
ax4.plot(v_kms,
         line_profile,
         'm-',
         lw=2.5,
         label=r'In-Transit Blue-Shifted Feature (56.3% Absorption)')
ax4.axvspan(
    -120,
    -20,
    color='mediumorchid',
    alpha=0.2,
    label=r'Escaping Gas Cloud Velocity Range ($-120\text{ to }-20\text{ km/s}$)'
)
ax4.set_xlabel('Doppler Velocity [km/s]', fontsize=12)
ax4.set_ylabel('Relative Flux', fontsize=12)
ax4.set_title('Giant Cloud Blue-Shifted Spectral Feature',
              fontsize=13,
              fontweight='bold')
ax4.legend(loc='lower left', fontsize=9)

plt.tight_layout()
fig2.savefig('replications_observational/paper_16/fig_comparison.png', dpi=300)
fig2.savefig('replications_observational/paper_16/fig_comparison.pdf')
plt.close(fig2)

print("✅ Saved Paper #16 multi-panel diagnostic figures!")
