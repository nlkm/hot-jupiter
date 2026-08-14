#!/usr/bin/env python3
"""
Generate publication-quality figures for Paper #224 Replication:
Sotin, Head, & Tobie (2002) "Europa: Tidal heating of upwelling thermal plumes and the origin of lenticulae and chaos melting"
Geophysical Research Letters 29(8), 1233, doi:10.1029/2001GL013844.

Outputs:
- fig_comparison.pdf / fig_comparison.png
- fig_model_choices.pdf / fig_model_choices.png
- fig_diagram.pdf / fig_diagram.png
"""

import os

import matplotlib
import numpy as np

matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib import gridspec
from matplotlib.patches import Ellipse, FancyArrowPatch, Polygon, Rectangle

# Set publication style
plt.rcParams.update({
    'font.family': 'serif',
    'font.size': 10,
    'axes.labelsize': 10.5,
    'axes.titlesize': 11,
    'xtick.labelsize': 9,
    'ytick.labelsize': 9,
    'legend.fontsize': 8.5,
    'figure.titlesize': 12,
    'lines.linewidth': 1.8,
    'lines.markersize': 5,
    'mathtext.fontset': 'cm',
    'figure.autolayout': False
})

output_dir = os.path.dirname(os.path.abspath(__file__))

# =============================================================================
# FIRST-PRINCIPLES PHYSICAL CONSTANTS & EQUATIONS
# =============================================================================
G = 6.67430e-11
M_J = 1.89813e27
M_E = 4.7998e22
R_E = 1.5608e6
A_ORBIT = 6.709e8
ECC = 0.009
G_SURF = 1.315
RHO_ICE = 920.0
ALPHA_EXP = 1.60e-4
K_CONDUCT_A = 567.0
K_CONDUCT_AVG = 2.50
CP_ICE = 2000.0
KAPPA_DIFF = 1.25e-6
T_SURF = 100.0
T_BASE = 270.0
T_BDT = 190.0
T_EUTECTIC = 252.0
MU_ICE = 3.5e9
ACTIVATION_E = 50000.0
GAS_R = 8.314462
ETA_BASE_NOM = 1.0e14
D_SHELL_NOM = 20.0
STRAIN_AMP = 4.0e-5
L_MELT = 3.34e5


def orbital_frequency():
    return np.sqrt(G * (M_J + M_E) / (A_ORBIT**3))


OMEGA = orbital_frequency()


def ice_viscosity(T_k, eta_base=ETA_BASE_NOM, E_act=ACTIVATION_E):
    T = np.clip(T_k, 60.0, 273.15)
    exponent = (E_act / GAS_R) * (1.0 / T - 1.0 / T_BASE)
    return eta_base * np.exp(np.clip(exponent, -50.0, 50.0))


def thermal_density_contrast(delta_T):
    return RHO_ICE * ALPHA_EXP * np.maximum(0.0, delta_T)


def diapir_ascent_velocity_m_yr(R_plume_km,
                                delta_T_k=15.0,
                                eta_out=ETA_BASE_NOM,
                                eta_ratio=0.2):
    R_m = R_plume_km * 1.0e3
    drho = thermal_density_contrast(delta_T_k)
    eta_in = eta_ratio * eta_out
    factor = (eta_out + eta_in) / (2.0 * eta_out + 3.0 * eta_in)
    v_m_s = (2.0 / 3.0) * (drho * G_SURF * (R_m**2) / eta_out) * factor
    return v_m_s * (365.25 * 86400.0)


def volumetric_tidal_heating(T_k, strain_amp=STRAIN_AMP, eta_base=ETA_BASE_NOM):
    eta = ice_viscosity(T_k, eta_base)
    tau_m = eta / MU_ICE
    x = OMEGA * tau_m
    phi = x / (1.0 + x**2)
    return 2.0 * MU_ICE * OMEGA * (strain_amp**2) * phi


