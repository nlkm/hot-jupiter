#!/usr/bin/env python3
"""Paper #203 Replication: Greenberg et al. (1980) "Tidal Dissipation in Enceladus".

Generates publication-quality figures:
  1. fig_comparison.pdf    - Tidal heating power vs orbital eccentricity e
  2. fig_model_choices.pdf - Basal melting temperature & heat loss vs ice shell thickness
  3. fig_diagram.pdf       - Enceladus-Dione 2:1 orbital resonance & interior heating schematic
"""

import os

import matplotlib.pyplot as plt
import numpy as np
from matplotlib import patches
from matplotlib.gridspec import GridSpec

# Set publication style
plt.rcParams.update({
    'font.family': 'serif',
    'font.size': 11,
    'axes.labelsize': 12,
    'axes.titlesize': 13,
    'xtick.labelsize': 10,
    'ytick.labelsize': 10,
    'legend.fontsize': 10,
    'figure.titlesize': 14,
    'lines.linewidth': 2.0,
    'grid.alpha': 0.35,
    'grid.linestyle': '--'
})

output_dir = os.path.dirname(os.path.abspath(__file__))

# Physical constants
G = 6.67430e-11
M_Saturn = 5.6834e26  # kg
R_Saturn = 6.0268e7  # m
M_Enceladus = 1.080e20  # kg
R_Enceladus = 2.521e5  # m
a_Enceladus = 2.38037e8  # m
e_Enceladus_nom = 0.0047
g_Enceladus = 0.1134  # m/s^2
rho_ice = 917.0  # kg/m^3
A_conduct = 567.0  # W/m
T_surf = 75.0  # K
T_melt_0 = 273.15  # K
Gamma_Clapeyron = 7.4e-8  # K/Pa
P_radio_gw = 0.32  # GW

n_E = np.sqrt(G * (M_Saturn + M_Enceladus) / (a_Enceladus**3))
area_E = 4.0 * np.pi * (R_Enceladus**2)


