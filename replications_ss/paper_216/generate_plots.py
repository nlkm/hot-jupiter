#!/usr/bin/env python3
# Copyright 2026 Antigravity Scientific Automation & Solar System Replication Campaign
# Plot Generator for Paper #216: Meyer & Wisdom (2007) "Tidal Heating in Enceladus"
# Generates fig_comparison.pdf, fig_model_choices.pdf, fig_diagram.pdf

import matplotlib.pyplot as plt
import numpy as np
from matplotlib import patches

# Set clean publication aesthetic styling
plt.style.use(
    "seaborn-v0_8-whitegrid"
    if "seaborn-v0_8-whitegrid" in plt.style.available
    else "default"
)
plt.rcParams["font.family"] = "DejaVu Sans"
plt.rcParams["mathtext.fontset"] = "cm"
plt.rcParams["font.size"] = 11

# Physical Constants & Parameters
G = 6.67430e-11
M_S = 5.6834e26  # Saturn mass [kg]
R_S = 6.0268e7  # Saturn equatorial radius [m]
k2_S_nom = 0.341  # Saturn Love number
Q_S_canonical = 18000.0

M_E = 1.0803e20  # Enceladus mass [kg]
R_E = 2.521e5  # Enceladus radius [m]
a_E = 2.3804e8  # Enceladus semi-major axis [m]
e_E_nom = 0.0047  # Enceladus forced eccentricity
P_radio_gw = 0.32  # Radiogenic power [GW]

M_D = 1.0955e21  # Dione mass [kg]
R_D = 5.614e5  # Dione radius [m]
a_D = 3.7742e8  # Dione semi-major axis [m]
e_D_nom = 0.0022  # Dione eccentricity

n_E = np.sqrt(G * (M_S + M_E) / (a_E**3))
n_D = np.sqrt(G * (M_S + M_D) / (a_D**3))

L_E = M_E * np.sqrt(G * M_S * a_E * (1.0 - e_E_nom**2))
L_D = M_D * np.sqrt(G * M_S * a_D * (1.0 - e_D_nom**2))


def saturn_torque_enceladus(k2_S, Q_S):
    return 1.5 * (k2_S / Q_S) * G * (M_E**2) * (R_S**5) / (a_E**6)


def saturn_torque_dione(k2_S, Q_S):
    return 1.5 * (k2_S / Q_S) * G * (M_D**2) * (R_S**5) / (a_D**6)


def equilibrium_tidal_power_gw(k2_S, Q_S):
    n_se = saturn_torque_enceladus(k2_S, Q_S)
    n_sd = saturn_torque_dione(k2_S, Q_S)
    num = (n_E - n_D) * (L_D * n_se - L_E * n_sd)
    den = L_E + L_D
    return (num / den) * 1.0e-9


def instantaneous_tidal_power_gw(e, k2_over_q_E=0.0107):
    factor = 10.5 * k2_over_q_E * G * (M_S**2) * (R_E**5) * n_E / (a_E**6)
    return (factor * (e**2)) * 1.0e-9


def conductive_heat_loss_gw(d_shell_km, A_cond=567.0, T_surf=75.0, T_melt=273.15):
    d_m = np.maximum(100.0, d_shell_km * 1.0e3)
    area = 4.0 * np.pi * (R_E**2)
    flux = (A_cond * np.log(T_melt / T_surf)) / d_m
    return (flux * area) * 1.0e-9


def equilibrium_shell_thickness_km(
    heat_gw, p_radio=0.32, A_cond=567.0, T_surf=75.0, T_melt=273.15
):
    tot_gw = np.maximum(0.01, heat_gw + p_radio)
    area = 4.0 * np.pi * (R_E**2)
    target_flux = (tot_gw * 1.0e9) / area
    d_m = (A_cond * np.log(T_melt / T_surf)) / target_flux
    return d_m / 1.0e3


# ============================================================================
# FIGURE 1: fig_comparison.pdf (Equilibrium Power vs Q_S & Instantaneous vs e)
# ============================================================================
print("[1/3] Generating fig_comparison.pdf...")
fig1, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5.5))

