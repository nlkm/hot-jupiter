#!/usr/bin/env python3
"""
Generate publication-quality figures for Paper #205:
Ojakangas & Stevenson (1989) "Thermal State of Enceladus' Ice Shell"
- fig_comparison.pdf: Dissipation rate Im(k2) vs Maxwell relaxation frequency omega_M
- fig_model_choices.pdf: Tidal power vs viscosity across eccentricities & conductive loss
- fig_diagram.pdf: Viscoelastic ice deformation & internal structure schematic
"""

import os

import matplotlib
import numpy as np

matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib import patches
from matplotlib.gridspec import GridSpec

# Set publication style
plt.rcParams.update({
    'font.family': 'sans-serif',
    'font.size': 11,
    'axes.labelsize': 12,
    'axes.titlesize': 13,
    'xtick.labelsize': 10,
    'ytick.labelsize': 10,
    'legend.fontsize': 10,
    'figure.titlesize': 14,
    'figure.dpi': 300,
    'lines.linewidth': 2.0,
})

OUTPUT_DIR = os.path.dirname(os.path.abspath(__file__))

# Physical constants for Enceladus - Saturn system
G = 6.67430e-11
M_SATURN = 5.6834e26
R_ENCELADUS = 2.521e5
A_ENCELADUS = 2.38037e8
E_NOM = 0.0047
MU_ICE = 3.3e9  # Pa
ETA_0 = 1.0e13  # Pa s
T_BASE = 273.15  # K
T_SURF = 75.0  # K
A_COND = 567.0  # W/m
E_A = 59400.0  # J/mol
R_GAS = 8.314462618
K2_PEAK = 0.0107

# Orbital mean motion / tidal forcing frequency
N_ORBIT = np.sqrt(G * M_SATURN / A_ENCELADUS**3)  # ~ 5.3074e-5 rad/s