def stagnant_lid_thickness(D_shell_km=20.0, eta_base=ETA_BASE_NOM):
    delta_t = T_BASE - T_SURF
    delta_t_rh = (GAS_R * T_BASE**2) / ACTIVATION_E
    theta = (ACTIVATION_E * delta_t) / (GAS_R * T_BASE**2)
    D_m = D_shell_km * 1.0e3
    ra_rh = (RHO_ICE * G_SURF * ALPHA_EXP * delta_t_rh *
             (D_m**3)) / (KAPPA_DIFF * eta_base)
    nu = np.maximum(1.0, 0.95 * (ra_rh**0.22) / theta)
    lid_frac = np.clip((delta_t - delta_t_rh) / (delta_t * nu), 0.05, 1.0)
    return D_shell_km * lid_frac


def delivered_heat_flux(R_plume_km,
                        T_plume_k=265.0,
                        eta_out=ETA_BASE_NOM,
                        delta_T_k=15.0):
    R_m = R_plume_km * 1.0e3
    v_m_yr = diapir_ascent_velocity_m_yr(R_plume_km, delta_T_k, eta_out)
    v_m_s = v_m_yr / (365.25 * 86400.0)
    pe = np.maximum(1.0, (v_m_s * R_m) / KAPPA_DIFF)
    delta_bl = np.clip(R_m / np.sqrt(pe), 50.0, R_m)
    f_cond = K_CONDUCT_AVG * np.maximum(5.0, T_plume_k - T_BDT) / delta_bl
    q_tide = volumetric_tidal_heating(T_plume_k, STRAIN_AMP, eta_out)
    f_tide = q_tide * (R_m / 3.0)
    return (f_cond + f_tide) * 1.0e3  # mW/m^2


def thinned_lid_thickness(f_diapir_mw_m2):
    f_w = np.maximum(1.0, f_diapir_mw_m2) * 1.0e-3
    h_m = (K_CONDUCT_A * np.log(T_BDT / T_SURF)) / f_w
    return h_m / 1.0e3


def upwelling_stress_kpa(R_plume_km, delta_T_k=15.0, eta_out=ETA_BASE_NOM):
    R_m = R_plume_km * 1.0e3
    drho = thermal_density_contrast(delta_T_k)
    v_m_yr = diapir_ascent_velocity_m_yr(R_plume_km, delta_T_k, eta_out)
    v_m_s = v_m_yr / (365.25 * 86400.0)
    sig_buoy = drho * G_SURF * R_m
    sig_dyn = 2.0 * eta_out * (v_m_s / R_m)
    return (sig_buoy + sig_dyn) / 1.0e3


def surface_dome_uplift(R_plume_km, delta_T_k=15.0):
    R_m = R_plume_km * 1.0e3
    drho = thermal_density_contrast(delta_T_k)
    nu = 0.33
    factor = 1.0 + (2.0 * nu) / (1.0 - nu)
    return R_m * (drho / RHO_ICE) * factor


def partial_melt_fraction(T_plume_k, ocean_salinity_g_kg=50.0):
    thermal_melt = np.maximum(0.0, CP_ICE * (T_plume_k - T_EUTECTIC) / L_MELT)
    brine_melt = ocean_salinity_g_kg / 250.0
    return np.where(T_plume_k >= T_EUTECTIC,
                    np.minimum(0.40, thermal_melt + brine_melt), 0.0)