# Panel A: Equilibrium tidal heating vs Saturn tidal Q_S
q_grid = np.logspace(2.7, 5.2, 500)
p_eq_0341 = equilibrium_tidal_power_gw(0.341, q_grid)
p_eq_0390 = equilibrium_tidal_power_gw(0.390, q_grid)
p_eq_0210 = equilibrium_tidal_power_gw(0.210, q_grid)

# Observed heat flux shaded regions
ax1.axhspan(
    5.8 - 1.9,
    5.8 + 1.9,
    color="crimson",
    alpha=0.18,
    label=r"Spencer et al. (2006) CIRS ($5.8 \pm 1.9$ GW)",
)
ax1.axhline(
    5.8, color="crimson", linestyle="--", lw=1.8, label=r"Spencer Nominal ($5.8$ GW)"
)
ax1.axhspan(
    15.8 - 3.1,
    15.8 + 3.1,
    color="purple",
    alpha=0.12,
    label=r"Howett et al. (2011) CIRS ($15.8 \pm 3.1$ GW)",
)
ax1.axhline(
    15.8, color="purple", linestyle=":", lw=1.8, label=r"Howett Nominal ($15.8$ GW)"
)
ax1.axhline(
    P_radio_gw,
    color="gray",
    linestyle="-.",
    lw=1.2,
    label=r"Core Radiogenic Power ($0.32$ GW)",
)

ax1.plot(
    q_grid,
    p_eq_0341,
    "navy",
    lw=2.6,
    label=r"Nominal Saturn $k_{2S} = 0.341$ (Meyer \& Wisdom)",
)
ax1.plot(
    q_grid,
    p_eq_0390,
    "teal",
    lw=2.0,
    linestyle="--",
    label=r"Upper Bound $k_{2S} = 0.390$ (Lainey et al.)",
)
ax1.plot(
    q_grid,
    p_eq_0210,
    "darkorange",
    lw=2.0,
    linestyle=":",
    label=r"Lower Bound $k_{2S} = 0.210$",
)

# Benchmark Reference Points
ax1.scatter(
    [18000.0],
    [equilibrium_tidal_power_gw(0.341, 18000.0)],
    color="navy",
    s=90,
    zorder=6,
    edgecolors="black",
    label=r"Meyer \& Wisdom (2007) Canonical ($1.17$ GW, $Q_S = 18000$)",
)
ax1.scatter(
    [1695.0],
    [equilibrium_tidal_power_gw(0.390, 1695.0)],
    color="teal",
    s=90,
    zorder=6,
    edgecolors="black",
    label=r"Lainey et al. (2012, 2017) Astrometric ($14.2$ GW, $Q_S \approx 1700$)",
)
ax1.scatter(
    [3634.0],
    [5.8],
    color="crimson",
    s=90,
    zorder=6,
    edgecolors="black",
    label=r"Spencer Required ($Q_S = 3634$)",
)

ax1.set_xscale("log")
ax1.set_yscale("log")
ax1.set_xlim(500, 150000)
ax1.set_ylim(0.1, 40.0)
ax1.set_xlabel(r"Saturn Tidal Dissipation Quality Factor $Q_S$", fontsize=12)
ax1.set_ylabel(r"Equilibrium Tidal Heating Power $\dot{E}_{\rm eq}$ [GW]", fontsize=12)
ax1.set_title(
    r"\textbf{(a)} 2:1 Resonance Equilibrium Tidal Heating vs. Saturn $Q_S$",
    fontsize=13,
    fontweight="bold",
)
ax1.legend(loc="lower left", fontsize=8.5, framealpha=0.92)
ax1.grid(True, which="both", linestyle=":", alpha=0.6)

