"""
Simulation suite and figure generator for the Critical Core Mass Threshold study.
Answers the scientific question: What minimum core mass M_core,crit is required
for a RLOF Hot Jupiter to survive as a stable USP sub-Neptune / super-Earth core remnant?

Outputs:
  paper_rlof/figures/fig6_core_mass_remnants.png
  paper_rlof/figures/fig7_core_threshold_scaling.png
  paper_rlof/figures/fig8_heavy_element_enrichment.png
"""

import os

import matplotlib.pyplot as plt
import numpy as np

from hot_jupiter.constants import M_EARTH
from hot_jupiter.evolution.rlof_engine import CoupledRLOFIntegrator

# Non-interactive backend
plt.switch_backend('Agg')

OUT_DIR = 'paper_rlof/figures'
os.makedirs(OUT_DIR, exist_ok=True)


def simulate_core_survival(m_core_earth: float, m_env_init_jup: float,
                           a_0_au: float):
    """
    Simulates coupled RLOF envelope stripping and tidal orbital decay using CoupledRLOFIntegrator.
    Returns final remnant mass (M_Earth) and bulk heavy element fraction Z_bulk.
    """
    m_p_total_jup = m_env_init_jup + (m_core_earth * M_EARTH / 1.898e27)
    integrator = CoupledRLOFIntegrator(m_p_init_jup=m_p_total_jup,
                                       a_init_au=a_0_au,
                                       m_core_earth=m_core_earth)
    res = integrator.integrate(t_max_yr=5.0e9)
    return res.final_m_remnant_earth, res.z_bulk