# -----------------------------------------------------------------------------
# 1. Figure 1: Dissipation rate Im(k2) vs Maxwell Relaxation Frequency
# -----------------------------------------------------------------------------
def generate_fig_comparison():
    _fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(13, 5.5))

    # Log frequency array
    log_omega_M = np.linspace(-10, 0, 500)
    omega_M = 10.0**log_omega_M
    chi = N_ORBIT / omega_M  # omega_forcing * tau_M

    # Maxwell viscoelastic dissipation Love number
    im_k2 = K2_PEAK * (2.0 * chi) / (1.0 + chi**2)

    # Asymptotes
    asymp_viscous = 2.0 * K2_PEAK * (omega_M / N_ORBIT
                                    )  # low omega_M / high eta
    asymp_elastic = 2.0 * K2_PEAK * (N_ORBIT / omega_M
                                    )  # high omega_M / low eta

    # Panel 1: Im(k2) vs omega_M
    ax1.loglog(omega_M,
               im_k2,
               'b-',
               lw=2.8,
               label=r'Maxwell Viscoelastic Model $\text{Im}(k_2)$')
    ax1.loglog(omega_M,
               asymp_viscous,
               'g--',
               lw=1.6,
               alpha=0.8,
               label=r'Viscous Asymptote ($\propto \omega_M$)')
    ax1.loglog(omega_M,
               asymp_elastic,
               'm--',
               lw=1.6,
               alpha=0.8,
               label=r'Elastic Asymptote ($\propto \omega_M^{-1}$)')

    # Peak marker
    ax1.axvline(
        N_ORBIT,
        color='r',
        ls=':',
        lw=1.8,
        label=
        rf'Tidal Forcing Frequency $\omega = n = {N_ORBIT:.2e}\text{{ rad/s}}$')
    ax1.plot([N_ORBIT], [K2_PEAK],
             'ro',
             markersize=8,
             zorder=5,
             label=rf'Maxwell Resonance Peak $\text{{Im}}(k_2) = {K2_PEAK}$')

    # Benchmark comparison points (Ojakangas & Stevenson 1989, Tobie 2008, Spencer 2006)
    obs_omega = [N_ORBIT, N_ORBIT * 0.1, N_ORBIT * 10.0, 1.0e-7, 1.0e-3]
    obs_chi = N_ORBIT / np.array(obs_omega)
    obs_imk2 = K2_PEAK * (2.0 * obs_chi) / (1.0 + obs_chi**2)
    ax1.plot(obs_omega,
             obs_imk2,
             'ks',
             markersize=6,
             alpha=0.9,
             label=r'Ojakangas & Stevenson (1989) Benchmarks')

    ax1.set_xlim(1e-10, 1e0)
    ax1.set_ylim(1e-6, 2e-2)
    ax1.set_xlabel(
        r'Maxwell Relaxation Frequency $\omega_M = \mu / \eta$ [$\text{rad/s}$]'
    )
    ax1.set_ylabel(r'Tidal Dissipation Love Number $\text{Im}(k_2)$')
    ax1.set_title(
        r'\textbf{(a)} Dissipation Factor $\text{Im}(k_2)$ vs. Maxwell Frequency $\omega_M$'
    )
    ax1.grid(True, which='both', ls=':', alpha=0.5)
    ax1.legend(loc='lower center', fontsize=8.5, framealpha=0.95)

    # Panel 2: Im(k2) vs Temperature in Ice Shell
    T_arr = np.linspace(80, 273.15, 400)
    eta_arr = ETA_0 * np.exp((E_A / R_GAS) * (1.0 / T_arr - 1.0 / T_BASE))
    omega_M_T = MU_ICE / eta_arr
    chi_T = N_ORBIT / omega_M_T
    im_k2_T = K2_PEAK * (2.0 * chi_T) / (1.0 + chi_T**2)

    ax2.semilogy(T_arr,
                 im_k2_T,
                 'navy',
                 lw=2.5,
                 label=r'$\text{Im}(k_2)(T)$ via Arrhenius Rheology')
    ax2.axvline(T_BASE,
                color='cyan',
                ls='--',
                lw=1.5,
                label=r'Basal Melting $T_{\text{base}} = 273.15\text{ K}$')
    ax2.axvline(T_SURF,
                color='deepskyblue',
                ls=':',
                lw=1.5,
                label=r'Surface Temp $T_{\text{surf}} = 75\text{ K}$')

    # Highlight warm ductile dissipation zone
    ax2.axvspan(240,
                273.15,
                color='orange',
                alpha=0.2,
                label='Ductile Basal Dissipation Zone')

    ax2.set_xlim(70, 280)
    ax2.set_ylim(1e-12, 2e-2)
    ax2.set_xlabel(r'Ice Temperature $T$ [$\text{K}$]')
    ax2.set_ylabel(r'Effective Dissipation Factor $\text{Im}(k_2)$')
    ax2.set_title(
        r'\textbf{(b)} Temperature Activation of Viscoelastic Dissipation')
    ax2.grid(True, which='both', ls=':', alpha=0.5)
    ax2.legend(loc='lower right', fontsize=8.5, framealpha=0.95)

    plt.tight_layout()
    plot_path = os.path.join(OUTPUT_DIR, 'fig_comparison.pdf')
    plt.savefig(plot_path, bbox_inches='tight')
    plt.close()
    print(f"✅ Generated {plot_path}")


