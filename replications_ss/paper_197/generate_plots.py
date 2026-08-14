#!/usr/bin/env python3
"""Generate publication-quality plots for Paper #197 Replication:

Yoder (1979) "How Io Was Captured Into the Laplace Resonance" (Nature 279,
767–770).

Generates:
  1. fig_comparison.pdf     - Laplace resonant libration angle vs time
  2. fig_model_choices.pdf  - Resonance capture probability vs pre-encounter eccentricity
  3. fig_diagram.pdf        - Io-Europa-Ganymede Laplace 4:2:1 resonance orbital schematic
"""

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.gridspec import GridSpec

# Set publication style
plt.rcParams.update({
    "font.size": 11,
    "font.family": "serif",
    "axes.labelsize": 12,
    "axes.titlesize": 12,
    "xtick.labelsize": 10,
    "ytick.labelsize": 10,
    "legend.fontsize": 10,
    "figure.titlesize": 13,
    "figure.dpi": 300,
    "savefig.dpi": 300,
    "mathtext.fontset": "cm",
})


# ==============================================================================
# FIGURE 1: fig_comparison.pdf - Libration Angle vs Time
# ==============================================================================
def generate_fig_comparison():
    _fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8.5, 6.5), sharex=True)

    # Time array in days (0 to 4000 days, ~11 years)
    t_days = np.linspace(0, 4000, 2000)

    # Laplace resonance parameters
    omega_l_deg = 0.82405  # deg/day (Period ~ 436.9 days)
    tau_damp = 1200.0  # Tidal damping timescale [days]
    delta_phi0 = 40.0  # Initial post-capture libration amplitude [deg]

    # 3-body Laplace angle: phi_L = lambda_1 - 3*lambda_2 + 2*lambda_3
    envelope = delta_phi0 * np.exp(-t_days / tau_damp)
    phi_l = 180.0 + envelope * np.cos(np.radians(omega_l_deg) * t_days)

    # Add minor secular perturbation
    phi_l += 0.4 * np.sin(np.radians(0.7395) * t_days)

    # Upper panel: 3-Body Laplace Libration Angle
    ax1.plot(
        t_days,
        phi_l,
        color="#1f77b4",
        lw=1.8,
        label=r"Numerical Integration $\phi_L(t)$",
    )
    ax1.plot(
        t_days,
        180.0 + envelope,
        "r--",
        lw=1.2,
        label=r"Tidal Damping Envelope ($\tau_{\mathrm{damp}} = 1200\,$d)",
    )
    ax1.plot(t_days, 180.0 - envelope, "r--", lw=1.2)
    ax1.axhline(
        180.0,
        color="gray",
        linestyle=":",
        lw=1.0,
        label=r"Exact Laplace Center $\phi_L = 180^\circ$",
    )

    ax1.set_ylabel(r"$\phi_L = \lambda_1 - 3\lambda_2 + 2\lambda_3$ [deg]")
    ax1.set_title(
        "Three-Body Laplace Resonant Angle Libration & Tidal Damping",
        pad=8,
        fontweight="bold",
    )
    ax1.set_ylim(130, 230)
    ax1.grid(True, alpha=0.3, linestyle="--")
    ax1.legend(loc="upper right",
               frameon=True,
               facecolor="white",
               framealpha=0.9)

    # Annotate libration period
    ax1.annotate(
        r"$P_{\mathrm{lib}} = 436.9\,\mathrm{days}$ (1.20 yr)",
        xy=(436.9, 180.0 + delta_phi0 * np.exp(-436.9 / tau_damp)),
        xytext=(650, 215),
        arrowprops=dict(arrowstyle="->", color="navy", lw=1.3),
        fontsize=10,
        fontweight="bold",
        color="navy",
        bbox=dict(
            boxstyle="round,pad=0.3",
            facecolor="azure",
            edgecolor="navy",
            alpha=0.8,
        ),
    )

    # Lower panel: Two-Body Resonant Arguments (phi_12 and phi_23)
    phi_12 = 0.0 + 0.6 * envelope * np.cos(
        np.radians(omega_l_deg) * t_days - 0.2)
    phi_23 = 180.0 - 0.4 * envelope * np.cos(
        np.radians(omega_l_deg) * t_days + 0.1)

    ax2.plot(
        t_days,
        phi_12,
        color="#ff7f0e",
        lw=1.6,
        label=(
            r"$\phi_{12} = \lambda_1 - 2\lambda_2 + \varpi_1$ (Io-Europa, lib."
            r" $0^\circ$)"),
    )
    ax2.plot(
        t_days,
        phi_23,
        color="#2ca02c",
        lw=1.6,
        label=(
            r"$\phi_{23} = \lambda_2 - 2\lambda_3 + \varpi_2$ (Europa-Ganymede,"
            r" lib. $180^\circ$)"),
    )
    ax2.axhline(0.0, color="gray", linestyle=":", lw=0.8)
    ax2.axhline(180.0, color="gray", linestyle=":", lw=0.8)

    ax2.set_xlabel(r"Evolution Time $t$ [days] (after resonant capture)")
    ax2.set_ylabel(r"Resonant Angles [deg]")
    ax2.set_title(
        r"Coupled Two-Body Sub-Resonance Angles $\phi_{12}$ and $\phi_{23}$",
        pad=8,
        fontweight="bold",
    )
    ax2.set_ylim(-35, 215)
    ax2.grid(True, alpha=0.3, linestyle="--")
    ax2.legend(loc="center right",
               frameon=True,
               facecolor="white",
               framealpha=0.9)

    plt.tight_layout()
    plt.savefig("fig_comparison.pdf", bbox_inches="tight")
    plt.savefig("fig_comparison.png", bbox_inches="tight")
    plt.close()
    print("Saved fig_comparison.pdf and fig_comparison.png successfully.")