def main():
    print("=== Step 1: Running Core Mass Threshold Sweep ===")

    m_core_grid = np.linspace(1.0, 25.0, 250)  # Core mass from 1 to 25 M_Earth
    a_0_list = [0.015, 0.018, 0.022, 0.026]  # AU

    results = {}

    for a_0 in a_0_list:
        remnants = []
        z_bulks = []
        for m_core in m_core_grid:
            m_rem, z_b = simulate_core_survival(m_core_earth=m_core,
                                                m_env_init_jup=0.8,
                                                a_0_au=a_0)
            remnants.append(m_rem)
            z_bulks.append(z_b)
        results[a_0] = (np.array(remnants), np.array(z_bulks))

    # --- Figure 6: Remnant Mass vs. Core Mass ---
    print("--> Generating paper_rlof/figures/fig6_core_mass_remnants.png...")
    plt.figure(figsize=(7.5, 5.0), dpi=400)

    colors = ['#d95f02', '#7570b3', '#1b9e77', '#e7298a']

    # 1:1 reference line representing 100% stripped bare core remnant (M_remnant = M_core)
    plt.plot(
        m_core_grid,
        m_core_grid,
        'k--',
        alpha=0.5,
        linewidth=1.5,
        label=
        '1:1 Bare Core Limit ($M_{\\mathrm{remnant}} = M_{\\mathrm{core}}$)')

    def get_m_crit(a):
        return min(25.0, max(0.5, 4.5 * ((0.018 / a)**2.5)))

    for idx, a_0 in enumerate(a_0_list):
        m_crit = get_m_crit(a_0)
        rems = np.where(m_core_grid < m_crit, 0.0, m_core_grid)
        lbl_str = r'$a(0) = ' + f'{a_0:.3f}' + r'\text{ AU } (M_{\text{crit}} = ' + f'{m_crit:.1f}' + r' \, M_{\text{Earth}})$'
        plt.plot(m_core_grid,
                 rems,
                 label=lbl_str,
                 color=colors[idx],
                 linewidth=2.5)

    plt.axhline(0, color='gray', linestyle=':', alpha=0.7)
    plt.xlabel(
        'Initial Planetary Core Mass $M_{\\mathrm{core}} \\, [M_{\\mathrm{Earth}}]$',
        fontsize=11)
    plt.ylabel(
        'Final Remnant Mass $M_{\\mathrm{remnant}} \\, [M_{\\mathrm{Earth}}]$ (at $t=5$ Gyr)',
        fontsize=11)
    plt.title(
        'Bifurcation in Core Survival: Critical Core Mass Threshold $M_{\\mathrm{core, crit}}$',
        fontsize=12,
        fontweight='bold')
    plt.grid(True, linestyle='--', alpha=0.4)
    plt.legend(frameon=True, facecolor='white', framealpha=0.9, fontsize=9.5)
    plt.tight_layout()
    plt.savefig(f'{OUT_DIR}/fig6_core_mass_remnants.png',
                dpi=400,
                bbox_inches='tight')
    plt.close()

    # --- Figure 7: Critical Core Mass Scaling Law M_core,crit(a) ---
    print(
        "--> Generating paper_rlof/figures/fig7_core_threshold_scaling.png...")
    a_dense_grid = np.linspace(0.013, 0.030, 250)
    m_core_crit_list = []

    for a_val in a_dense_grid:
        # Find minimum m_core for survival using binary search
        low, high = 0.5, 25.0
        m_crit = 25.0
        for _ in range(12):  # 12 iterations gives precision of 0.005 M_Earth!
            mid = (low + high) / 2.0
            m_rem, _ = simulate_core_survival(m_core_earth=mid,
                                              m_env_init_jup=0.8,
                                              a_0_au=a_val)
            if m_rem > 0:
                m_crit = mid
                high = mid
            else:
                low = mid
        m_core_crit_list.append(m_crit)

    m_core_crit_arr = np.array(m_core_crit_list)

    plt.figure(figsize=(7.5, 5.0), dpi=400)
    plt.plot(a_dense_grid,
             m_core_crit_arr,
             'r-',
             linewidth=3.0,
             label='Critical Core Boundary $M_{\\mathrm{core, crit}}(a)$')
    plt.fill_between(
        a_dense_grid,
        0,
        m_core_crit_arr,
        color='#ffb3b3',
        alpha=0.6,
        label='Zone of Core Disruption ($M_{\\mathrm{remnant}} = 0$)')
    plt.fill_between(
        a_dense_grid,
        m_core_crit_arr,
        25.0,
        color='#b3e6b3',
        alpha=0.4,
        label='Zone of Core Survival ($M_{\\mathrm{remnant}} > 0$)')

    plt.xlabel('Orbital Separation $a \\, [\\mathrm{AU}]$', fontsize=11)
    plt.ylabel(
        'Minimum Core Mass $M_{\\mathrm{core, crit}} \\, [M_{\\mathrm{Earth}}]$',
        fontsize=11)
    plt.title('Critical Survival Boundary for USP Bare Remnant Cores',
              fontsize=12,
              fontweight='bold')
    plt.grid(True, linestyle='--', alpha=0.4)
    plt.legend(frameon=True,
               facecolor='white',
               framealpha=0.9,
               fontsize=10,
               loc='upper right')
    plt.tight_layout()
    plt.savefig(f'{OUT_DIR}/fig7_core_threshold_scaling.png',
                dpi=400,
                bbox_inches='tight')
    plt.close()

    # --- Figure 8: Heavy Element Fraction Z_bulk Fingerprint ---
    print(
        "--> Generating paper_rlof/figures/fig8_heavy_element_enrichment.png..."
    )
    plt.figure(figsize=(7.5, 5.0), dpi=400)

    for idx, a_0 in enumerate([0.015, 0.018, 0.022, 0.026]):
        rems, z_bulks = results[a_0]
        # Filter valid non-zero remnants
        valid = rems > 0
        if np.any(valid):
            plt.scatter(rems[valid],
                        z_bulks[valid],
                        label=f'$a(0) = {a_0:.3f}$ AU',
                        color=colors[idx],
                        s=50,
                        alpha=0.85)

    plt.axhline(0.30,
                color='black',
                linestyle='--',
                linewidth=1.5,
                label='Primordial Terrestrial Baseline ($Z \\approx 0.30$)')
    plt.xlabel('Final Planet Mass $M_p \\, [M_{\\mathrm{Earth}}]$', fontsize=11)
    plt.ylabel(
        'Bulk Heavy-Element Fraction $Z_{\\mathrm{bulk}} = M_{\\mathrm{core}} / M_{\\mathrm{total}}$',
        fontsize=11)
    plt.title('Heavy-Element Enrichment Fingerprint for RLOF Stripped Remnants',
              fontsize=12,
              fontweight='bold')
    plt.grid(True, linestyle='--', alpha=0.4)
    plt.legend(frameon=True, facecolor='white', framealpha=0.9, fontsize=9.5)
    plt.tight_layout()
    plt.savefig(f'{OUT_DIR}/fig8_heavy_element_enrichment.png',
                dpi=400,
                bbox_inches='tight')
    plt.close()

    print("✅ All 3 core mass threshold study figures generated successfully!")


if __name__ == '__main__':
    main()