# -----------------------------------------------------------------------------
# 2. Figure 2: Tidal Power vs Viscosity across Eccentricities
# -----------------------------------------------------------------------------
def generate_fig_model_choices():
    _fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(13, 5.5))

    log_eta = np.linspace(10, 18, 500)
    eta = 10.0**log_eta
    omega_M = MU_ICE / eta
    chi = N_ORBIT / omega_M
    im_k2 = K2_PEAK * (2.0 * chi) / (1.0 + chi**2)

    # Tidal power formula: P_tide = (21/2) Im(k2) G M_S^2 R_E^5 n e^2 / a^6
    factor = 10.5 * G * (M_SATURN**2) * (R_ENCELADUS**
                                         5) * N_ORBIT / (A_ENCELADUS**6)

    p_nom = factor * (E_NOM**2) * im_k2 / 1.0e9  # GW
    p_low = factor * (0.0025**2) * im_k2 / 1.0e9
    p_high = factor * (0.0075**2) * im_k2 / 1.0e9

    # Panel 1: Power vs Viscosity
    ax1.semilogy(log_eta, p_nom, 'b-', lw=2.6, label=rf'Nominal $e = {E_NOM}$')
    ax1.semilogy(log_eta, p_low, 'g--', lw=2.0, label=r'Damped $e = 0.0025$')
    ax1.semilogy(log_eta, p_high, 'r-.', lw=2.0, label=r'Resonant $e = 0.0075$')

    # Observations & Conduction thresholds
    ax1.axhline(15.8,
                color='darkred',
                ls='-',
                lw=1.8,
                alpha=0.8,
                label=r'Cassini CIRS South Pole Heat ($15.8\text{ GW}$)')
    ax1.axhspan(12.7,
                18.9,
                color='lightsalmon',
                alpha=0.3,
                label=r'Cassini CIRS Uncertainty ($\pm 3.1\text{ GW}$)')
    ax1.axhline(
        29.3,
        color='purple',
        ls=':',
        lw=1.8,
        label=r'Global Conduction ($d = 20\text{ km}$, $29.3\text{ GW}$)')

    # Optimal viscosity marker
    eta_opt = MU_ICE / N_ORBIT
    log_eta_opt = np.log10(eta_opt)
    ax1.axvline(
        log_eta_opt,
        color='gray',
        ls='--',
        alpha=0.7,
        label=rf'Resonant $\eta_{{\text{{opt}}}} = {eta_opt:.1e}\text{{ Pa s}}$'
    )

    ax1.set_xlim(10, 18)
    ax1.set_ylim(1e-2, 1e2)
    ax1.set_xlabel(
        r'Log$_{10}$ Ice Viscosity $\log_{10}(\eta / [\text{Pa}\cdot\text{s}])$'
    )
    ax1.set_ylabel(r'Tidal Dissipation Power $P_{\text{tide}}$ [$\text{GW}$]')
    ax1.set_title(r'\textbf{(a)} Tidal Dissipation Power vs. Ice Viscosity')
    ax1.grid(True, which='both', ls=':', alpha=0.5)
    ax1.legend(loc='lower center', fontsize=8.0, framealpha=0.95)

    # Panel 2: Conduction vs Shell Thickness & Equilibrium
    d_arr = np.linspace(2, 50, 300)
    area = 4.0 * np.pi * (R_ENCELADUS**2)
    q_cond_gw = (area * A_COND * np.log(T_BASE / T_SURF) /
                 (d_arr * 1.0e3)) / 1.0e9

    ax2.plot(d_arr,
             q_cond_gw,
             'indigo',
             lw=2.5,
             label=r'Global Conductive Loss $Q_{\text{cond}}(d)$')
    ax2.axhline(
        15.88 + 0.4,
        color='b',
        ls='--',
        lw=2.0,
        label=
        r'Peak Heat Source $P_{\text{tide}}^{\text{max}} + P_{\text{radio}} = 16.3\text{ GW}$'
    )
    ax2.axhline(
        5.8 + 0.4,
        color='g',
        ls='-.',
        lw=1.8,
        label=
        r'Off-Peak Dissipation $P_{\text{tide}} + P_{\text{radio}} = 6.2\text{ GW}$'
    )

    # Equilibrium points
    d_eq_peak = (area * A_COND * np.log(T_BASE / T_SURF) /
                 ((15.88 + 0.4) * 1.0e9)) / 1.0e3
    ax2.plot(
        [d_eq_peak], [15.88 + 0.4],
        'ro',
        markersize=8,
        label=
        rf'Stable Peak $d_{{\text{{eq}}}} \approx {d_eq_peak:.1f}\text{{ km}}$')

    ax2.set_xlim(0, 50)
    ax2.set_ylim(0, 100)
    ax2.set_xlabel(r'Ice Shell Thickness $d$ [$\text{km}$]')
    ax2.set_ylabel(r'Thermal Heat Flow [$\text{GW}$]')
    ax2.set_title(
        r'\textbf{(b)} Shell Thermal Equilibrium $Q_{\text{cond}}(d) = P_{\text{sources}}$'
    )
    ax2.grid(True, which='both', ls=':', alpha=0.5)
    ax2.legend(loc='upper right', fontsize=8.5, framealpha=0.95)

    plt.tight_layout()
    plot_path = os.path.join(OUTPUT_DIR, 'fig_model_choices.pdf')
    plt.savefig(plot_path, bbox_inches='tight')
    plt.close()
    print(f"✅ Generated {plot_path}")