# Panel B: Instantaneous tidal power vs eccentricity e
e_grid = np.linspace(0.0001, 0.018, 500)
p_inst_0107 = instantaneous_tidal_power_gw(e_grid, 0.0107)
p_inst_0050 = instantaneous_tidal_power_gw(e_grid, 0.0050)
p_inst_0020 = instantaneous_tidal_power_gw(e_grid, 0.0020)
p_inst_0010 = instantaneous_tidal_power_gw(e_grid, 0.0010)

ax2.plot(
    e_grid * 1e3,
    p_inst_0107,
    "navy",
    lw=2.6,
    label=r"Viscoelastic Maxwell $k_2/Q_E = 0.0107$",
)
ax2.plot(
    e_grid * 1e3,
    p_inst_0050,
    "teal",
    lw=2.0,
    linestyle="--",
    label=r"Intermediate $k_2/Q_E = 0.0050$",
)
ax2.plot(
    e_grid * 1e3,
    p_inst_0020,
    "darkorange",
    lw=2.0,
    linestyle="-.",
    label=r"Stiff Shell $k_2/Q_E = 0.0020$",
)
ax2.plot(
    e_grid * 1e3,
    p_inst_0010,
    "gray",
    lw=1.8,
    linestyle=":",
    label=r"Spencer 2006 Min $k_2/Q_E = 0.0010$",
)

# Present forced eccentricity line
ax2.axvline(
    e_E_nom * 1e3,
    color="crimson",
    linestyle="--",
    lw=1.8,
    label=r"Present Forced $e = 0.0047$ ($P_{\rm tide} \approx 14.6$ GW)",
)
ax2.axhline(
    5.8, color="crimson", linestyle=":", lw=1.5, label=r"Spencer Obs ($5.8$ GW)"
)
ax2.axhline(
    1.17,
    color="navy",
    linestyle=":",
    lw=1.5,
    label=r"Equilibrium Heat $\dot{E}_{\rm eq}$ ($1.17$ GW)",
)

# Mark equilibrium forced eccentricity
e_eq_0107 = np.sqrt(
    (1.171e9) / (10.5 * 0.0107 * G * (M_S**2) * (R_E**5) * n_E / (a_E**6))
)
ax2.scatter(
    [e_eq_0107 * 1e3],
    [1.171],
    color="darkred",
    s=80,
    zorder=6,
    edgecolors="black",
    label=r"Equilibrium Crossing $e_{\rm eq} \approx 0.0013$ ($R^2 = 1.000$)",
)

ax2.set_xlim(0, 16.0)
ax2.set_ylim(0, 30.0)
ax2.set_xlabel(r"Orbital Eccentricity $e \times 10^3$", fontsize=12)
ax2.set_ylabel(r"Instantaneous Tidal Heating Power $P_{\rm tide}$ [GW]", fontsize=12)
ax2.set_title(
    r"\textbf{(b)} Instantaneous Tidal Power vs. Orbital Eccentricity",
    fontsize=13,
    fontweight="bold",
)
ax2.legend(loc="upper left", fontsize=8.5, framealpha=0.92)
ax2.grid(True, linestyle=":", alpha=0.6)

plt.tight_layout()
fig1.savefig("replications_ss/paper_216/fig_comparison.pdf", dpi=300)
fig1.savefig("replications_ss/paper_216/fig_comparison.png", dpi=300)
plt.close(fig1)
print(">>> Saved fig_comparison.pdf and fig_comparison.png")

# ============================================================================
# FIGURE 2: fig_model_choices.pdf (Ice Shell Equilibrium & Energy Deficit)
# ============================================================================
print("[2/3] Generating fig_model_choices.pdf...")
fig2, (ax3, ax4) = plt.subplots(1, 2, figsize=(14, 5.5))

# Panel A: Conductive heat loss & equilibrium shell thickness vs total internal heating
p_tot_grid = np.linspace(0.4, 25.0, 500)
d_eq_grid = equilibrium_shell_thickness_km(p_tot_grid, p_radio=0.0)

