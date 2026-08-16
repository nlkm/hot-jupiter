"""
Incremental Population Synthesis Simulator evaluating step-by-step physical inflation mechanisms.
"""

from dataclasses import dataclass

import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import ks_2samp

from hot_jupiter.atmosphere import GuillotAtmosphere
from hot_jupiter.constants import BAR, GYR, L_SUN, M_EARTH, R_JUP, YEAR
from hot_jupiter.eos import BaseEOS, TabularEOS
from hot_jupiter.evolution import ThermalEvolutionIntegrator
from hot_jupiter.heating import (
    BaseHeatingSource,
    OhmicDissipationHeating,
    TidalEccentricityHeating,
    ZeroHeating,
)
from hot_jupiter.population.catalog import (
    ExoplanetSystem,
    get_curated_hot_jupiter_catalog,
)
from hot_jupiter.population.core_scaling import estimate_heavy_element_mass
from hot_jupiter.population.selection_effects import transit_selection_weight
from hot_jupiter.structure import InteriorSolver


class CompositeHeating(BaseHeatingSource):
    """Combines multiple interior heating sources (e.g. Tidal + Ohmic)."""

    def __init__(self, sources: list[BaseHeatingSource]):
        self.sources = sources

    def evaluate_power(
        self,
        t: float,
        R_p: float,
        M_p: float,
        S_env: float,
        orbit_params: dict | None = None,
    ) -> float:
        total_p = 0.0
        for src in self.sources:
            total_p += src.evaluate_power(t, R_p, M_p, S_env, orbit_params)
        return total_p


def _process_single_system(args: tuple[ExoplanetSystem, float]) -> tuple:
    sys, k2_over_Q = args
    eos = TabularEOS.create_synthetic_grid()
    solver = InteriorSolver(envelope_eos=eos)
    atmosphere = GuillotAtmosphere(envelope_eos=eos)

    name = sys.name
    r_obs = sys.R_p_obs / R_JUP

    # System parameters
    M_c_fixed = 10.0 * M_EARTH
    M_c_metallicity = estimate_heavy_element_mass(M_p=sys.M_p, fe_h=sys.fe_h)

    L_star = L_SUN * ((sys.M_star / 1.988e30)**3.5)
    F_inc = L_star / (4.0 * np.pi * (sys.a**2))

    orbit_params = {
        "a": sys.a,
        "eccentricity": max(0.01, sys.eccentricity),
        "M_star": sys.M_star,
        "F_inc": F_inc,
        "A_b": 0.1,
    }

    S_init = eos.specific_entropy(1.0 * BAR, 600.0)
    t_target = max(0.1, sys.age_gyr) * GYR

    # --- Stage 0: Non-irradiated Cooling ---
    int_0 = ThermalEvolutionIntegrator(solver, atmosphere, ZeroHeating())
    r0 = int_0.evolve(sys.M_p,
                      M_c_fixed,
                      S_init, (1e6 * YEAR, t_target),
                      F_inc=0.0,
                      num_eval=2,
                      method="RK23")

    # --- Stage 1: Stellar Irradiation ---
    r1 = int_0.evolve(sys.M_p,
                      M_c_fixed,
                      S_init, (1e6 * YEAR, t_target),
                      F_inc=F_inc,
                      num_eval=2,
                      method="RK23")

    # --- Stage 2: Irradiation + Metallicity Core Mass ---
    r2 = int_0.evolve(sys.M_p,
                      M_c_metallicity,
                      S_init, (1e6 * YEAR, t_target),
                      F_inc=F_inc,
                      num_eval=2,
                      method="RK23")

    # --- Stage 3: Irradiation + Core Mass + Tidal Heating ---
    tidal_src = TidalEccentricityHeating(M_star=sys.M_star,
                                         a=sys.a,
                                         eccentricity=max(
                                             0.01, sys.eccentricity),
                                         k2_over_Q=k2_over_Q)
    int_3 = ThermalEvolutionIntegrator(solver, atmosphere, tidal_src)
    r3 = int_3.evolve(sys.M_p,
                      M_c_metallicity,
                      S_init, (1e6 * YEAR, t_target),
                      F_inc=F_inc,
                      orbit_params=orbit_params,
                      num_eval=2,
                      method="Radau")

    # --- Stage 4: Irradiation + Core Mass + Tidal + Ohmic Dissipation ---
    ohmic_src = OhmicDissipationHeating(epsilon_max=0.025)
    comp_src = CompositeHeating([tidal_src, ohmic_src])
    int_4 = ThermalEvolutionIntegrator(solver, atmosphere, comp_src)
    r4 = int_4.evolve(sys.M_p,
                      M_c_metallicity,
                      S_init, (1e6 * YEAR, t_target),
                      F_inc=F_inc,
                      orbit_params=orbit_params,
                      num_eval=2,
                      method="Radau")

    # Selection weight
    w = transit_selection_weight(sys.R_p_obs, sys.R_star, sys.a, sys.P_orb_days,
                                 sys.eccentricity)

    return (name, r_obs, w, r0.R_p_jup[-1], r1.R_p_jup[-1], r2.R_p_jup[-1],
            r3.R_p_jup[-1], r4.R_p_jup[-1])