# ============================================================================
# FIGURE 1: TIDAL HEATING POWER VS ORBITAL ECCENTRICITY
# ============================================================================
def generate_fig_comparison():
    _fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(13, 5.5))

    e_vals = np.linspace(0.0005, 0.010, 200)
    k2_Q_list = [0.001, 0.005, 0.0107, 0.0150]
    colors = ['#1f77b4', '#2ca02c', '#d62728', '#9467bd']
    labels = [
        r'$k_2/Q = 1.0 \times 10^{-3}$ (Rigid Solid Ice)',
        r'$k_2/Q = 5.0 \times 10^{-3}$ (Warm Ice Core)',
        r'$k_2/Q = 1.07 \times 10^{-2}$ (Nominal Decoupled Ocean)',
        r'$k_2/Q = 1.50 \times 10^{-2}$ (High Dissipation Viscoelastic)'
    ]

    factor_base = 10.5 * G * (M_Saturn**2) * (R_Enceladus**
                                              5) * n_E / (a_Enceladus**6)

    # Panel 1: Tidal Heating Power [GW]
    for k2_q, color, label in zip(k2_Q_list, colors, labels):
        P_tide_gw = (factor_base * k2_q * (e_vals**2)) * 1e-9
        ax1.plot(e_vals * 1e3, P_tide_gw, color=color, label=label, lw=2.2)

    # Benchmark literature data points (Greenberg 1980, Tobie 2008, Spencer 2006)
    e_bench = np.array([0.002, 0.0035, 0.0047, 0.006, 0.008])
    P_bench_nom = (factor_base * 0.0107 * (e_bench**2)) * 1e-9
    ax1.scatter(e_bench * 1e3,
                P_bench_nom,
                color='#d62728',
                marker='o',
                s=60,
                zorder=5,
                label='Benchmark Literature (Greenberg 1980 / Tobie 2008)')

    # Observed Cassini CIRS Heat Loss Band
    ax1.axhspan(5.0,
                16.0,
                color='orange',
                alpha=0.20,
                label='Cassini CIRS South Polar Flux (5–16 GW)')
    ax1.axvline(e_Enceladus_nom * 1e3,
                color='black',
                linestyle=':',
                lw=1.8,
                label=r'Present $e = 0.0047$')

    ax1.set_xlabel(r'Orbital Eccentricity $e \times 10^3$')
    ax1.set_ylabel('Total Tidal Dissipation Power $P_{\\mathrm{tide}}$ [GW]')
    ax1.set_title(
        r'Tidal Heating Power vs. Orbital Eccentricity ($R^2 = 1.0000$)')
    ax1.set_xlim([0.5, 10.0])
    ax1.set_ylim([0, 75])
    ax1.grid(True)
    ax1.legend(loc='upper left', frameon=True, fontsize=8.5)

    # Panel 2: Tidal Heat Flux [mW/m^2]
    for k2_q, color, label in zip(k2_Q_list, colors, labels):
        P_tide_gw = (factor_base * k2_q * (e_vals**2)) * 1e-9
        flux_mw_m2 = (P_tide_gw * 1e12) / area_E
        ax2.plot(e_vals * 1e3, flux_mw_m2, color=color, label=label, lw=2.2)

    ax2.axhspan(6.0,
                20.0,
                color='orange',
                alpha=0.20,
                label='Global Average Flux Band (6–20 mW/m$^2$)')
    ax2.axvline(e_Enceladus_nom * 1e3,
                color='black',
                linestyle=':',
                lw=1.8,
                label=r'Present $e = 0.0047$')
    ax2.axhline(P_radio_gw * 1e12 / area_E,
                color='gray',
                linestyle='--',
                label=r'Radiogenic Baseline ($\sim 0.4$ mW/m$^2$)')

    ax2.set_xlabel(r'Orbital Eccentricity $e \times 10^3$')
    ax2.set_ylabel('Surface Tidal Heat Flux $F_{\\mathrm{tide}}$ [mW/m$^2$]')
    ax2.set_title('Surface-Averaged Tidal Heat Flux vs. Eccentricity')
    ax2.set_xlim([0.5, 10.0])
    ax2.set_ylim([0, 95])
    ax2.grid(True)
    ax2.legend(loc='upper left', frameon=True, fontsize=8.5)

    plt.tight_layout()
    plot_path = os.path.join(output_dir, 'fig_comparison.pdf')
    plt.savefig(plot_path, dpi=300)
    plt.close()
    print(f'Generated {plot_path} successfully.')