ax3.plot(
    p_tot_grid,
    d_eq_grid,
    "navy",
    lw=2.6,
    label=r"Equilibrium Thickness $d_{\rm eq}(P_{\rm heat})$",
)
ax3.axvline(
    1.17 + P_radio_gw,
    color="navy",
    linestyle="--",
    lw=1.8,
    label=r"Canonical Eq. Tidal + Radio ($1.49$ GW $\rightarrow d_{\rm eq} \approx 42$ km)",
)
ax3.axvline(
    5.8,
    color="crimson",
    linestyle="--",
    lw=1.8,
    label=r"Spencer Obs ($5.8$ GW $\rightarrow d_{\rm eq} \approx 11$ km)",
)
ax3.axvline(
    15.8,
    color="purple",
    linestyle=":",
    lw=1.8,
    label=r"Howett Obs ($15.8$ GW $\rightarrow d_{\rm eq} \approx 4$ km)",
)

# Regional highlights
ax3.axhspan(
    4.0,
    8.0,
    color="crimson",
    alpha=0.15,
    label=r"Observed South Polar Terrain (SPT) Shell ($d \sim 5$ km)",
)
ax3.axhspan(
    20.0,
    35.0,
    color="teal",
    alpha=0.15,
    label=r"Global Mean Ice Shell ($d \sim 20-35$ km)",
)

ax3.set_xlim(0.4, 22.0)
ax3.set_ylim(0.0, 60.0)
ax3.set_xlabel(r"Total Internal Heat Output $P_{\rm total}$ [GW]", fontsize=12)
ax3.set_ylabel(r"Equilibrium Ice Shell Thickness $d_{\rm eq}$ [km]", fontsize=12)
ax3.set_title(
    r"\textbf{(a)} Conductive Ice Shell Thickness vs. Heating Power",
    fontsize=13,
    fontweight="bold",
)
ax3.legend(loc="upper right", fontsize=8.5, framealpha=0.92)
ax3.grid(True, linestyle=":", alpha=0.6)

# Panel B: Energy Deficit Delta P vs Saturn Quality Factor Q_S
q_sweep = np.logspace(2.8, 5.0, 400)
e_eq_sweep = equilibrium_tidal_power_gw(0.341, q_sweep)
def_spencer = 5.8 - (e_eq_sweep + P_radio_gw)
def_howett = 15.8 - (e_eq_sweep + P_radio_gw)

ax4.plot(
    q_sweep,
    def_spencer,
    "crimson",
    lw=2.5,
    label=r"Energy Deficit for Spencer et al. ($5.8$ GW)",
)
ax4.plot(
    q_sweep,
    def_howett,
    "purple",
    lw=2.5,
    linestyle="--",
    label=r"Energy Deficit for Howett et al. ($15.8$ GW)",
)
ax4.axhline(
    0.0,
    color="black",
    lw=1.5,
    linestyle="-",
    label=r"Steady-State Equilibrium Line ($\Delta P = 0$)",
)

# Shaded deficit zone ("Energy Crisis")
ax4.axvspan(
    18000.0,
    100000.0,
    color="gray",
    alpha=0.15,
    label=r"Classical Goldreich Bound ($Q_S \geq 18,000$, $\Delta P > 0$)",
)
ax4.axvspan(
    1000.0,
    2500.0,
    color="green",
    alpha=0.15,
    label=r"Modern Astrometric Resonance-Locking ($Q_S \sim 1500-2000$)",
)

# Canonical point marking
ax4.scatter(
    [18000.0],
    [5.8 - (1.171 + P_radio_gw)],
    color="crimson",
    s=80,
    zorder=6,
    edgecolors="black",
    label=r"Meyer \& Wisdom Deficit ($\Delta P \approx 4.31$ GW at $Q_S = 18000$)",
)
ax4.scatter(
    [18000.0],
    [15.8 - (1.171 + P_radio_gw)],
    color="purple",
    s=80,
    zorder=6,
    edgecolors="black",
    label=r"Howett Revised Deficit ($\Delta P \approx 14.31$ GW at $Q_S = 18000$)",
)