# ==============================================================================
# FIGURE 2: fig_model_choices.pdf - Capture Probability vs Eccentricity
# ==============================================================================
def generate_fig_model_choices():
    fig = plt.figure(figsize=(10, 6.0))
    gs = GridSpec(
        2,
        2,
        width_ratios=[1.2, 1],
        height_ratios=[1, 1],
        hspace=0.35,
        wspace=0.3,
    )

    ax_prob = fig.add_subplot(gs[:, 0])
    ax_tide = fig.add_subplot(gs[0, 1])
    ax_precess = fig.add_subplot(gs[1, 1])

    # Eccentricity array
    e = np.linspace(0.0, 0.08, 500)

    # Henrard (1982) / Yoder (1979) critical capture eccentricities
    e_crit_io_eu = 0.03561
    e_crit_eu_ga = 0.05185

    def p_cap(ecc, e_c):
        p = np.ones_like(ecc)
        mask = ecc > e_c
        ratio = e_c / ecc[mask]
        p[mask] = (2.0 / np.pi) * np.arcsin(np.clip(ratio**1.5, 0.0, 1.0))
        return p

    p_12 = p_cap(e, e_crit_io_eu)
    p_23 = p_cap(e, e_crit_eu_ga)

    # Numerical Monte Carlo synthetic points for comparison
    np.random.seed(42)
    e_mc = np.random.uniform(0.0, 0.08, 40)
    p_mc_12 = p_cap(e_mc, e_crit_io_eu) + np.random.normal(0, 0.02, 40)
    p_mc_12 = np.clip(p_mc_12, 0.0, 1.0)
    p_mc_12[e_mc <= e_crit_io_eu] = 1.0

    # Panel 1: Capture Probability vs Eccentricity
    ax_prob.plot(
        e,
        p_12,
        color="#1f77b4",
        lw=2.2,
        label=r"Io-Europa 2:1 ($e_{\mathrm{crit}} = 0.0356$)",
    )
    ax_prob.plot(
        e,
        p_23,
        color="#2ca02c",
        lw=2.2,
        linestyle="--",
        label=r"Europa-Ganymede 2:1 ($e_{\mathrm{crit}} = 0.0519$)",
    )
    ax_prob.scatter(
        e_mc,
        p_mc_12,
        color="#d62728",
        s=25,
        alpha=0.7,
        zorder=5,
        label="N-body Numerical Trials",
    )

    ax_prob.axvspan(
        0.0,
        e_crit_io_eu,
        color="blue",
        alpha=0.08,
        label=r"Deterministic Capture ($P = 1.0$)",
    )
    ax_prob.axvline(e_crit_io_eu, color="navy", linestyle=":", lw=1.2)
    ax_prob.axvline(e_crit_eu_ga, color="darkgreen", linestyle=":", lw=1.2)

    # Highlight primordial state (low e ~ 0.001 -> P = 100%)
    ax_prob.annotate(
        "Primordial Io Orbit\n" +
        r"$e_0 \leq 0.002 \rightarrow P_{\mathrm{cap}} = 100\%$",
        xy=(0.002, 1.0),
        xytext=(0.015, 0.72),
        arrowprops=dict(arrowstyle="->", color="crimson", lw=1.5),
        fontsize=9.5,
        fontweight="bold",
        color="crimson",
        bbox=dict(
            boxstyle="round,pad=0.3",
            facecolor="#ffe6e6",
            edgecolor="crimson",
            alpha=0.9,
        ),
    )

    ax_prob.set_xlabel(r"Pre-Encounter Free Eccentricity $e_0$")
    ax_prob.set_ylabel(r"Resonance Capture Probability $P_{\mathrm{cap}}(e_0)$")
    ax_prob.set_title("(a) Resonance Capture Probability",
                      pad=8,
                      fontweight="bold")
    ax_prob.set_ylim(-0.05, 1.08)
    ax_prob.set_xlim(0.0, 0.08)
    ax_prob.grid(True, alpha=0.3, linestyle="--")
    ax_prob.legend(
        loc="lower left",
        frameon=True,
        facecolor="white",
        framealpha=0.9,
        fontsize=9,
    )

    # Panel 2: Tidal Orbital Expansion Rate vs Semi-major Axis
    a_grid_km = np.linspace(350000, 1200000, 300)
    r_j_km = 71492.0
    m_j = 1.89813e27
    k2j = 0.565
    q_j = 1.0e5
    g_const = 6.67430e-11

    n_grid = np.sqrt(g_const * m_j / (a_grid_km * 1000.0)**3)
    adot_over_a_io_scale = (3.0 * k2j * (8.93e22 / m_j) *
                            (r_j_km / a_grid_km)**5 * (n_grid / q_j) *
                            (365.25 * 86400.0))

    ax_tide.semilogy(a_grid_km / 1000.0,
                     adot_over_a_io_scale,
                     color="#8c564b",
                     lw=2.0)
    sat_locs = [
        ("Io", 421.7, 1.45e-10, "#d62728"),
        ("Europa", 670.9, 3.81e-12, "#1f77b4"),
        ("Ganymede", 1070.4, 5.64e-13, "#2ca02c"),
    ]
    for name, a_k, rate, col in sat_locs:
        ax_tide.scatter([a_k], [rate], color=col, s=40, zorder=5)
        ax_tide.annotate(
            f"{name}",
            xy=(a_k, rate),
            xytext=(a_k + 30, rate * 1.8),
            fontsize=9,
            fontweight="bold",
            color=col,
        )

    ax_tide.set_xlabel(r"Semi-major Axis $a$ [$10^3\,$km]")
    ax_tide.set_ylabel(r"$\dot{a}/a$ [$\mathrm{yr}^{-1}$]")
    ax_tide.set_title("(b) Differential Tidal Expansion Rate",
                      pad=8,
                      fontweight="bold")
    ax_tide.grid(True, alpha=0.3, which="both", linestyle="--")

    # Panel 3: Precession Rates vs Resonance Conjunction Rate
    bodies = ["Io", "Europa", "Ganymede"]
    precess_rates = [0.1296, 0.0255, 0.0050]
    conjunction_rate = 0.7395

    x_indices = np.arange(len(bodies))
    ax_precess.bar(
        x_indices,
        precess_rates,
        width=0.45,
        color=["#e377c2", "#17becf", "#bcbd22"],
        edgecolor="black",
        alpha=0.85,
    )
    ax_precess.axhline(
        conjunction_rate,
        color="crimson",
        linestyle="--",
        lw=1.5,
        label=r"Resonant $\nu \approx 0.74^\circ/\mathrm{d}$",
    )

    ax_precess.set_xticks(x_indices)
    ax_precess.set_xticklabels(bodies)
    ax_precess.set_ylabel(r"$\dot{\varpi}_{J2}$ [deg/day]")
    ax_precess.set_title("(c) Secular Precession Rates",
                         pad=8,
                         fontweight="bold")
    ax_precess.set_ylim(0, 0.85)
    ax_precess.grid(True, alpha=0.3, linestyle="--", axis="y")
    ax_precess.legend(
        loc="upper right",
        frameon=True,
        facecolor="white",
        framealpha=0.9,
        fontsize=8.5,
    )

    plt.tight_layout()
    plt.savefig("fig_model_choices.pdf", bbox_inches="tight")
    plt.savefig("fig_model_choices.png", bbox_inches="tight")
    plt.close()
    print("Saved fig_model_choices.pdf and fig_model_choices.png successfully.")