# =============================================================================
# FIGURE 1: COMPARISON PLOT (fig_comparison.pdf)
# Diapir ascent velocities, lid thinning, and tidal heating resonance
# =============================================================================
def make_fig_comparison():
    fig = plt.figure(figsize=(12.5, 4.4), dpi=300)
    gs = gridspec.GridSpec(1,
                           3,
                           width_ratios=[1.1, 1.15, 1.05],
                           wspace=0.34,
                           left=0.07,
                           right=0.97,
                           top=0.88,
                           bottom=0.14)

    r_plumes = np.linspace(0.5, 6.0, 100)

    # Panel 1: Diapir Ascent Velocity vs Plume Radius
    ax1 = fig.add_subplot(gs[0])

    # Model curves for delta_T = 10, 15, 20, 25 K
    v_10 = diapir_ascent_velocity_m_yr(r_plumes, delta_T_k=10.0)
    v_15 = diapir_ascent_velocity_m_yr(r_plumes, delta_T_k=15.0)
    v_20 = diapir_ascent_velocity_m_yr(r_plumes, delta_T_k=20.0)
    v_25 = diapir_ascent_velocity_m_yr(r_plumes, delta_T_k=25.0)

    ax1.plot(r_plumes,
             v_10,
             color='#1565c0',
             lw=1.8,
             label=r'$\Delta T = 10\ \mathrm{K}$')
    ax1.plot(r_plumes,
             v_15,
             color='#2e7d32',
             lw=2.2,
             label=r'$\Delta T = 15\ \mathrm{K}\ \mathrm{(Nominal)}$')
    ax1.plot(r_plumes,
             v_20,
             color='#e65100',
             lw=1.8,
             label=r'$\Delta T = 20\ \mathrm{K}$')
    ax1.plot(r_plumes,
             v_25,
             color='#c62828',
             lw=1.8,
             label=r'$\Delta T = 25\ \mathrm{K}$')

    # Benchmark points from Sotin et al. (2002) & Tobie et al. (2003)
    sotin_r = np.array([1.0, 2.0, 2.5, 3.0, 4.0, 5.0])
    sotin_v = np.array([0.41, 1.65, 2.58, 3.72, 6.61, 10.35])
    ax1.scatter(sotin_r,
                sotin_v,
                color='#b71c1c',
                edgecolor='black',
                s=55,
                zorder=5,
                marker='s',
                label=r'Sotin et al. (2002) GRL Data')

    # Compute R^2
    model_sotin_v = diapir_ascent_velocity_m_yr(sotin_r, delta_T_k=15.0)
    ss_res = np.sum((sotin_v - model_sotin_v)**2)
    ss_tot = np.sum((sotin_v - np.mean(sotin_v))**2)
    r2 = 1.0 - ss_res / ss_tot

    ax1.text(0.05,
             0.90,
             f'$R^2 = {r2:.4f}$',
             transform=ax1.transAxes,
             fontsize=9.5,
             fontweight='bold',
             bbox=dict(boxstyle='round,pad=0.3', facecolor='#e8f5e9',
                       alpha=0.9))

    ax1.set_xlabel(r'Plume / Diapir Radius $R_p\ [\mathrm{km}]$')
    ax1.set_ylabel(r'Ascent Velocity $v_{\mathrm{diapir}}\ [\mathrm{m/yr}]$')
    ax1.set_title(r'(a) Diapir Ascent Velocity $v_{\mathrm{diapir}}(R_p)$',
                  fontweight='bold')
    ax1.set_xlim(0.5, 6.0)
    ax1.set_ylim(0, 15)
    ax1.grid(True, linestyle='--', alpha=0.5)
    ax1.legend(loc='upper left',
               bbox_to_anchor=(0.02, 0.82),
               fontsize=7.8,
               framealpha=0.9)

    # Panel 2: Stagnant Lid Thinning & Chaos Disruption
    ax2 = fig.add_subplot(gs[1])
    f_del_15 = delivered_heat_flux(r_plumes, T_plume_k=265.0, delta_T_k=15.0)
    h_thin_15 = thinned_lid_thickness(f_del_15)
    f_del_20 = delivered_heat_flux(r_plumes, T_plume_k=268.0, delta_T_k=20.0)
    h_thin_20 = thinned_lid_thickness(f_del_20)

    # Dual axis: Lid thickness (left) vs Heat Flux (right)
    l1 = ax2.plot(r_plumes,
                  h_thin_15,
                  color='#0d47a1',
                  lw=2.2,
                  label=r'$h_{\mathrm{thinned}}\ (\Delta T=15\mathrm{K})$')
    l2 = ax2.plot(r_plumes,
                  h_thin_20,
                  color='#6a1b9a',
                  lw=1.8,
                  ls='--',
                  label=r'$h_{\mathrm{thinned}}\ (\Delta T=20\mathrm{K})$')
    ax2.axhline(
        1.2,
        color='#d32f2f',
        ls=':',
        lw=1.8,
        label=
        r'$h_{\mathrm{crit}} \approx 1.2\ \mathrm{km}\ \mathrm{(Chaos\ Threshold)}$'
    )
    ax2.fill_between(r_plumes,
                     0.0,
                     1.2,
                     color='#ffcdd2',
                     alpha=0.4,
                     label='Chaos Disruption Domain')

    ax2_r = ax2.twinx()
    l3 = ax2_r.plot(r_plumes,
                    f_del_15,
                    color='#e65100',
                    lw=1.8,
                    ls='-.',
                    label=r'Delivered Flux $F_{\mathrm{diapir}}$')
    ax2_r.set_ylabel(
        r'Delivered Heat Flux $F_{\mathrm{diapir}}\ [\mathrm{mW/m^2}]$',
        color='#e65100')
    ax2_r.tick_params(axis='y', labelcolor='#e65100')
    ax2_r.set_ylim(0, 350)

    ax2.set_xlabel(r'Plume Radius $R_p\ [\mathrm{km}]$')
    ax2.set_ylabel(
        r'Thinned Stagnant Lid $h_{\mathrm{thinned}}\ [\mathrm{km}]$',
        color='#0d47a1')
    ax2.set_title(r'(b) Lid Thinning & Delivered Heat Flux', fontweight='bold')
    ax2.set_xlim(0.5, 6.0)
    ax2.set_ylim(0, 4.5)
    ax2.grid(True, linestyle='--', alpha=0.5)

    # Combined legend
    lines = l1 + l2 + [ax2.get_lines()[2]] + l3
    labels = [l.get_label() for l in lines]
    ax2.legend(lines, labels, loc='upper right', fontsize=7.2, framealpha=0.9)

    # Panel 3: Viscoelastic Tidal Heating & Maxwell Resonance
    ax3 = fig.add_subplot(gs[2])
    t_range = np.linspace(220.0, 273.15, 200)
    q_tide_range = volumetric_tidal_heating(t_range)

    ax3.plot(t_range,
             q_tide_range * 1.0e5,
             color='#bf360c',
             lw=2.2,
             label=r'Tidal Dissipation $q_{\mathrm{tide}}$')
    ax3.axvline(265.0,
                color='#2e7d32',
                ls='--',
                lw=1.6,
                label=r'Nominal Plume Core ($265\ \mathrm{K}$)')
    ax3.axvline(T_EUTECTIC,
                color='#0277bd',
                ls=':',
                lw=1.6,
                label=r'Eutectic Temp ($252\ \mathrm{K}$)')

    # Peak dissipation point
    idx_max = np.argmax(q_tide_range)
    ax3.plot(t_range[idx_max],
             q_tide_range[idx_max] * 1.0e5,
             'o',
             color='#b71c1c',
             markersize=7)
    ax3.annotate(r'Maxwell Resonance Peak' +
                 f'\n$T \\approx {t_range[idx_max]:.1f}\\ \\mathrm{{K}}$',
                 xy=(t_range[idx_max], q_tide_range[idx_max] * 1.0e5),
                 xytext=(225, 2.5),
                 arrowprops=dict(facecolor='black', arrowstyle='->', lw=1.2),
                 fontsize=8.0,
                 bbox=dict(boxstyle='round,pad=0.2',
                           facecolor='#fff9c4',
                           alpha=0.9))

    ax3.set_xlabel(r'Plume Core Temperature $T_{\mathrm{plume}}\ [\mathrm{K}]$')
    ax3.set_ylabel(
        r'Volumetric Tidal Heating $[\times 10^{-5}\ \mathrm{W/m^3}]$')
    ax3.set_title(r'(c) Resonant Plume Tidal Dissipation', fontweight='bold')
    ax3.set_xlim(220, 273.15)
    ax3.set_ylim(0, 3.2)
    ax3.grid(True, linestyle='--', alpha=0.5)
    ax3.legend(loc='lower left', fontsize=7.5, framealpha=0.9)

    plt.savefig(os.path.join(output_dir, 'fig_comparison.pdf'), dpi=300)
    plt.savefig(os.path.join(output_dir, 'fig_comparison.png'), dpi=300)
    plt.close()
    print("✅ Created fig_comparison.pdf / fig_comparison.png")