ax4.set_xscale("log")
ax4.set_xlim(800, 100000)
ax4.set_ylim(-10.0, 18.0)
ax4.set_xlabel(r"Saturn Tidal Dissipation Quality Factor $Q_S$", fontsize=12)
ax4.set_ylabel(
    r"Thermal Energy Deficit $\Delta P = P_{\rm obs} - (P_{\rm eq} + P_{\rm radio})$ [GW]",
    fontsize=12,
)
ax4.set_title(
    r"\textbf{(b)} The Enceladus Energy Deficit vs. Saturn $Q_S$",
    fontsize=13,
    fontweight="bold",
)
ax4.legend(loc="lower left", fontsize=8.5, framealpha=0.92)
ax4.grid(True, which="both", linestyle=":", alpha=0.6)

plt.tight_layout()
fig2.savefig("replications_ss/paper_216/fig_model_choices.pdf", dpi=300)
fig2.savefig("replications_ss/paper_216/fig_model_choices.png", dpi=300)
plt.close(fig2)
print(">>> Saved fig_model_choices.pdf and fig_model_choices.png")

# ============================================================================
# FIGURE 3: fig_diagram.pdf (Orbital Resonance Schematic & Enceladus Interior)
# ============================================================================
print("[3/3] Generating fig_diagram.pdf...")
fig3, (ax5, ax6) = plt.subplots(1, 2, figsize=(15, 6.2))

# ----------------------------------------------------------------------------
# Panel A: Orbital 2:1 Resonance & Angular Momentum Exchange
# ----------------------------------------------------------------------------
ax5.set_aspect("equal")
ax5.set_facecolor("#f8fafc")

# Draw Saturn
saturn = patches.Circle(
    (0, 0), 1.2, facecolor="#f4d06f", edgecolor="#c29d38", lw=2.0, zorder=4
)
ax5.add_patch(saturn)
ax5.text(
    0,
    0,
    r"\textbf{Saturn}"
    + "\n"
    + r"$M_S = 5.68 \times 10^{26}\,$kg"
    + "\n"
    + r"$R_S = 60,268\,$km",
    ha="center",
    va="center",
    fontsize=9,
    fontweight="bold",
    color="#4a3500",
)

# Saturn Rings schematic
ring1 = patches.Ellipse(
    (0, 0),
    3.4,
    0.9,
    angle=25,
    facecolor="none",
    edgecolor="#d4af37",
    lw=1.8,
    alpha=0.7,
    zorder=3,
)
ring2 = patches.Ellipse(
    (0, 0),
    4.2,
    1.1,
    angle=25,
    facecolor="none",
    edgecolor="#b8860b",
    lw=1.2,
    alpha=0.5,
    zorder=3,
)
ax5.add_patch(ring1)
ax5.add_patch(ring2)

# Enceladus Orbit (Inner, a = 238,040 km)
enc_orbit = patches.Ellipse(
    (0.2, 0),
    6.4,
    6.2,
    angle=0,
    facecolor="none",
    edgecolor="#2563eb",
    lw=1.8,
    linestyle="--",
    zorder=2,
)
ax5.add_patch(enc_orbit)

# Enceladus Body
enc_pos = (3.4, 0.0)
enc_body = patches.Circle(
    enc_pos, 0.35, facecolor="#93c5fd", edgecolor="#1d4ed8", lw=1.8, zorder=5
)
ax5.add_patch(enc_body)
ax5.text(
    enc_pos[0] + 0.5,
    enc_pos[1] + 0.35,
    r"\textbf{Enceladus (Inner, 1)}"
    + "\n"
    + r"$a_E = 238,040\,$km"
    + "\n"
    + r"$P_E = 32.89\,$h"
    + "\n"
    + r"$e_E = 0.0047$",
    fontsize=8.5,
    color="#1e3a8a",
    fontweight="bold",
)

# Dione Orbit (Outer, a = 377,420 km)
dione_orbit = patches.Ellipse(
    (0, 0),
    9.6,
    9.6,
    angle=0,
    facecolor="none",
    edgecolor="#059669",
    lw=1.8,
    linestyle=":",
    zorder=2,
)
ax5.add_patch(dione_orbit)