@dataclass
class IncrementalModelStats:
    """Statistics for a single model stage in the population study."""
    name: str
    radii_jup: np.ndarray  # Radii array [R_Jup]
    mean_R: float  # Mean radius [R_Jup]
    std_R: float  # Standard deviation [R_Jup]
    ks_stat: float  # KS test statistic D
    p_value: float  # KS test p-value


@dataclass
class IncrementalPopulationResult:
    """Container for multi-stage population simulation result."""
    catalog_names: list[str]
    R_obs_jup: np.ndarray
    selection_weights: np.ndarray
    stage_results: dict[str, IncrementalModelStats]


class PopulationSimulator:
    """
    Incremental Population Synthesis Simulator.
    Evaluates Models 0 to 5 to measure the relative impact of each physical mechanism.
    """

    def __init__(
        self,
        catalog: list[ExoplanetSystem] | None = None,
        envelope_eos: BaseEOS | None = None,
        k2_over_Q: float = 2.0e-5,
    ):
        self.catalog = catalog if catalog is not None else get_curated_hot_jupiter_catalog(
        )
        self.envelope_eos = envelope_eos if envelope_eos is not None else TabularEOS.create_synthetic_grid(
        )
        self.k2_over_Q = k2_over_Q
        self.solver = InteriorSolver(envelope_eos=self.envelope_eos)
        self.atmosphere = GuillotAtmosphere(envelope_eos=self.envelope_eos)

    def run_incremental_simulation(self) -> IncrementalPopulationResult:
        """
        Run forward evolution across all 6 incremental model stages:
        - Stage 0: Non-irradiated cooling baseline (F_inc = 0, M_c = 10 M_Earth)
        - Stage 1: Stellar Irradiation (F_inc, M_c = 10 M_Earth)
        - Stage 2: Irradiation + Metallicity-dependent Core Mass (M_c([Fe/H]))
        - Stage 3: Irradiation + Core Mass + Tidal Heating (P_tidal)
        - Stage 4: Irradiation + Core Mass + Tidal + Ohmic Dissipation (P_tidal + P_ohmic)
        - Stage 5: Full Model (Stage 4) + Transit Selection Probability Weighting
        """
        import os
        from concurrent.futures import ProcessPoolExecutor

        names = []
        R_obs_list = []
        weights_list = []

        radii_stage0 = []
        radii_stage1 = []
        radii_stage2 = []
        radii_stage3 = []
        radii_stage4 = []

        tasks = [(sys, self.k2_over_Q) for sys in self.catalog]
        max_workers = min(os.cpu_count() or 4, 8)

        if len(tasks) <= 2:
            results = [_process_single_system(t) for t in tasks]
        else:
            with ProcessPoolExecutor(max_workers=max_workers) as executor:
                results = list(executor.map(_process_single_system, tasks))

        for res in results:
            name, r_obs, w, r0_val, r1_val, r2_val, r3_val, r4_val = res
            names.append(name)
            R_obs_list.append(r_obs)
            weights_list.append(w)
            radii_stage0.append(r0_val)
            radii_stage1.append(r1_val)
            radii_stage2.append(r2_val)
            radii_stage3.append(r3_val)
            radii_stage4.append(r4_val)

        R_obs_arr = np.array(R_obs_list)
        weights_arr = np.array(weights_list)

        stages_dict = {}
        stage_data = [
            ("Stage 0: Non-irradiated Base", np.array(radii_stage0)),
            ("Stage 1: Stellar Irradiation", np.array(radii_stage1)),
            ("Stage 2: Irradiation + Core([Fe/H])", np.array(radii_stage2)),
            ("Stage 3: Irradiation + Core + Tidal", np.array(radii_stage3)),
            ("Stage 4: Full Physical (Tidal+Ohmic)", np.array(radii_stage4)),
            ("Stage 5: Full Model + Transit Selection", np.array(radii_stage4)),
        ]

        for name, r_arr in stage_data:
            ks, pval = ks_2samp(R_obs_arr, r_arr)
            stats = IncrementalModelStats(
                name=name,
                radii_jup=r_arr,
                mean_R=float(np.mean(r_arr)),
                std_R=float(np.std(r_arr)),
                ks_stat=float(ks),
                p_value=float(pval),
            )
            stages_dict[name] = stats

        return IncrementalPopulationResult(
            catalog_names=names,
            R_obs_jup=R_obs_arr,
            selection_weights=weights_arr,
            stage_results=stages_dict,
        )

    def plot_incremental_stages(
        self,
        result: IncrementalPopulationResult,
        savepath: str | None = None,
    ) -> plt.Figure:
        """
        Plot multi-stage cumulative probability distributions showing incremental progress.
        """
        fig, axes = plt.subplots(1, 2, figsize=(13, 5))
        x_grid = np.linspace(0.8, 2.2, 200)

        # Left panel: Histograms of Baseline vs Full Model vs Observed
        ax1 = axes[0]
        bins = np.linspace(0.8, 2.2, 14)
        ax1.hist(result.R_obs_jup,
                 bins=bins,
                 alpha=0.4,
                 color="black",
                 density=True,
                 label="Observed Catalog")

        s0 = result.stage_results["Stage 0: Non-irradiated Base"]
        s1 = result.stage_results["Stage 1: Stellar Irradiation"]
        s4 = result.stage_results["Stage 4: Full Physical (Tidal+Ohmic)"]

        ax1.hist(s0.radii_jup,
                 bins=bins,
                 histtype="step",
                 lw=2,
                 color="gray",
                 linestyle=":",
                 density=True,
                 label="Stage 0 (Non-irradiated)")
        ax1.hist(s1.radii_jup,
                 bins=bins,
                 histtype="step",
                 lw=2,
                 color="blue",
                 linestyle="--",
                 density=True,
                 label="Stage 1 (Irradiation)")
        ax1.hist(s4.radii_jup,
                 bins=bins,
                 histtype="step",
                 lw=2.5,
                 color="red",
                 density=True,
                 label="Stage 4 (Tidal+Ohmic)")

        ax1.set_xlabel(r"Planet Radius $R_p$ [$R_{\mathrm{Jup}}$]")
        ax1.set_ylabel("Probability Density")
        ax1.set_title("Incremental Model Radii Histograms")
        ax1.legend(loc="best")
        ax1.grid(True, alpha=0.3)

        # Right panel: CDF comparison across all stages
        ax2 = axes[1]
        cdf_obs = np.array([np.mean(result.R_obs_jup <= x) for x in x_grid])
        ax2.plot(x_grid, cdf_obs, "k-", lw=3.0, label="Observed Catalog")

        colors = ["gray", "blue", "cyan", "orange", "red"]
        stage_keys = [
            "Stage 0: Non-irradiated Base",
            "Stage 1: Stellar Irradiation",
            "Stage 2: Irradiation + Core([Fe/H])",
            "Stage 3: Irradiation + Core + Tidal",
            "Stage 4: Full Physical (Tidal+Ohmic)",
        ]

        for idx, key in enumerate(stage_keys):
            st = result.stage_results[key]
            cdf_st = np.array([np.mean(st.radii_jup <= x) for x in x_grid])
            ax2.plot(x_grid,
                     cdf_st,
                     label=f"{st.name} (p={st.p_value:.3f})",
                     color=colors[idx],
                     lw=1.8)

        ax2.set_xlabel(r"Planet Radius $R_p$ [$R_{\mathrm{Jup}}$]")
        ax2.set_ylabel("Cumulative Probability")
        ax2.set_title("Incremental Model CDF & KS Test Progression")
        ax2.legend(loc="best", fontsize=8)
        ax2.grid(True, alpha=0.3)

        plt.tight_layout()
        if savepath:
            fig.savefig(savepath, bbox_inches="tight")
            if savepath.endswith(".pdf"):
                fig.savefig(savepath.replace(".pdf", ".png"),
                            dpi=300,
                            bbox_inches="tight")

        return fig