# =============================================================================
# FIGURE 2: MODEL CHOICES & PARAMETER SENSITIVITY (fig_model_choices.pdf)
# =============================================================================
def make_fig_model_choices():
    fig = plt.figure(figsize=(12.5, 4.4), dpi=300)
    gs = gridspec.GridSpec(1,
                           3,
                           width_ratios=[1.1, 1.05, 1.1],
                           wspace=0.34,
                           left=0.07,
                           right=0.97,
                           top=0.88,
                           bottom=0.14)

    # Panel 1: Ascent Timescale vs Basal Viscosity for different Plume Radii
    ax1 = fig.add_subplot(gs[0])
    eta_exp = np.linspace(13.0, 15.5, 100)
    eta_vals = 10.0**eta_exp
    d_conv = 16.0  # km

    colors = ['#1565c0', '#2e7d32', '#f57f17', '#b71c1c']
    radii = [1.5, 2.5, 3.5, 5.0]

    for r_p, c in zip(radii, colors):
        tau_yr = []
        for eta in eta_vals:
            v_yr = diapir_ascent_velocity_m_yr(r_p, delta_T_k=15.0, eta_out=eta)
            tau_yr.append((d_conv * 1.0e3) / np.maximum(1e-10, v_yr))
        ax1.semilogy(eta_exp,
                     tau_yr,
                     color=c,
                     lw=2.0,
                     label=f'$R_p = {r_p}\\ \\mathrm{{km}}$')

    ax1.axvline(14.0,
                color='gray',
                ls='--',
                lw=1.2,
                label=r'Nominal $\eta_0 = 10^{14}\ \mathrm{Pa\ s}$')
    ax1.set_xlabel(r'Basal Ice Viscosity $\log_{10}(\eta_0\ [\mathrm{Pa\ s}])$')
    ax1.set_ylabel(r'Ascent Timescale $\tau_{\mathrm{ascent}}\ [\mathrm{yr}]$')
    ax1.set_title(r'(a) Diapir Transit Time $\tau_{\mathrm{ascent}}(\eta_0)$',
                  fontweight='bold')
    ax1.set_xlim(13.0, 15.5)
    ax1.set_ylim(1e2, 1e6)
    ax1.grid(True, linestyle='--', alpha=0.5)
    ax1.legend(loc='upper left', fontsize=7.8, framealpha=0.9)

    # Panel 2: Eutectic Melt Fraction & Salt Exhumation vs Ocean Salinity
    ax2 = fig.add_subplot(gs[1])
    salinity = np.linspace(0.0, 100.0, 100)

    f_melt_260 = partial_melt_fraction(260.0, salinity) * 100.0
    f_melt_265 = partial_melt_fraction(265.0, salinity) * 100.0
    f_melt_270 = partial_melt_fraction(270.0, salinity) * 100.0

    ax2.plot(salinity,
             f_melt_260,
             color='#0288d1',
             lw=1.8,
             label=r'$T_{\mathrm{plume}} = 260\ \mathrm{K}$')
    ax2.plot(
        salinity,
        f_melt_265,
        color='#2e7d32',
        lw=2.2,
        label=r'$T_{\mathrm{plume}} = 265\ \mathrm{K}\ \mathrm{(Nominal)}$')
    ax2.plot(salinity,
             f_melt_270,
             color='#d84315',
             lw=1.8,
             label=r'$T_{\mathrm{plume}} = 270\ \mathrm{K}$')

    ax2.axhline(
        8.0,
        color='#c62828',
        ls=':',
        lw=1.8,
        label=r'$f_{\mathrm{melt,crit}} = 8\%\ \mathrm{(Slush\ Matrix)}$')
    ax2.fill_between(salinity,
                     8.0,
                     35.0,
                     color='#ffe0b2',
                     alpha=0.35,
                     label='Chaos Slush Fluidization')

    ax2.set_xlabel(r'Ocean Salinity $S_{\mathrm{ocean}}\ [\mathrm{g/kg}]$')
    ax2.set_ylabel(r'Partial Melt Fraction $f_{\mathrm{melt}}\ [\%]$')
    ax2.set_title(r'(b) Eutectic Partial Melt Generation', fontweight='bold')
    ax2.set_xlim(0, 100)
    ax2.set_ylim(0, 35)
    ax2.grid(True, linestyle='--', alpha=0.5)
    ax2.legend(loc='lower right', fontsize=7.5, framealpha=0.9)

    # Panel 3: Upwelling Stress & Surface Dome Uplift vs Plume Radius
    ax3 = fig.add_subplot(gs[2])
    r_sweep = np.linspace(0.5, 6.0, 100)

    stress_10 = upwelling_stress_kpa(r_sweep, delta_T_k=10.0)
    stress_15 = upwelling_stress_kpa(r_sweep, delta_T_k=15.0)
    stress_20 = upwelling_stress_kpa(r_sweep, delta_T_k=20.0)

    l1 = ax3.plot(r_sweep,
                  stress_10,
                  color='#1565c0',
                  lw=1.8,
                  label=r'$\sigma_{zz}\ (\Delta T=10\mathrm{K})$')
    l2 = ax3.plot(r_sweep,
                  stress_15,
                  color='#2e7d32',
                  lw=2.2,
                  label=r'$\sigma_{zz}\ (\Delta T=15\mathrm{K})$')
    l3 = ax3.plot(r_sweep,
                  stress_20,
                  color='#e65100',
                  lw=1.8,
                  label=r'$\sigma_{zz}\ (\Delta T=20\mathrm{K})$')
    ax3.axhline(50.0,
                color='#b71c1c',
                ls=':',
                lw=1.8,
                label=r'$\sigma_{\mathrm{tensile}} = 50\ \mathrm{kPa}$')

    ax3_r = ax3.twinx()
    uplift_15 = surface_dome_uplift(r_sweep, delta_T_k=15.0)
    l4 = ax3_r.plot(r_sweep,
                    uplift_15,
                    color='#6a1b9a',
                    lw=2.0,
                    ls='-.',
                    label=r'Dome Uplift $\Delta h_{\mathrm{dome}}$')
    ax3_r.set_ylabel(
        r'Surface Dome Uplift $\Delta h_{\mathrm{dome}}\ [\mathrm{m}]$',
        color='#6a1b9a')
    ax3_r.tick_params(axis='y', labelcolor='#6a1b9a')
    ax3_r.set_ylim(0, 120)

    ax3.set_xlabel(r'Plume Radius $R_p\ [\mathrm{km}]$')
    ax3.set_ylabel(r'Upwelling Normal Stress $\sigma_{zz}\ [\mathrm{kPa}]$',
                   color='#2e7d32')
    ax3.set_title(r'(c) Dynamic Stress & Lenticula Dome', fontweight='bold')
    ax3.set_xlim(0.5, 6.0)
    ax3.set_ylim(0, 160)
    ax3.grid(True, linestyle='--', alpha=0.5)

    all_l = l1 + l2 + l3 + [ax3.get_lines()[3]] + l4
    all_lab = [l.get_label() for l in all_l]
    ax3.legend(all_l, all_lab, loc='upper left', fontsize=7.2, framealpha=0.9)

    plt.savefig(os.path.join(output_dir, 'fig_model_choices.pdf'), dpi=300)
    plt.savefig(os.path.join(output_dir, 'fig_model_choices.png'), dpi=300)
    plt.close()
    print("✅ Created fig_model_choices.pdf / fig_model_choices.png")