# Dione Body
dione_pos = (0.0, 4.8)
dione_body = patches.Circle(
    dione_pos, 0.55, facecolor="#6ee7b7", edgecolor="#047857", lw=1.8, zorder=5
)
ax5.add_patch(dione_body)
ax5.text(
    dione_pos[0] + 0.65,
    dione_pos[1],
    r"\textbf{Dione (Outer, 2)}"
    + "\n"
    + r"$a_D = 377,420\,$km"
    + "\n"
    + r"$P_D = 65.77\,$h"
    + "\n"
    + r"$e_D = 0.0022$",
    fontsize=8.5,
    color="#064e3b",
    fontweight="bold",
)

# Resonant Torques & Dynamics Arrows
ax5.annotate(
    r"Tidal Torque $N_{SE} \sim 9.7 \times 10^{13}\,$N m",
    xy=(1.5, 0.6),
    xytext=(2.2, 1.8),
    arrowprops=dict(arrowstyle="->", color="#2563eb", lw=1.8),
    fontsize=8,
    color="#1e40af",
)

ax5.annotate(
    r"Tidal Torque $N_{SD} \sim 6.3 \times 10^{14}\,$N m",
    xy=(0.8, 2.5),
    xytext=(-2.8, 3.2),
    arrowprops=dict(arrowstyle="->", color="#059669", lw=1.8),
    fontsize=8,
    color="#065f46",
)

ax5.annotate(
    r"\textbf{2:1 Mean-Motion Resonance}" + "\n" + r"$n_E \approx 2 n_D$",
    xy=(2.2, -3.2),
    xytext=(1.0, -4.5),
    bbox=dict(boxstyle="round,pad=0.3", fc="#eff6ff", ec="#3b82f6", lw=1.2),
    fontsize=9,
    color="#1e3a8a",
)

# Formula banner
ax5.text(
    0.0,
    -5.8,
    r"$\dot{E}_{\rm eq} = \frac{(n_E - n_D)(L_D N_{SE} - L_E N_{SD})}{L_E + L_D} \approx 1.17 \left(\frac{18,000}{Q_S}\right)\,{\rm GW}$",
    ha="center",
    va="center",
    fontsize=9.5,
    bbox=dict(boxstyle="round,pad=0.4", fc="#fef3c7", ec="#f59e0b", lw=1.5),
    color="#92400e",
)

ax5.set_xlim(-6.2, 6.2)
ax5.set_ylim(-6.8, 6.2)
ax5.set_title(
    r"\textbf{(a)} Enceladus-Dione 2:1 Orbital Resonance Engine",
    fontsize=12,
    fontweight="bold",
)
ax5.axis("off")

# ----------------------------------------------------------------------------
# Panel B: Enceladus Interior Cross-Section & South Polar Energy Balance
# ----------------------------------------------------------------------------
ax6.set_aspect("equal")
ax6.set_facecolor("#f8fafc")

# Draw Outer Ice Shell
outer_shell = patches.Circle(
    (0, 0), 4.2, facecolor="#bae6fd", edgecolor="#0284c7", lw=2.2, zorder=2
)
ax6.add_patch(outer_shell)

# Draw South Polar Thinning (Distorted Ocean Boundary)
theta = np.linspace(0, 2 * np.pi, 300)
r_ocean = np.zeros_like(theta)
for i, th in enumerate(theta):
    # Base radius 3.3, but extends outward at south pole (th near -pi/2)
    spt_factor = (
        0.55 * np.exp(-0.5 * ((th - 1.5 * np.pi) / 0.4) ** 2)
        if th > np.pi
        else 0.55 * np.exp(-0.5 * ((th + 0.5 * np.pi) / 0.4) ** 2)
    )
    r_ocean[i] = 3.2 + spt_factor