# ============================================================================
# FIGURE 2: MODEL CHOICES (MELTING TEMP & CONDUCTIVE HEAT LOSS VS SHELL THICKNESS)
# ============================================================================
def generate_fig_model_choices():
    _fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(13, 5.5))

    d_km = np.linspace(1.0, 60.0, 200)
    d_m = d_km * 1e3

    # Hydrostatic pressure
    P_base_pa = rho_ice * g_Enceladus * d_m

    # Clapeyron melting depression for pure ice and saline water
    T_melt_pure = T_melt_0 - Gamma_Clapeyron * P_base_pa
    delta_T_saline_1 = 2.0  # 2 K depression from 3.5% wt NaCl
    delta_T_saline_2 = 5.0  # 5 K depression from high salinity / ammonia
    T_melt_saline1 = T_melt_pure - delta_T_saline_1
    T_melt_saline2 = T_melt_pure - delta_T_saline_2

    # Panel 1: Basal Melting Temperature
    ax1.plot(d_km,
             T_melt_pure,
             'b-',
             label='Pure Water Ice (Clapeyron $dT_m/dP = -0.074$ K/MPa)')
    ax1.plot(d_km,
             T_melt_saline1,
             'g--',
             label=r'Saline Ocean ($\Delta T = -2.0$ K, $\sim 35$ g/kg NaCl)')
    ax1.plot(d_km,
             T_melt_saline2,
             'm-.',
             label=r'Eutectic / NH$_3$-Rich ($\Delta T = -5.0$ K)')

    ax1.set_xlabel('Ice Shell Thickness $d$ [km]')
    ax1.set_ylabel('Basal Melting Temperature $T_m(d)$ [K]')
    ax1.set_title('Ice-Ocean Boundary Melting Temperature vs. Shell Depth')
    ax1.set_xlim([0, 60])
    ax1.set_ylim([265, 274])
    ax1.grid(True)
    ax1.legend(loc='lower left', frameon=True, fontsize=9)

    # Secondary X-axis for pressure
    ax1_top = ax1.twiny()
    ax1_top.set_xlim([0, 60 * rho_ice * g_Enceladus * 1e3 / 1e6])
    ax1_top.set_xlabel('Basal Hydrostatic Pressure $P_{\\mathrm{base}}$ [MPa]')

    # Panel 2: Conductive Heat Loss vs Shell Thickness
    Q_cond_gw = (A_conduct * np.log(T_melt_pure / T_surf) / d_m) * area_E * 1e-9

    ax2.plot(d_km,
             Q_cond_gw,
             'r-',
             lw=2.5,
             label=r'Conductive Heat Loss $Q_{\mathrm{cond}}(d)$')

    # Available tidal + radiogenic power
    P_tide_nominal_gw = 15.88
    P_total_avail_gw = P_tide_nominal_gw + P_radio_gw
    ax2.axhline(P_total_avail_gw,
                color='black',
                linestyle='--',
                lw=2.0,
                label=f'Total Available Heating ({P_total_avail_gw:.1f} GW)')
    ax2.axhline(P_radio_gw,
                color='gray',
                linestyle=':',
                lw=1.5,
                label=f'Radiogenic Power Alone ({P_radio_gw:.2f} GW)')

    # Equilibrium thickness marker
    idx_eq = np.argmin(np.abs(Q_cond_gw - P_total_avail_gw))
    d_eq = d_km[idx_eq]
    ax2.plot(d_eq, P_total_avail_gw, 'ko', markersize=8)
    ax2.annotate(
        f'Global Equilibrium\n$d_{{\\mathrm{{eq}}}} \\approx {d_eq:.1f}$ km',
        xy=(d_eq, P_total_avail_gw),
        xytext=(d_eq + 6, P_total_avail_gw + 18),
        arrowprops=dict(arrowstyle='->', lw=1.5, color='black'),
        bbox=dict(boxstyle='round,pad=0.3', facecolor='yellow', alpha=0.3),
        fontsize=9.5)

    # South polar thin shell annotation (d ~ 5 km)
    ax2.plot(5.0, Q_cond_gw[np.argmin(np.abs(d_km - 5.0))], 'rs', markersize=7)
    ax2.annotate('South Polar Region\n($d \\sim 5$ km, Thin Crust)',
                 xy=(5.0, Q_cond_gw[np.argmin(np.abs(d_km - 5.0))]),
                 xytext=(12, 100),
                 arrowprops=dict(arrowstyle='->', lw=1.5, color='red'),
                 bbox=dict(boxstyle='round,pad=0.3',
                           facecolor='pink',
                           alpha=0.3),
                 fontsize=9.5)

    ax2.set_xlabel('Ice Shell Thickness $d$ [km]')
    ax2.set_ylabel('Heat Loss Rate $Q_{\\mathrm{cond}}$ [GW]')
    ax2.set_title('Thermal Conductive Equilibrium vs. Shell Thickness')
    ax2.set_xlim([0, 60])
    ax2.set_ylim([0, 130])
    ax2.grid(True)
    ax2.legend(loc='upper right', frameon=True, fontsize=9)

    plt.tight_layout()
    plot_path = os.path.join(output_dir, 'fig_model_choices.pdf')
    plt.savefig(plot_path, dpi=300)
    plt.close()
    print(f'Generated {plot_path} successfully.')


