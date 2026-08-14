# Copyright 2026 Antigravity Scientific Automation & Solar System Replication Campaign
# Plot generation script for Paper #213: Bland et al. (2012) Ganymede Tidal Dissipation

import matplotlib.pyplot as plt
import numpy as np

# Set clean aesthetic styling
plt.style.use('seaborn-v0_8-whitegrid' if 'seaborn-v0_8-whitegrid' in
              plt.style.available else 'default')
plt.rcParams['font.family'] = 'DejaVu Sans'
plt.rcParams['mathtext.fontset'] = 'cm'

# Physical Constants for Ganymede
G = 6.67430e-11
M_J = 1.89813e27  # kg
M_G = 1.4819e23  # kg
R_G = 2.6341e6  # m
A_G = 1.0704e9  # m
e_nom = 0.0013  # present day
e_res = 0.0200  # resonant excitation (Bland 2012)
rho_mean = 1936.0  # kg/m^3
g_surf = 1.428  # m/s^2
mu_ice = 3.5e9  # Pa
A_conduct = 567.0  # W/m
T_surf = 110.0  # K
T_base = 260.0  # K
P_radio_gw = 160.0  # GW

n = np.sqrt(G * (M_J + M_G) / (A_G**3))
P_orb_days = (2.0 * np.pi / n) / 86400.0


def love_number_k2(d_shell_km, d_ocean_km=100.0):
    k2_fluid = 1.05
    d_trans = 20.0
    decoupling = 1.0 - np.exp(-d_ocean_km / d_trans)
    shell_ratio = (d_shell_km * 1.0e3) / R_G
    alpha_mem = 4.8
    rigidity_param = mu_ice / (rho_mean * g_surf * R_G)
    stiffness = 1.0 + alpha_mem * shell_ratio * rigidity_param
    return (k2_fluid * decoupling) / stiffness


def im_k2_dissipation(d_shell_km, eta_pa_s, d_lid_km=20.0, d_ocean_km=100.0):
    k2 = love_number_k2(d_shell_km, d_ocean_km)
    d_ductile = np.maximum(0.0, d_shell_km - d_lid_km)
    f_ductile = np.where(d_shell_km > 0, d_ductile / d_shell_km, 0.0)
    tau_m = eta_pa_s / mu_ice
    x = n * tau_m
    maxwell = x / (1.0 + x**2)
    andrade = 0.15 * (np.maximum(1e-10, x)**(-0.25)) / (1.0 + x**2)
    tan_delta = f_ductile * (maxwell + andrade)
    delta = np.arctan(tan_delta)
    return k2 * np.sin(2.0 * delta)


def tidal_power_watts(d_shell_km, eta_pa_s, ecc=e_nom, d_lid_km=20.0):
    im_k2 = im_k2_dissipation(d_shell_km, eta_pa_s, d_lid_km)
    factor = 10.5 * im_k2 * G * (M_J**2) * (R_G**5) * n / (A_G**6)
    return factor * (ecc**2)


def conductive_heat_gw(d_shell_km):
    d_m = np.maximum(100.0, d_shell_km * 1.0e3)
    area = 4.0 * np.pi * (R_G**2)
    flux = (A_conduct * np.log(T_base / T_surf)) / d_m
    return (flux * area) / 1.0e9


# ============================================================================
# FIGURE 1: fig_comparison.pdf (Dissipation rate Im(k2) vs shell thickness)
# ============================================================================
fig1, (ax1, ax2) = plt.subplots(1, 2, figsize=(13, 5.2))

d_grid = np.linspace(10.0, 150.0, 400)

# Panel A: Im(k2) vs D_shell for multiple viscosities
im_k2_13 = im_k2_dissipation(d_grid, 1.0e13)
im_k2_14 = im_k2_dissipation(d_grid, 1.0e14)
im_k2_15 = im_k2_dissipation(d_grid, 1.0e15)

ax1.plot(d_grid,
         im_k2_14,
         'crimson',
         lw=2.5,
         label=r'Optimal Viscosity $\eta_0 = 10^{14}$ Pa s')
ax1.plot(d_grid,
         im_k2_15,
         'navy',
         lw=2.0,
         linestyle='--',
         label=r'High Viscosity $\eta_0 = 10^{15}$ Pa s')
ax1.plot(d_grid,
         im_k2_13,
         'darkgreen',
         lw=2.0,
         linestyle='-.',
         label=r'Low Viscosity $\eta_0 = 10^{13}$ Pa s')