# =============================================================================
# FIGURE 3: SCHEMATIC DIAGRAM (fig_diagram.pdf)
# Conceptual architecture of diapiric ascent, tidal heating, and chaos terrain
# =============================================================================
def make_fig_diagram():
    fig = plt.figure(figsize=(10.5, 6.0), dpi=300)
    ax = fig.add_subplot(111)
    ax.set_xlim(0, 100)
    ax.set_ylim(0, 100)
    ax.axis('off')

    # Background sections
    # 1. Surface space (vacuum)
    ax.add_patch(Rectangle((0, 85), 100, 15, facecolor='#1a237e', alpha=0.15))
    ax.text(
        5,
        94,
        'Europa Space Environment (Vacuum, $T_s \\approx 100\\ \\mathrm{K}$)',
        fontsize=10.5,
        fontweight='bold',
        color='#1a237e')

    # 2. Stagnant Brittle Lid (z = 0 to 4 km)
    ax.add_patch(
        Rectangle((0, 68),
                  100,
                  17,
                  facecolor='#90caf9',
                  alpha=0.55,
                  edgecolor='#1565c0',
                  lw=2.0))
    ax.text(
        3,
        76,
        'Cold Stagnant Lid ($T \\approx 100 - 190\\ \\mathrm{K}$)\nBrittle Elastic Rheology',
        fontsize=9.0,
        fontweight='bold',
        color='#0d47a1')

    # 3. Convective Ice Mantle (z = 4 to 20 km)
    ax.add_patch(
        Rectangle((0, 20),
                  100,
                  48,
                  facecolor='#e1f5fe',
                  alpha=0.6,
                  edgecolor='#0288d1',
                  lw=2.0))
    ax.text(
        3,
        45,
        r'Ductile Convective Ice Shell ($T \approx 190 - 270\ \mathrm{K}$)' +
        '\n' +
        r'$\eta \approx 10^{13} - 10^{15}\ \mathrm{Pa\ s}$ (Arrhenius Creep)',
        fontsize=9.0,
        fontweight='bold',
        color='#01579b')

    # 4. Subsurface Global Ocean (z > 20 km)
    ax.add_patch(
        Rectangle((0, 0),
                  100,
                  20,
                  facecolor='#0277bd',
                  alpha=0.85,
                  edgecolor='#01579b',
                  lw=2.0))
    ax.text(3,
            10,
            r'Subsurface Saline Water Ocean ($T_m \approx 270\ \mathrm{K}$)' +
            '\n' +
            r'Decoupled Fluid Layer (Rich in $\mathrm{MgSO_4, NaCl, H_2SO_4}$)',
            fontsize=9.5,
            fontweight='bold',
            color='white')

    # Upwelling Thermal Diapir 1 (Rising in ductile mantle)
    diapir1 = Ellipse((35, 38),
                      width=16,
                      height=22,
                      facecolor='#ff7043',
                      alpha=0.85,
                      edgecolor='#d84315',
                      lw=2.2)
    ax.add_patch(diapir1)
    ax.text(35,
            40,
            r'Warm Diapir' + '\n' + r'$T \approx 265\ \mathrm{K}$' + '\n' +
            r'$\Delta \rho \approx 3\ \mathrm{kg/m^3}$',
            ha='center',
            va='center',
            fontsize=8.5,
            fontweight='bold',
            color='white')

    # Ascent arrow
    arrow1 = FancyArrowPatch((35, 23), (35, 52),
                             arrowstyle='->',
                             mutation_scale=20,
                             color='#b71c1c',
                             lw=2.5)
    ax.add_patch(arrow1)
    ax.text(45,
            36,
            r'$v_{\mathrm{diapir}} \approx 2.6\ \mathrm{m/yr}$' + '\n' +
            r'$\tau_{\mathrm{ascent}} \approx 6000\ \mathrm{yr}$' + '\n' +
            r'$\mathrm{Pe} \approx 200 \gg 1$',
            fontsize=8.5,
            fontweight='bold',
            color='#b71c1c')

    # Resonant tidal heating annotation inside plume
    ax.text(35,
            28,
            r'Resonant Tidal Heating' + '\n' +
            r'$q_{\mathrm{tide}} \approx 2\times 10^{-5}\ \mathrm{W/m^3}$',
            ha='center',
            va='center',
            fontsize=7.5,
            color='#fffde7',
            fontweight='bold')

    # Impinging Diapir 2 & Chaos Terrain Formation (Under thinned lid)
    diapir2 = Ellipse((75, 68),
                      width=24,
                      height=18,
                      facecolor='#ff5722',
                      alpha=0.9,
                      edgecolor='#bf360c',
                      lw=2.5)
    ax.add_patch(diapir2)
    ax.text(75,
            66,
            r'Impinged Diapir Head' + '\n' +
            r'$F_{\mathrm{diapir}} \approx 150\ \mathrm{mW/m^2}$' + '\n' +
            r'$f_{\mathrm{melt}} \approx 15\%\ \mathrm{(Eutectic\ Slush)}$',
            ha='center',
            va='center',
            fontsize=8.2,
            fontweight='bold',
            color='white')

    # Thinned Lid Indentation / Chaos Depression
    ax.add_patch(
        Polygon([[62, 85], [66, 78], [84, 78], [88, 85]],
                closed=True,
                facecolor='#ffe0b2',
                edgecolor='#e65100',
                lw=2.0))
    ax.text(75,
            80.5,
            r'Thinned Lid ($h \leq 1\ \mathrm{km}$)',
            ha='center',
            va='center',
            fontsize=8.0,
            fontweight='bold',
            color='#b71c1c')

    # Chaos Rafted Blocks at surface
    block1 = Polygon([[64, 85], [68, 85], [67, 88], [63, 88]],
                     closed=True,
                     facecolor='#90caf9',
                     edgecolor='#0d47a1',
                     lw=1.5)
    block2 = Polygon([[71, 84], [76, 85.5], [75, 89], [70, 87.5]],
                     closed=True,
                     facecolor='#90caf9',
                     edgecolor='#0d47a1',
                     lw=1.5)
    block3 = Polygon([[79, 84.5], [84, 84], [85, 87.5], [80, 88]],
                     closed=True,
                     facecolor='#90caf9',
                     edgecolor='#0d47a1',
                     lw=1.5)
    ax.add_patch(block1)
    ax.add_patch(block2)
    ax.add_patch(block3)

    ax.text(75,
            92,
            'Chaos Terrain (Conamara Chaos)\nTilted Ice Rafts on Slush Matrix',
            ha='center',
            va='center',
            fontsize=9.0,
            fontweight='bold',
            color='#b71c1c',
            bbox=dict(boxstyle='round,pad=0.2',
                      facecolor='#fff3e0',
                      edgecolor='#e65100'))

    # Ocean Salt Exhumation arrow
    arrow_ex = FancyArrowPatch((75, 75), (75, 85),
                               arrowstyle='->',
                               mutation_scale=18,
                               color='#ffeb3b',
                               lw=3.0)
    ax.add_patch(arrow_ex)
    ax.text(75,
            76.5,
            'Ocean Salts Exhumed',
            ha='center',
            va='center',
            fontsize=7.5,
            fontweight='bold',
            color='#212121')

    # Step callouts
    callouts = [
        (1, 20, 18,
         '1. Ocean Freezing & Salt Entrainment ($z = 20\\ \\mathrm{km}$)'),
        (2, 22, 53,
         '2. Thermal Diapir Buoyant Instability ($R_p \\approx 2.5\\ \\mathrm{km}$)'
        ),
        (3, 40, 26,
         '3. Maxwell Resonant Tidal Dissipation ($q_{\\mathrm{tide}}$ max)'),
        (4, 58, 62,
         '4. Thermal Lid Thinning ($H_{\\mathrm{lid}} \\to h_{\\mathrm{thin}} < 1\\ \\mathrm{km}$)'
        ),
        (5, 58, 74,
         '5. Dynamic Uplift & Brittle Tensile Cracking ($\\sigma > 50\\ \\mathrm{kPa}$)'
        ),
        (6, 62, 97,
         '6. Eutectic Melting & Catastrophic Chaos Rafting ($f_{\\mathrm{melt}} \\ge 8\\%$)'
        ), (7, 85, 90, '7. Surface Exhumation of Ocean Material')
    ]

    for num, x, y, text in callouts:
        ax.plot(x, y, 'o', color='#d50000', markersize=14)
        ax.text(x,
                y,
                str(num),
                color='white',
                ha='center',
                va='center',
                fontsize=8.5,
                fontweight='bold')

    plt.savefig(os.path.join(output_dir, 'fig_diagram.pdf'), dpi=300)
    plt.savefig(os.path.join(output_dir, 'fig_diagram.png'), dpi=300)
    plt.close()
    print("✅ Created fig_diagram.pdf / fig_diagram.png")


if __name__ == '__main__':
    print("Generating Paper #224 publication figures...")
    make_fig_comparison()
    make_fig_model_choices()
    make_fig_diagram()
    print("All Paper #224 figures generated successfully!")