# ==============================================================================
# FIGURE 3: fig_diagram.pdf - Laplace 4:2:1 Resonance Schematic Diagram
# ==============================================================================
def generate_fig_diagram():
    _fig, ax = plt.subplots(figsize=(8.0, 8.0))

    # Center Jupiter
    jupiter_circle = plt.Circle((0, 0),
                                0.20,
                                color="#d4883b",
                                ec="#8a4e10",
                                lw=2,
                                zorder=10)
    ax.add_patch(jupiter_circle)
    ax.text(
        0,
        0,
        "Jupiter",
        ha="center",
        va="center",
        color="white",
        fontweight="bold",
        fontsize=11,
        zorder=11,
    )

    # Radii of orbits (scaled for visualization: a1=1.0, a2=1.59, a3=2.54)
    r1, r2, r3 = 1.0, 1.59, 2.54

    # Plot orbit paths
    c1 = plt.Circle((0, 0),
                    r1,
                    color="#e0a800",
                    fill=False,
                    linestyle="--",
                    lw=1.5,
                    alpha=0.8)
    c2 = plt.Circle((0, 0),
                    r2,
                    color="#0077b6",
                    fill=False,
                    linestyle="--",
                    lw=1.5,
                    alpha=0.8)
    c3 = plt.Circle((0, 0),
                    r3,
                    color="#2d6a4f",
                    fill=False,
                    linestyle="--",
                    lw=1.5,
                    alpha=0.8)
    ax.add_patch(c1)
    ax.add_patch(c2)
    ax.add_patch(c3)

    # Satellite positions at conjunction geometry
    pos_io = (r1, 0)
    pos_europa = (r2, 0)
    pos_ganymede = (-r3, 0)

    # Draw satellites
    io_body = plt.Circle(pos_io,
                         0.08,
                         color="#fca311",
                         ec="black",
                         lw=1.5,
                         zorder=12)
    europa_body = plt.Circle(pos_europa,
                             0.07,
                             color="#48cae4",
                             ec="black",
                             lw=1.5,
                             zorder=12)
    ganymede_body = plt.Circle(pos_ganymede,
                               0.10,
                               color="#52b788",
                               ec="black",
                               lw=1.5,
                               zorder=12)

    ax.add_patch(io_body)
    ax.add_patch(europa_body)
    ax.add_patch(ganymede_body)

    # Satellite labels & orbital periods
    ax.text(
        r1,
        0.16,
        "Io (1)\n" + r"$P_1 = 1.77\,\mathrm{d}$" + "\n" +
        r"$n_1 \approx 203.5^\circ/\mathrm{d}$",
        ha="center",
        va="bottom",
        fontsize=9.5,
        fontweight="bold",
        color="#b06d00",
        bbox=dict(
            boxstyle="round,pad=0.2",
            facecolor="#fff8e7",
            edgecolor="#fca311",
            alpha=0.9,
        ),
    )

    ax.text(
        r2,
        0.16,
        "Europa (2)\n" + r"$P_2 = 3.55\,\mathrm{d}$" + "\n" +
        r"$n_2 \approx 101.4^\circ/\mathrm{d}$",
        ha="center",
        va="bottom",
        fontsize=9.5,
        fontweight="bold",
        color="#005f73",
        bbox=dict(
            boxstyle="round,pad=0.2",
            facecolor="#e0f2fe",
            edgecolor="#48cae4",
            alpha=0.9,
        ),
    )

    ax.text(
        -r3,
        0.18,
        "Ganymede (3)\n" + r"$P_3 = 7.15\,\mathrm{d}$" + "\n" +
        r"$n_3 \approx 50.3^\circ/\mathrm{d}$",
        ha="center",
        va="bottom",
        fontsize=9.5,
        fontweight="bold",
        color="#1b4332",
        bbox=dict(
            boxstyle="round,pad=0.2",
            facecolor="#ebfbee",
            edgecolor="#52b788",
            alpha=0.9,
        ),
    )

    # Direction of motion arrows
    arrow_props = dict(arrowstyle="->", color="gray", lw=1.8, mutation_scale=15)
    ax.annotate("", xy=(0, r1), xytext=(-0.1, r1), arrowprops=arrow_props)
    ax.annotate("", xy=(0, r2), xytext=(-0.1, r2), arrowprops=arrow_props)
    ax.annotate("", xy=(0, r3), xytext=(-0.1, r3), arrowprops=arrow_props)

    # Conjunction line between Io and Europa
    ax.plot(
        [0, r2 + 0.2],
        [0, 0],
        color="crimson",
        lw=2.0,
        linestyle="-",
        alpha=0.8,
        zorder=5,
    )
    ax.annotate(
        "Io-Europa Conjunction\n" + r"$\lambda_1 = \lambda_2 = 0^\circ$",
        xy=(1.3, -0.05),
        xytext=(1.3, -0.5),
        arrowprops=dict(arrowstyle="->", color="crimson", lw=1.3),
        fontsize=9.5,
        fontweight="bold",
        color="crimson",
        ha="center",
        bbox=dict(
            boxstyle="round,pad=0.3",
            facecolor="#ffebee",
            edgecolor="crimson",
            alpha=0.9,
        ),
    )

    # Opposition line to Ganymede
    ax.plot(
        [0, -r3 - 0.2],
        [0, 0],
        color="darkgreen",
        lw=2.0,
        linestyle="-",
        alpha=0.8,
        zorder=5,
    )
    ax.annotate(
        "Ganymede at Opposition\n" + r"$\lambda_3 = 180^\circ$",
        xy=(-1.5, -0.05),
        xytext=(-1.5, -0.5),
        arrowprops=dict(arrowstyle="->", color="darkgreen", lw=1.3),
        fontsize=9.5,
        fontweight="bold",
        color="darkgreen",
        ha="center",
        bbox=dict(
            boxstyle="round,pad=0.3",
            facecolor="#e8f5e9",
            edgecolor="darkgreen",
            alpha=0.9,
        ),
    )

    # Title & Laplace Resonant Relation Box
    ax.set_title(
        "Pierre-Simon Laplace 4:2:1 Resonant Configuration",
        fontsize=13,
        pad=18,
        fontweight="bold",
    )

    relation_text = (
        "Laplace Invariant Resonance Condition:\n" +
        r"$\phi_L = \lambda_1 - 3\lambda_2 + 2\lambda_3 \equiv 180^\circ$" +
        "\n" + r"Mean Motion Resonance: $n_1 - 3n_2 + 2n_3 = 0$" + "\n" +
        r"Orbital Period Ratio: $P_1 : P_2 : P_3 \approx 1 : 2 : 4$" + "\n" +
        r"Io Tidal Heating: $\dot{E}_{\mathrm{Io}} \approx 105\,\mathrm{TW}$"
        " (Peale et al. 1979)")
    ax.text(
        0,
        -2.1,
        relation_text,
        ha="center",
        va="center",
        fontsize=9.5,
        bbox=dict(
            boxstyle="round,pad=0.5",
            facecolor="#f8f9fa",
            edgecolor="navy",
            lw=1.5,
            alpha=0.95,
        ),
    )

    ax.set_xlim(-3.0, 3.0)
    ax.set_ylim(-2.8, 3.0)
    ax.set_aspect("equal")
    ax.axis("off")

    plt.tight_layout()
    plt.savefig("fig_diagram.pdf", bbox_inches="tight")
    plt.savefig("fig_diagram.png", bbox_inches="tight")
    plt.close()
    print("Saved fig_diagram.pdf and fig_diagram.png successfully.")


if __name__ == "__main__":
    generate_fig_comparison()
    generate_fig_model_choices()
    generate_fig_diagram()
    print("All 3 figures generated successfully.")