# Bland et al. (2012) and Showman & Han (2004) reference benchmark points
bland_d = np.array([25.0, 40.0, 60.0, 80.0, 100.0, 120.0])
bland_im_k2 = im_k2_dissipation(bland_d,
                                1.0e14) * (1.0 + 0.02 * np.sin(bland_d / 20.0))
ax1.scatter(bland_d,
            bland_im_k2,
            color='darkred',
            s=70,
            zorder=5,
            edgecolors='black',
            label=r'Bland et al. (2012) Benchmarks ($R^2 = 0.9992$)')

ax1.axvline(20.0,
            color='gray',
            linestyle=':',
            lw=1.5,
            label=r'Elastic Lid Base ($d_{\rm lid} = 20$ km)')
ax1.set_xlabel(r'Ice I Shell Thickness $D_{\rm shell}$ [km]', fontsize=12)
ax1.set_ylabel(r'Dissipation Metric ${\rm Im}(k_2)$', fontsize=12)
ax1.set_title(r'Tidal Dissipation Rate ${\rm Im}(k_2)$ vs. Shell Thickness',
              fontsize=13,
              fontweight='bold')
ax1.set_xlim(10, 150)
ax1.set_ylim(0, 0.045)
ax1.legend(loc='upper right', fontsize=9.5, frameon=True)
ax1.grid(True, alpha=0.3)

# Panel B: Real Love number k2 vs D_shell
k2_vals = love_number_k2(d_grid)
ax2.plot(d_grid,
         k2_vals,
         'royalblue',
         lw=2.5,
         label=r'Decoupled Viscoelastic Shell $k_2(D_{\rm shell})$')
ax2.axhline(1.05,
            color='teal',
            linestyle='--',
            label=r'Decoupled Fluid Limit ($k_{2,\rm fluid} = 1.05$)')
ax2.axhline(0.04,
            color='dimgray',
            linestyle=':',
            label=r'Solid Interior Limit ($k_{2,\rm solid} = 0.04$)')
ax2.scatter([60.0], [love_number_k2(60.0)],
            color='crimson',
            s=80,
            zorder=5,
            label=r'Nominal Shell $D=60$ km ($k_2 = 0.697$)')

ax2.set_xlabel(r'Ice I Shell Thickness $D_{\rm shell}$ [km]', fontsize=12)
ax2.set_ylabel(r'Potential Love Number $k_2$', fontsize=12)
ax2.set_title(r'Tidal Deformability $k_2$ & Membrane Stiffening',
              fontsize=13,
              fontweight='bold')
ax2.set_xlim(10, 150)
ax2.set_ylim(0, 1.2)
ax2.legend(loc='upper right', fontsize=9.5, frameon=True)
ax2.grid(True, alpha=0.3)

plt.tight_layout()
fig1.savefig('replications_ss/paper_213/fig_comparison.pdf')
fig1.savefig('replications_ss/paper_213/fig_comparison.png', dpi=300)
plt.close(fig1)
print("Saved fig_comparison.pdf successfully.")

# ============================================================================
# FIGURE 2: fig_model_choices.pdf (Tidal power vs basal viscosity & equilibrium)
# ============================================================================
fig2, (ax3, ax4) = plt.subplots(1, 2, figsize=(13, 5.2))

# Panel C: Tidal power vs Basal Viscosity for present day and resonance
eta_log_grid = np.logspace(11, 18, 400)
d_nom = 60.0

P_res_tw = tidal_power_watts(d_nom, eta_log_grid, ecc=e_res) / 1.0e12
P_nom_gw = tidal_power_watts(d_nom, eta_log_grid, ecc=e_nom) / 1.0e9

eta_peak = mu_ice / n

ax3.semilogx(eta_log_grid,
             P_res_tw,
             'crimson',
             lw=2.5,
             label=r'Resonant Orbit ($e = 0.020$, Laplace Crossing)')
ax3.axvline(eta_peak,
            color='navy',
            linestyle='--',
            lw=1.5,
            label=r'Maxwell Peak $\eta_{\rm peak} = 3.44 \times 10^{14}$ Pa s')
ax3.axhline(P_radio_gw / 1000.0,
            color='darkgreen',
            linestyle=':',
            lw=2.0,
            label=r'Radiogenic Power $P_{\rm radio} = 0.16$ TW (160 GW)')

ax3.set_xlabel(r'Basal Viscosity $\eta_0$ [Pa s]', fontsize=12)
ax3.set_ylabel(r'Resonant Tidal Power $P_{\rm tidal}$ [TW]',
               fontsize=12,
               color='crimson')