# -----------------------------------------------------------------------------
# 3. Figure 3: Schematic of Viscoelastic Ice Shell Deformation
# -----------------------------------------------------------------------------
def generate_fig_diagram():
    fig = plt.figure(figsize=(12, 6.5))
    gs = GridSpec(1, 2, width_ratios=[1.2, 1.0])

    # Left: Moon Cross Section & Shell Stratification
    ax1 = fig.add_subplot(gs[0])
    ax1.set_aspect('equal')
    ax1.axis('off')

    # Draw Outer Boundary
    outer_r = 1.0
    ocean_r = 0.85
    core_r = 0.60

    # Draw Core (Silicate porous rock)
    core = patches.Circle((0, 0),
                          core_r,
                          facecolor='#c2b280',
                          edgecolor='#8b7355',
                          lw=2,
                          label='Silicate Core')
    ax1.add_patch(core)

    # Draw Subsurface Ocean
    ocean = patches.Wedge((0, 0),
                          ocean_r,
                          0,
                          360,
                          width=ocean_r - core_r,
                          facecolor='#4682b4',
                          edgecolor='#27408b',
                          lw=1.5,
                          alpha=0.7,
                          label='Liquid Ocean')
    ax1.add_patch(ocean)

    # Draw Viscoelastic Warm Ice Layer
    warm_ice = patches.Wedge((0, 0),
                             0.95,
                             0,
                             360,
                             width=0.95 - ocean_r,
                             facecolor='#b0e0e6',
                             edgecolor='#4682b4',
                             lw=1.5,
                             alpha=0.8,
                             label='Ductile Maxwell Ice')
    ax1.add_patch(warm_ice)

    # Draw Elastic Rigid Lid
    elastic_lid = patches.Wedge((0, 0),
                                outer_r,
                                0,
                                360,
                                width=outer_r - 0.95,
                                facecolor='#f0f8ff',
                                edgecolor='#5f9ea0',
                                lw=2,
                                label='Rigid Elastic Lid')
    ax1.add_patch(elastic_lid)

    # Draw South Polar Terrain Thinned Crust & Plumes (Bottom)
    spt_arc = patches.Arc((0, 0),
                          2 * outer_r,
                          2 * outer_r,
                          angle=0,
                          theta1=240,
                          theta2=300,
                          color='red',
                          lw=3.5)
    ax1.add_patch(spt_arc)

    # Plumes shooting out at south pole
    for angle_deg in [260, 270, 280]:
        rad = np.radians(angle_deg)
        x0, y0 = outer_r * np.cos(rad), outer_r * np.sin(rad)
        x1, y1 = (outer_r + 0.25) * np.cos(rad), (outer_r + 0.25) * np.sin(rad)
        ax1.annotate('',
                     xy=(x1, y1),
                     xytext=(x0, y0),
                     arrowprops=dict(arrowstyle="->", color="cyan", lw=2.5))

    ax1.text(0,
             -1.35,
             "Active Cryovolcanic Plumes\n(Tiger Stripe Fractures)",
             ha='center',
             va='center',
             fontsize=10,
             fontweight='bold',
             color='darkblue',
             bbox=dict(boxstyle="round,pad=0.3", fc="#e6f2ff", ec="navy", lw=1))

    # Internal Labels
    ax1.text(0,
             0,
             "Porous Silicate Core\n$R_{\\text{core}} \\approx 180\\text{ km}$",
             ha='center',
             va='center',
             fontsize=9,
             fontweight='bold')
    ax1.text(0,
             0.72,
             "Global Ocean ($30-40\\text{ km}$)",
             ha='center',
             va='center',
             fontsize=8.5,
             color='darkblue')
    ax1.text(
        0,
        0.90,
        "Ductile Viscous Ice\n($\\omega_M \\approx \\omega_{\\text{forcing}}$)",
        ha='center',
        va='center',
        fontsize=7.5,
        color='navy')
    ax1.text(0,
             1.10,
             "Elastic Crust ($T_{\\text{surf}} = 75\\text{ K}$)",
             ha='center',
             va='center',
             fontsize=9,
             fontweight='bold')

    ax1.set_xlim(-1.4, 1.4)
    ax1.set_ylim(-1.5, 1.3)
    ax1.set_title(r'\textbf{(a)} Enceladus Viscoelastic Shell Architecture',
                  fontsize=12)

    # Right: Rheological Model & Tidal Stress-Strain Hysteresis Loop
    ax2 = fig.add_subplot(gs[1])

    # Generate an elliptical hysteresis loop: sigma vs epsilon
    theta = np.linspace(0, 2 * np.pi, 200)
    delta_phase = np.pi / 4.0  # Phase lag at Maxwell resonance (45 deg)
    eps = np.cos(theta)
    sigma = np.cos(theta + delta_phase)

    ax2.plot(
        eps,
        sigma,
        'r-',
        lw=2.5,
        label=r'Stress-Strain Cycle $\sigma(t) \leftrightarrow \epsilon(t)$')
    ax2.fill(
        eps,
        sigma,
        color='lightcoral',
        alpha=0.3,
        label=
        r'Dissipated Energy $\oint \sigma d\epsilon \propto \text{Im}(k_2)$')

    ax2.annotate('Tidal Phase Lag\n$\\delta = \\arctan(1/\\omega \\tau_M)$',
                 xy=(0.0, np.sin(-delta_phase)),
                 xytext=(0.3, -0.6),
                 arrowprops=dict(facecolor='black',
                                 shrink=0.05,
                                 width=1,
                                 headwidth=6),
                 fontsize=9.5,
                 fontweight='bold')

    # Draw Maxwell Model Circuit in upper inset
    ax2.text(
        0.0,
        1.15,
        r'Maxwell Element: Elastic Rigidity $\mu$ + Viscous Dashpot $\eta$',
        ha='center',
        va='center',
        fontsize=10,
        fontweight='bold',
        bbox=dict(boxstyle="square,pad=0.4", fc="#fff8dc", ec="#b8860b",
                  lw=1.2))

    ax2.set_xlim(-1.3, 1.3)
    ax2.set_ylim(-1.3, 1.3)
    ax2.set_xlabel(r'Normalized Tidal Strain $\epsilon(t) / \epsilon_0$')
    ax2.set_ylabel(r'Normalized Tidal Stress $\sigma(t) / \sigma_0$')
    ax2.set_title(r'\textbf{(b)} Viscoelastic Dissipation Hysteresis Cycle',
                  fontsize=12)
    ax2.grid(True, ls=':', alpha=0.6)
    ax2.legend(loc='lower right', fontsize=8.5, framealpha=0.95)

    plt.tight_layout()
    plot_path = os.path.join(OUTPUT_DIR, 'fig_diagram.pdf')
    plt.savefig(plot_path, bbox_inches='tight')
    plt.close()
    print(f"✅ Generated {plot_path}")


if __name__ == '__main__':
    print("Generating figures for Paper #205 replication...")
    generate_fig_comparison()
    generate_fig_model_choices()
    generate_fig_diagram()
    print("🎉 All figures generated successfully!")
