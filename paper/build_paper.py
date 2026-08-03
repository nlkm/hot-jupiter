"""
Modular CLI Paper Compilation System with Process Locking.

Build Tiers:
  1. Default (no flags): Fast LaTeX compilation using existing vector PDF figures (< 1.5s)
  2. --figures / --figs: Re-render vector graphics from saved simulation data & compile (< 3s)
  3. --analysis        : Re-run full numerical simulation analyses from scratch & compile
"""

import argparse
import os
import sys
import shutil
import subprocess

LOCK_FILE = os.path.abspath(os.path.join(os.path.dirname(__file__), ".paper_build.lock"))


def acquire_lock():
    if os.path.exists(LOCK_FILE):
        try:
            with open(LOCK_FILE, "r") as f:
                pid = int(f.read().strip())
            # Check if process with pid is still running
            os.kill(pid, 0)
            print(f"⚠️  Another paper build process is already running (PID {pid}). Exiting.")
            sys.exit(0)
        except (OSError, ValueError):
            # Stale lockfile
            os.remove(LOCK_FILE)

    with open(LOCK_FILE, "w") as f:
        f.write(str(os.getpid()))


def release_lock():
    if os.path.exists(LOCK_FILE):
        try:
            os.remove(LOCK_FILE)
        except OSError:
            pass


def main():
    acquire_lock()
    try:
        parser = argparse.ArgumentParser(description="Modular Paper Build System")
        parser.add_argument("--figures", "--figs", action="store_true", help="Re-render vector figures and compile LaTeX")
        parser.add_argument("--analysis", action="store_true", help="Re-run all numerical simulation analyses from scratch")
        parser.add_argument("--jupiter", action="store_true", help="Re-run Jupiter benchmark simulation")
        parser.add_argument("--population", action="store_true", help="Re-run Population synthesis simulation")
        parser.add_argument("--all", action="store_true", help="Re-run all simulations and re-render figures")
        args = parser.parse_args()

        repo_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
        paper_dir = os.path.join(repo_dir, "paper")
        outputs_dir = os.path.join(repo_dir, "outputs")
        figures_dir = os.path.join(paper_dir, "figures")

        os.makedirs(outputs_dir, exist_ok=True)
        os.makedirs(figures_dir, exist_ok=True)

        fig1 = os.path.join(outputs_dir, "jupiter_cooling_track.pdf")
        fig2 = os.path.join(outputs_dir, "jupiter_internal_profile.pdf")
        fig3 = os.path.join(outputs_dir, "hot_jupiter_incremental_ks_comparison.pdf")
        fig4 = os.path.join(outputs_dir, "hot_jupiter_coupled_orbital_spin_evolution.pdf")
        fig5 = os.path.join(outputs_dir, "multi_planet_system_evolution.pdf")
        fig6 = os.path.join(outputs_dir, "stellar_misaligned_orbit_evolution.pdf")

        # Tier 3: Re-running Numerical Simulation Analysis
        run_all_sims = args.analysis or args.all
        env = os.environ.copy()
        env["PYTHONPATH"] = repo_dir

        if run_all_sims or args.jupiter or not (os.path.exists(fig1) and os.path.exists(fig2)):
            print("--> [Tier 3: Analysis] Running examples/jupiter_cooling.py...")
            subprocess.run([sys.executable, os.path.join(repo_dir, "examples", "jupiter_cooling.py")], cwd=repo_dir, env=env, check=True)

        if run_all_sims or args.population or not os.path.exists(fig3):
            print("--> [Tier 3: Analysis] Running examples/hot_jupiter_population_study.py...")
            subprocess.run([sys.executable, os.path.join(repo_dir, "examples", "hot_jupiter_population_study.py")], cwd=repo_dir, env=env, check=True)

        if run_all_sims or not os.path.exists(fig4):
            print("--> [Tier 3: Analysis] Running examples/hot_jupiter_coupled_orbital_spin.py...")
            subprocess.run([sys.executable, os.path.join(repo_dir, "examples", "hot_jupiter_coupled_orbital_spin.py")], cwd=repo_dir, env=env, check=True)

        if run_all_sims or not os.path.exists(fig5):
            print("--> [Tier 3: Analysis] Running examples/multi_planet_system_benchmark.py...")
            subprocess.run([sys.executable, os.path.join(repo_dir, "examples", "multi_planet_system_benchmark.py")], cwd=repo_dir, env=env, check=True)

        if run_all_sims or not os.path.exists(fig6):
            print("--> [Tier 3: Analysis] Running examples/stellar_misaligned_orbit_scenario.py...")
            subprocess.run([sys.executable, os.path.join(repo_dir, "examples", "stellar_misaligned_orbit_scenario.py")], cwd=repo_dir, env=env, check=True)

        # Tier 2: Syncing Vector Figures to paper/figures/
        print("--> [Tier 2: Figures] Syncing all vector PDF figures from outputs/ to paper/figures/...")
        for fname in os.listdir(outputs_dir):
            if fname.endswith(".pdf"):
                src = os.path.join(outputs_dir, fname)
                dst = os.path.join(figures_dir, fname)
                shutil.copy(src, dst)
                print(f"    Synced {fname}")

        # Tier 1: Fast pdflatex compilation
        print("--> [Tier 1: LaTeX] Compiling paper.tex to PDF using pdflatex...")
        tex_file = "paper.tex"

        res1 = subprocess.run(["pdflatex", "-interaction=nonstopmode", tex_file], cwd=paper_dir, capture_output=True, text=True)
        if res1.returncode != 0:
            print("pdflatex Pass 1 Warning/Error:")
            print(res1.stdout[-1000:])

        subprocess.run(["pdflatex", "-interaction=nonstopmode", tex_file], cwd=paper_dir, capture_output=True, text=True)

        pdf_file = os.path.join(paper_dir, "paper.pdf")
        if os.path.exists(pdf_file):
            size_kb = os.path.getsize(pdf_file) / 1024.0
            print(f"\n✅ SUCCESS: Compiled {pdf_file} ({size_kb:.1f} KB)")
        else:
            print("\n❌ ERROR: Failed to generate paper.pdf")
            sys.exit(1)
    finally:
        release_lock()


if __name__ == "__main__":
    main()