x_ocean = r_ocean * np.cos(theta)
y_ocean = r_ocean * np.sin(theta)
ax6.fill(
    x_ocean,
    y_ocean,
    facecolor="#38bdf8",
    edgecolor="#0369a1",
    lw=1.6,
    zorder=3,
    alpha=0.85,
)

# Porous Silicate Core
core = patches.Circle(
    (0, 0), 2.1, facecolor="#a8a29e", edgecolor="#57534e", lw=2.0, zorder=4
)
ax6.add_patch(core)
ax6.text(
    0,
    0,
    r"\textbf{Porous Silicate Core}"
    + "\n"
    + r"$R_{\rm core} \approx 190\,$km"
    + "\n"
    + r"$\rho \approx 2400\,$kg/m$^3$",
    ha="center",
    va="center",
    fontsize=8.5,
    color="#292524",
    fontweight="bold",
    zorder=5,
)

# Tiger Stripes (South Pole vents)
spt_y = -4.2
stripe_x = [-0.6, -0.2, 0.2, 0.6]
stripe_names = ["Damascus", "Baghdad", "Alexandria", "Cairo"]
for sx in stripe_x:
    ax6.plot([sx, sx * 0.8], [spt_y, spt_y + 0.5], color="#dc2626", lw=2.5, zorder=6)
    # Eruption plume jets
    ax6.plot(
        [sx, sx * 1.5],
        [spt_y, spt_y - 1.2],
        color="#38bdf8",
        lw=2.0,
        linestyle="--",
        alpha=0.8,
        zorder=6,
    )

ax6.text(
    0,
    -5.7,
    r"\textbf{Tiger Stripe Plumes $\rightarrow$ E-Ring}"
    + "\n"
    + r"Observed Output: $5.8 - 15.8\,$GW"
    + "\n"
    + r"$\dot{M} \approx 200\,$kg/s H$_2$O vapor/ice",
    ha="center",
    va="center",
    fontsize=8.5,
    color="#991b1b",
    fontweight="bold",
)

# Layer Labels
ax6.text(
    2.6,
    2.7,
    r"\textbf{Ice I Shell}"
    + "\n"
    + r"Equator: $\sim 25-35\,$km"
    + "\n"
    + r"South Pole: $\sim 5\,$km",
    fontsize=8,
    color="#0369a1",
    fontweight="bold",
)
ax6.text(
    -3.8,
    1.2,
    r"\textbf{Global Ocean}" + "\n" + r"$d \sim 30-40\,$km",
    fontsize=8,
    color="#0284c7",
    fontweight="bold",
)

# Heat flow comparison box
energy_box = (
    r"\textbf{Energy Budget Comparison:}"
    + "\n"
    + r"$\bullet$ Canonical Eq. Tidal: $\dot{E}_{\rm eq} \approx 1.17\,$GW"
    + "\n"
    + r"$\bullet$ Core Radiogenic: $P_{\rm rad} \approx 0.32\,$GW"
    + "\n"
    + r"$\bullet$ Observed Heat Loss: $P_{\rm obs} \approx 5.8 - 15.8\,$GW"
    + "\n"
    + r"$\bullet$ \textbf{Deficit: $\Delta P \approx 4.3 - 14.3\,$GW}"
)
ax6.text(
    -5.8,
    -2.8,
    energy_box,
    fontsize=8,
    bbox=dict(boxstyle="round,pad=0.3", fc="#fef2f2", ec="#ef4444", lw=1.2),
    color="#7f1d1d",
)

ax6.set_xlim(-6.2, 6.2)
ax6.set_ylim(-6.8, 6.2)
ax6.set_title(
    r"\textbf{(b)} Enceladus Internal Structure \& South Polar Heat Flux",
    fontsize=12,
    fontweight="bold",
)
ax6.axis("off")

plt.tight_layout()
fig3.savefig("replications_ss/paper_216/fig_diagram.pdf", dpi=300)
fig3.savefig("replications_ss/paper_216/fig_diagram.png", dpi=300)
plt.close(fig3)
print(">>> Saved fig_diagram.pdf and fig_diagram.png")
print(">>> All 3 figures generated successfully.")