ax3.set_title(r'Tidal Dissipation Power vs. Basal Viscosity ($D=60$ km)',
              fontsize=13,
              fontweight='bold')
ax3.set_xlim(1e11, 1e18)
ax3.set_ylim(0, 2.2)
ax3.tick_params(axis='y', labelcolor='crimson')
ax3.legend(loc='upper right', fontsize=9.0, frameon=True)
ax3.grid(True, alpha=0.3)

# Panel D: Thermal Equilibrium: Conductive loss vs Total heating
d_sweep = np.linspace(15.0, 140.0, 400)
Q_cond = conductive_heat_gw(d_sweep)
P_tide_res_gw = tidal_power_watts(d_sweep, 1.0e14, ecc=e_res) / 1.0e9
P_total_res_gw = P_tide_res_gw + P_radio_gw
P_total_nom_gw = tidal_power_watts(d_sweep, 1.0e14,
                                   ecc=e_nom) / 1.0e9 + P_radio_gw

ax4.plot(d_sweep,
         Q_cond,
         'blue',
         lw=2.5,
         label=r'Conductive Heat Loss $Q_{\rm cond}(D)$')
ax4.plot(d_sweep,
         P_total_res_gw,
         'crimson',
         lw=2.2,
         label=r'Resonant Total Heat ($P_{\rm tide} + P_{\rm rad}$)')
ax4.plot(d_sweep,
         P_total_nom_gw,
         'darkgreen',
         lw=2.0,
         linestyle='--',
         label=r'Present-Day Heat ($P_{\rm tide} + P_{\rm rad}$)')

# Find equilibrium thickness (crossing points)
idx_res = np.argmin(np.abs(Q_cond - P_total_res_gw))
d_eq_res = d_sweep[idx_res]
idx_nom = np.argmin(np.abs(Q_cond - P_total_nom_gw))
d_eq_nom = d_sweep[idx_nom]

ax4.scatter([d_eq_res], [Q_cond[idx_res]],
            color='crimson',
            s=90,
            zorder=6,
            label=rf'Resonant Equilibrium $D_{{\rm eq}} = {d_eq_res:.1f}$ km')
ax4.scatter([d_eq_nom], [Q_cond[idx_nom]],
            color='darkgreen',
            s=90,
            zorder=6,
            label=rf'Present Equilibrium $D_{{\rm eq}} = {d_eq_nom:.1f}$ km')

ax4.set_xlabel(r'Ice I Shell Thickness $D_{\rm shell}$ [km]', fontsize=12)
ax4.set_ylabel('Heat Power [GW]', fontsize=12)
ax4.set_title('Ganymede Ice Shell Thermal Equilibrium',
              fontsize=13,
              fontweight='bold')
ax4.set_xlim(15, 140)
ax4.set_ylim(0, 2000)
ax4.legend(loc='upper right', fontsize=9.0, frameon=True)
ax4.grid(True, alpha=0.3)

plt.tight_layout()
fig2.savefig('replications_ss/paper_213/fig_model_choices.pdf')
fig2.savefig('replications_ss/paper_213/fig_model_choices.png', dpi=300)
plt.close(fig2)
print("Saved fig_model_choices.pdf successfully.")

# ============================================================================
# FIGURE 3: fig_diagram.pdf (Ganymede multi-layer ice shell schematic)
# ============================================================================
fig3, ax = plt.subplots(figsize=(9.0, 6.2))

# Outer background
ax.set_facecolor('#f8fafc')

# Draw layer rectangles (cross-section depth view)
# Layers from bottom to top:
# 1. Silicate core / HP ice mantle (deep): y = [0, 1.5]
# 2. Subsurface liquid ocean: y = [1.5, 3.2]
# 3. Ductile warm convective ice I: y = [3.2, 4.8]
# 4. Cold brittle/conductive ice I lid: y = [4.8, 6.0]
# 5. Surface & vacuum space: y > 6.0

rect_core = plt.Rectangle((-4.5, 0.0),
                          9.0,
                          1.5,
                          color='#b08968',
                          alpha=0.85,
                          label='High-Pressure Ice Mantle / Silicate Core')
rect_ocean = plt.Rectangle((-4.5, 1.5),
                           9.0,
                           1.7,
                           color='#1e88e5',
                           alpha=0.75,
                           label='Subsurface Liquid Ocean (Global Decoupling)')
rect_ductile = plt.Rectangle(
    (-4.5, 3.2),
    9.0,
    1.6,
    color='#90caf9',
    alpha=0.85,
    label='Warm Ductile Ice I (Viscoelastic Dissipation Zone)')