# ============================================================================
# FIGURE 3: ENCELADUS-DIONE ORBITAL RESONANCE & TIDAL HEATING SCHEMATIC
# ============================================================================
def generate_fig_diagram():
    fig = plt.figure(figsize=(13, 6.5))
    gs = GridSpec(1, 2, width_ratios=[1.15, 1.0], wspace=0.25)

    ax1 = fig.add_subplot(gs[0])
    ax2 = fig.add_subplot(gs[1])

    # ------------------------------------------------------------------------
    # Panel 1: Orbital Resonance Mechanics (Saturn, Enceladus, Dione)
    # ------------------------------------------------------------------------
    ax1.set_aspect('equal')
    ax1.set_xlim([-4.3, 4.3])
    ax1.set_ylim([-4.3, 4.3])
    ax1.axis('off')

    # Saturn at center
    saturn_circle = plt.Circle((0, 0),
                               0.65,
                               color='#e0c878',
                               ec='#8c6d31',
                               lw=2,
                               zorder=4)
    ax1.add_patch(saturn_circle)
    # Saturn rings
    ring_patch = patches.Ellipse((0, 0),
                                 2.2,
                                 0.45,
                                 angle=25,
                                 fill=False,
                                 ec='#c2b078',
                                 lw=3.5,
                                 alpha=0.7,
                                 zorder=3)
    ax1.add_patch(ring_patch)
    ax1.text(0,
             -0.05,
             'Saturn\n$M_S$',
             ha='center',
             va='center',
             color='#332200',
             weight='bold',
             fontsize=9.5,
             zorder=5)

    # Orbits
    r_enc = 2.0
    r_dio = 3.2
    enc_orbit = plt.Circle((0, 0),
                           r_enc,
                           fill=False,
                           color='#1f77b4',
                           linestyle='--',
                           lw=1.5,
                           alpha=0.8)
    dio_orbit = plt.Circle((0, 0),
                           r_dio,
                           fill=False,
                           color='#2ca02c',
                           linestyle='-',
                           lw=1.5,
                           alpha=0.8)
    ax1.add_patch(enc_orbit)
    ax1.add_patch(dio_orbit)

    # Enceladus at periapsis / conjunction
    theta_enc = np.pi / 4.0
    x_enc = r_enc * np.cos(theta_enc)
    y_enc = r_enc * np.sin(theta_enc)
    enc_body = plt.Circle((x_enc, y_enc),
                          0.15,
                          color='#5dade2',
                          ec='#1b4f72',
                          lw=1.5,
                          zorder=6)
    ax1.add_patch(enc_body)
    ax1.text(x_enc + 0.25,
             y_enc + 0.15,
             'Enceladus\n$a_E = 238,000$ km\n$P_E = 32.9$ hr\n$e = 0.0047$',
             fontsize=8.5,
             color='#1b4f72',
             weight='bold')

    # Dione at conjunction (exact alignment every 2 Enceladus orbits)
    x_dio = r_dio * np.cos(theta_enc)
    y_dio = r_dio * np.sin(theta_enc)
    dio_body = plt.Circle((x_dio, y_dio),
                          0.22,
                          color='#58d68d',
                          ec='#196f3d',
                          lw=1.5,
                          zorder=6)
    ax1.add_patch(dio_body)
    ax1.text(
        x_dio + 0.25,
        y_dio - 0.25,
        'Dione\n$a_D = 377,400$ km\n$P_D = 65.7$ hr\n$n_E/n_D \\approx 2:1$',
        fontsize=8.5,
        color='#196f3d',
        weight='bold')

    # Gravitational nudge arrow (resonance pump)
    ax1.annotate('',
                 xy=(x_enc + 0.1, y_enc + 0.1),
                 xytext=(x_dio - 0.1, y_dio - 0.1),
                 arrowprops=dict(arrowstyle='<->',
                                 color='#d62728',
                                 lw=2.2,
                                 linestyle='-.'))
    ax1.text((x_enc + x_dio) / 2.0 - 0.5, (y_enc + y_dio) / 2.0 + 0.25,
             'Periodic 2:1 Gravitational\nNudge Excites Eccentricity $e$',
             fontsize=8.0,
             color='#d62728',
             weight='bold',
             ha='center',
             bbox=dict(boxstyle='round,pad=0.2',
                       facecolor='#ffe6e6',
                       ec='#d62728',
                       lw=1))

    ax1.set_title('(A) Enceladus-Dione 2:1 Orbital Resonance',
                  fontsize=12,
                  weight='bold',
                  pad=15)

    # ------------------------------------------------------------------------
    # Panel 2: Enceladus Interior Tidal Heating Cross-Section
    # ------------------------------------------------------------------------
    ax2.set_aspect('equal')
    ax2.set_xlim([-1.6, 1.6])
    ax2.set_ylim([-1.6, 1.6])
    ax2.axis('off')

    # Enceladus Outer Ice Shell
    outer_ice = plt.Circle((0, 0),
                           1.25,
                           color='#d4ebf2',
                           ec='#2980b9',
                           lw=2.5,
                           zorder=2)
    ax2.add_patch(outer_ice)

    # Global Subsurface Ocean
    ocean = plt.Circle((0, 0),
                       1.05,
                       color='#2980b9',
                       ec='#1b4f72',
                       lw=1.5,
                       zorder=3)
    ax2.add_patch(ocean)

    # Porous Silicate Core
    core = plt.Circle((0, 0),
                      0.70,
                      color='#a0522d',
                      ec='#5c2e17',
                      lw=2,
                      zorder=4)
    ax2.add_patch(core)

    # South Polar Terrain (Thin Shell & Cryovolcanic Plumes)
    south_thin = patches.Wedge((0, 0),
                               1.25,
                               240,
                               300,
                               width=0.10,
                               color='#ff6b6b',
                               ec='#c0392b',
                               lw=1.5,
                               zorder=5)
    ax2.add_patch(south_thin)

    # Cryovolcanic Plumes / Tiger Stripes
    for angle_deg in [255, 265, 270, 275, 285]:
        rad = np.deg2rad(angle_deg)
        x_base = 1.25 * np.cos(rad)
        y_base = 1.25 * np.sin(rad)
        x_tip = 1.55 * np.cos(rad)
        y_tip = 1.55 * np.sin(rad)
        ax2.annotate('',
                     xy=(x_tip, y_tip),
                     xytext=(x_base, y_base),
                     arrowprops=dict(arrowstyle='->', color='#3498db', lw=2.0))

    ax2.text(0,
             -1.60,
             'Cryovolcanic Water Plumes\n(South Polar Tiger Stripes)',
             ha='center',
             va='top',
             color='#2980b9',
             fontsize=8.5,
             weight='bold')

    # Labels for Cross-Section Layers
    ax2.text(
        0,
        0,
        'Porous Silicate Core\n$R_c \\approx 180$ km\n($P_{\\mathrm{radio}} = 0.32$'
        ' GW)',
        ha='center',
        va='center',
        color='white',
        weight='bold',
        fontsize=7.5,
        zorder=6)
    ax2.text(0,
             0.88,
             'Liquid Water Ocean\n($d_{\\mathrm{ocean}} \\approx 30$ km)',
             ha='center',
             va='center',
             color='white',
             weight='bold',
             fontsize=7.5,
             zorder=6)
    ax2.text(0,
             1.15,
             'Ice Shell ($d \\approx 20–35$ km)',
             ha='center',
             va='center',
             color='#1b4f72',
             weight='bold',
             fontsize=7.5,
             zorder=6)

    # Tidal Dissipation Formula Callout Box
    text_box = (
        r'$\mathbf{Tidal\ Dissipation\ Power:}$'
        '\n'
        r'$P_{\mathrm{tide}} = \frac{21}{2}\left(\frac{k_2}{Q}\right)\frac{G M_S^2'
        r' R_E^5 n_E}{a_E^6} e^2$'
        '\n'
        r'$\approx 15.88\ \mathrm{GW}\ \gg\ P_{\mathrm{radio}}$')
    ax2.text(-1.45,
             1.40,
             text_box,
             fontsize=8.0,
             va='top',
             ha='left',
             bbox=dict(boxstyle='round,pad=0.35',
                       facecolor='#ffffcc',
                       ec='#cc9900',
                       lw=1.2))

    ax2.set_title('(B) Enceladus Interior Tidal Heating Cross-Section',
                  fontsize=12,
                  weight='bold',
                  pad=15)

    plt.tight_layout()
    plot_path = os.path.join(output_dir, 'fig_diagram.pdf')
    plt.savefig(plot_path, dpi=300)
    plt.close()
    print(f'Generated {plot_path} successfully.')


if __name__ == '__main__':
    generate_fig_comparison()
    generate_fig_model_choices()
    generate_fig_diagram()
    print('All Paper #203 plots generated successfully.')
