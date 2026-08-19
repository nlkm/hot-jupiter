"""
Publication plotting script for Frontier 4 Discovery:
Asymmetric Aerosol Rainout & Day-Night Chemical Quenching in Irradiated Gas Giants.
"""

import csv
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def main():
    out_dir = Path(__file__).parent.resolve()
    tp_file = out_dir / "terminator_profiles.csv"
    spec_file = out_dir / "jwst_transmission_spectrum.csv"

    if not tp_file.exists() or not spec_file.exists():
        print("Error: CSV files not found. Run simulation driver first.")
        return

    # Parse TP profiles
    p_bar, t_cond, t_day, t_eve, t_mor, t_nit, c_mor, c_eve, r_eff, tau_1 = [], [], [], [], [], [], [], [], [], []
    with open(tp_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            p_bar.append(float(row["pressure_bar"]))
            t_cond.append(float(row["t_cond_k"]))
            t_day.append(float(row["t_day_k"]))
            t_eve.append(float(row["t_evening_k"]))
            t_mor.append(float(row["t_morning_k"]))
            t_nit.append(float(row["t_night_k"]))
            c_mor.append(float(row["cloud_mor"]))
            c_eve.append(float(row["cloud_eve"]))
            r_eff.append(float(row["r_eff_mor_um"]))
            tau_1.append(float(row["tau_mor_1um"]))

    p_bar = np.array(p_bar)
    t_cond = np.array(t_cond)
    t_day = np.array(t_day)
    t_eve = np.array(t_eve)
    t_mor = np.array(t_mor)
    t_nit = np.array(t_nit)
    c_mor = np.array(c_mor)
    c_eve = np.array(c_eve)
    r_eff = np.array(r_eff)
    tau_1 = np.array(tau_1)

    # -------------------------------------------------------------------------
    # FIGURE 1: TEMPERATURE-PRESSURE PROFILES & CONDENSATION ASYMMETRY
    # -------------------------------------------------------------------------
    fig, ax = plt.subplots(figsize=(8.5, 6.0))

    ax.plot(t_day,
            p_bar,
            color="#e74c3c",
            lw=2.5,
            label="Dayside $T(P)$ (Hot Advection)")
    ax.plot(t_eve,
            p_bar,
            color="#e67e22",
            lw=2.8,
            label="Evening Terminator $T(P)$ (Clear Limb)")
    ax.plot(t_mor,
            p_bar,
            color="#2980b9",
            lw=2.8,
            label="Morning Terminator $T(P)$ (Cloudy Limb)")
    ax.plot(t_nit,
            p_bar,
            color="#2c3e50",
            lw=2.2,
            linestyle="--",
            label="Nightside $T(P)$ (Cold Interior)")
    ax.plot(t_cond,
            p_bar,
            color="#8e44ad",
            lw=2.5,
            linestyle="-.",
            label=r"$\mathrm{MgSiO_3}$ Silicate Condensation Curve")

    # Cloud formation zone shading on morning limb
    mask_cloud = t_mor < t_cond
    ax.fill_betweenx(p_bar[mask_cloud],
                     t_mor[mask_cloud],
                     t_cond[mask_cloud],
                     color="#3498db",
                     alpha=0.20,
                     label="Morning Silicate Condensation Zone")

    ax.set_yscale("log")
    ax.invert_yaxis()
    ax.set_xlabel("Atmospheric Temperature [K]",
                  fontweight="bold",
                  fontsize=11.5)
    ax.set_ylabel("Pressure [bar]", fontweight="bold", fontsize=11.5)
    ax.set_title(
        "Frontier 4: 3D Day-Night & Evening-Morning Thermal Structure Asymmetry",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax.grid(True, linestyle=":", alpha=0.6)
    ax.set_xlim(800, 2600)
    ax.set_ylim(100.0, 1.0e-5)
    ax.legend(frameon=True, facecolor="white", fontsize=9.5, loc="lower left")

    plt.tight_layout()
    fig.savefig(out_dir / "fig1_terminator_chemistry_asymmetry.pdf",
                bbox_inches="tight")
    fig.savefig(out_dir / "fig1_terminator_chemistry_asymmetry.png",
                dpi=300,
                bbox_inches="tight")
    plt.close(fig)
    print("Generated fig1_terminator_chemistry_asymmetry.pdf")

    # -------------------------------------------------------------------------
    # FIGURE 2: SYNTHETIC JWST TRANSMISSION SPECTRA (INGRESS VS EGRESS)
    # -------------------------------------------------------------------------
    wl, d_mor, d_eve, d_sym, contrast = [], [], [], [], []
    with open(spec_file, "r") as f:
        reader = csv.DictReader(f)
        for row in reader:
            wl.append(float(row["wavelength_um"]))
            d_mor.append(float(row["depth_morning_ppm"]))
            d_eve.append(float(row["depth_evening_ppm"]))
            d_sym.append(float(row["depth_symmetric_ppm"]))
            contrast.append(float(row["contrast_ppm"]))

    wl = np.array(wl)
    d_mor = np.array(d_mor)
    d_eve = np.array(d_eve)
    d_sym = np.array(d_sym)
    contrast = np.array(contrast)

    fig, (ax1, ax2) = plt.subplots(2,
                                   1,
                                   figsize=(12, 7.5),
                                   sharex=True,
                                   gridspec_kw={"height_ratios": [2.2, 1.2]})

    ax1.plot(
        wl,
        d_eve,
        color="#e67e22",
        lw=2.5,
        label=
        r"Evening Limb (Ingress / Clear): Prominent $\mathrm{H_2O, CO_2, CO}$")
    ax1.plot(wl,
             d_mor,
             color="#2980b9",
             lw=2.5,
             label=r"Morning Limb (Egress / Cloudy): Muted Silicate Deck")
    ax1.plot(wl,
             d_sym,
             color="gray",
             lw=1.8,
             linestyle="--",
             label=r"1D Standard Symmetric Model Average")

    # Molecular labels
    ax1.annotate(r"$\mathrm{H_2O}$ (1.4 $\mu$m)",
                 xy=(1.4, 15250),
                 xytext=(1.25, 15450),
                 fontsize=9.5,
                 fontweight="bold",
                 color="#2c3e50")
    ax1.annotate(r"$\mathrm{H_2O}$ (2.7 $\mu$m)",
                 xy=(2.7, 15500),
                 xytext=(2.55, 15700),
                 fontsize=9.5,
                 fontweight="bold",
                 color="#2c3e50")
    ax1.annotate(r"$\mathrm{CO_2}$ (4.3 $\mu$m)",
                 xy=(4.3, 15750),
                 xytext=(4.10, 15950),
                 fontsize=9.5,
                 fontweight="bold",
                 color="#c0392b")
    ax1.annotate(r"$\mathrm{CO}$ (4.65 $\mu$m)",
                 xy=(4.65, 15400),
                 xytext=(4.55, 15600),
                 fontsize=9.5,
                 fontweight="bold",
                 color="#8e44ad")

    ax1.set_ylabel(r"Transit Depth $(R_p/R_\star)^2$ [ppm]",
                   fontweight="bold",
                   fontsize=11)

    ax1.set_title(
        "JWST NIRSpec/PRISM Synthetic Transmission Asymmetry: WASP-39b / WASP-76b Benchmark",
        fontweight="bold",
        fontsize=12,
        pad=10)
    ax1.grid(True, linestyle=":", alpha=0.6)
    ax1.legend(frameon=True, facecolor="white", fontsize=9.5, loc="upper left")

    # Contrast subplot
    ax2.plot(
        wl,
        contrast,
        color="#27ae60",
        lw=2.5,
        label=
        r"Evening - Morning Asymmetry Signal $\Delta D_{\rm limb}(\lambda)$")
    ax2.axhline(0, color="black", lw=1.0, linestyle=":")
    ax2.set_xlabel(r"Wavelength $\lambda$ [$\mu$m]",
                   fontweight="bold",
                   fontsize=11.5)
    ax2.set_ylabel(r"$\Delta D$ [ppm]", fontweight="bold", fontsize=11)
    ax2.grid(True, linestyle=":", alpha=0.6)
    ax2.legend(frameon=True, facecolor="white", fontsize=9.5, loc="lower right")

    plt.tight_layout()
    fig.savefig(out_dir / "fig2_jwst_transmission_asymmetry.pdf",
                bbox_inches="tight")
    fig.savefig(out_dir / "fig2_jwst_transmission_asymmetry.png",
                dpi=300,
                bbox_inches="tight")
    plt.close(fig)
    print("Generated fig2_jwst_transmission_asymmetry.pdf")

    # -------------------------------------------------------------------------
    # FIGURE 3: CLOUD MICROPHYSICS & SLANT OPTICAL DEPTH
    # -------------------------------------------------------------------------
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5.2))

    ax1.plot(c_mor * 1.0e3,
             p_bar,
             color="#2980b9",
             lw=2.8,
             label=r"Morning Limb Cloud Mass [g / kg$_{\rm gas}$]")
    ax1.plot(c_eve * 1.0e3,
             p_bar,
             color="#e67e22",
             lw=2.5,
             linestyle="--",
             label=r"Evening Limb Cloud Mass (0.0)")
    ax1.set_yscale("log")
    ax1.invert_yaxis()
    ax1.set_xlabel("Cloud Condensate Density [g / kg$_{\\rm gas}$]",
                   fontweight="bold",
                   fontsize=11)
    ax1.set_ylabel("Pressure [bar]", fontweight="bold", fontsize=11)
    ax1.set_title("Kinetic Aerosol Mass Fraction Profile",
                  fontweight="bold",
                  fontsize=12)
    ax1.grid(True, linestyle=":", alpha=0.6)
    ax1.set_ylim(10.0, 1.0e-4)
    ax1.legend(frameon=True, facecolor="white", fontsize=9.5)

    ax2.plot(
        tau_1,
        p_bar,
        color="#8e44ad",
        lw=2.8,
        label=r"Morning Slant Optical Depth $\tau_{\rm slant}(1\,\mu\mathrm{m})$"
    )
    ax2.axvline(1.0,
                color="red",
                linestyle=":",
                lw=2.0,
                label=r"$\tau_{\rm slant} = 1.0$ (Photosphere Transition)")
    ax2.set_xscale("log")
    ax2.set_yscale("log")
    ax2.invert_yaxis()
    ax2.set_xlabel(r"Slant Optical Depth $\tau_{\rm slant}$",
                   fontweight="bold",
                   fontsize=11)
    ax2.set_ylabel("Pressure [bar]", fontweight="bold", fontsize=11)
    ax2.set_title("Slant Optical Depth & Transit Photosphere",
                  fontweight="bold",
                  fontsize=12)
    ax2.grid(True, linestyle=":", alpha=0.6)
    ax2.set_xlim(1.0e-3, 1.0e3)
    ax2.set_ylim(10.0, 1.0e-4)
    ax2.legend(frameon=True, facecolor="white", fontsize=9.5)

    plt.tight_layout()
    fig.savefig(out_dir / "fig3_cloud_microphysics_optical_depth.pdf",
                bbox_inches="tight")
    fig.savefig(out_dir / "fig3_cloud_microphysics_optical_depth.png",
                dpi=300,
                bbox_inches="tight")
    plt.close(fig)
    print("Generated fig3_cloud_microphysics_optical_depth.pdf")
    print("All 3 Frontier 4 discovery figures generated successfully!")


if __name__ == "__main__":
    main()