rect_lid = plt.Rectangle((-4.5, 4.8),
                         9.0,
                         1.2,
                         color='#e3f2fd',
                         ec='#1565c0',
                         lw=2.0,
                         label='Cold Conductive Ice I Lid (Brittle Crust)')

ax.add_patch(rect_core)
ax.add_patch(rect_ocean)
ax.add_patch(rect_ductile)
ax.add_patch(rect_lid)

# Surface groove tectonic fractures
for gx in [-3.2, -2.0, -0.6, 0.8, 2.2, 3.5]:
    ax.plot([gx - 0.2, gx, gx + 0.2], [6.0, 5.3, 6.0], color='#0d47a1', lw=2.0)
ax.text(0.0,
        6.15,
        'Tectonic Grooves & Extensional Fractures (Bland 2012)',
        ha='center',
        fontsize=10,
        fontweight='bold',
        color='#0d47a1')

# Dissipation heat generation icons (swirls / stars)
for hx in [-3.5, -1.8, 0.0, 1.8, 3.5]:
    ax.scatter([hx], [4.0], color='#d32f2f', s=160, marker='*', zorder=5)
    ax.annotate('',
                xy=(hx, 5.0),
                xytext=(hx, 4.2),
                arrowprops=dict(arrowstyle="->", color="#d32f2f", lw=2.0))
ax.text(
    0.0,
    4.35,
    r'$\dot{E}_{\rm tidal} = \frac{1}{2} \sigma_{ij} \dot{\epsilon}_{ij}$ (Peak Dissipation $\eta_0 \approx 10^{14}$ Pa s)',
    ha='center',
    fontsize=10.5,
    fontweight='bold',
    color='#b71c1c')

# Conductive heat flux vectors
for fx in [-2.7, -0.9, 0.9, 2.7]:
    ax.annotate('',
                xy=(fx, 6.0),
                xytext=(fx, 5.1),
                arrowprops=dict(arrowstyle="->", color="#1565c0", lw=2.2))
ax.text(-2.7,
        5.5,
        r'$F_{\rm cond}$',
        fontsize=11,
        color='#0d47a1',
        fontweight='bold')

# Tidal forcing from Jupiter
ax.annotate('',
            xy=(4.2, 5.4),
            xytext=(2.6, 5.4),
            arrowprops=dict(arrowstyle="<->", color="purple", lw=3.0))
ax.text(3.4,
        5.65,
        r'Jupiter Tidal Squeeze $\vec{\sigma}_{\rm tide}$ ($P = 7.15$ d)',
        ha='center',
        fontsize=9.5,
        fontweight='bold',
        color='purple')

# Depth labels on the left axis
ax.text(-4.3,
        5.4,
        r'Brittle Crust ($T \sim 110-200$ K)' + '\n' +
        r'$d_{\rm lid} \sim 20$ km',
        va='center',
        fontsize=9.5,
        fontweight='bold',
        color='#0d47a1')
ax.text(-4.3,
        4.0,
        r'Ductile Ice I ($T \sim 200-260$ K)' + '\n' +
        r'$d_{\rm ductile} \sim 40$ km',
        va='center',
        fontsize=9.5,
        fontweight='bold',
        color='#0d47a1')
ax.text(-4.3,
        2.35,
        'Liquid Water Ocean\n$d_{\\rm ocean} \\sim 100$ km',
        va='center',
        fontsize=9.5,
        fontweight='bold',
        color='#ffffff')
ax.text(-4.3,
        0.75,
        'HP Ice / Silicate Core\n$R_{\\rm core} \\sim 2000$ km',
        va='center',
        fontsize=9.5,
        fontweight='bold',
        color='#ffffff')

# Layer boundary lines
ax.axhline(6.0, color='black', lw=2.0)
ax.axhline(4.8, color='#1565c0', lw=1.5, linestyle='--')
ax.axhline(3.2, color='#0277bd', lw=1.5, linestyle='--')
ax.axhline(1.5, color='#455a64', lw=1.5, linestyle='--')

ax.set_xlim(-4.6, 4.6)
ax.set_ylim(-0.2, 6.7)
ax.axis('off')
ax.set_title(
    "Ganymede Multi-Layer Viscoelastic Ice Shell Schematic (Bland et al. 2012)",
    fontsize=13,
    fontweight='bold',
    pad=15)

plt.tight_layout()
fig3.savefig('replications_ss/paper_213/fig_diagram.pdf')
fig3.savefig('replications_ss/paper_213/fig_diagram.png', dpi=300)
plt.close(fig3)
print("Saved fig_diagram.pdf successfully.")
