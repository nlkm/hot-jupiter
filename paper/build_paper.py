"""
Modular CLI Paper Compilation System for paper/paper.tex using Vector Graphics (PDF).

Usage:
  python3 paper/build_paper.py             # Fast PDF compilation using cached vector figures (< 2 seconds)
  python3 paper/build_paper.py --jupiter   # Re-run ONLY Jupiter benchmark simulation & compile
  python3 paper/build_paper.py --population# Re-run ONLY Population synthesis simulation & compile
  python3 paper/build_paper.py --all       # Re-run ALL simulation analyses from scratch & compile
"""

import argparse
import os
import shutil
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser(description="Modular Paper Build System (Vector PDF Figures)")
    parser.add_argument("--jupiter", action="store_true", help="Re-run Jupiter benchmark simulation")
    parser.add_argument("--population", action="store_true", help="Re-run Population synthesis simulation")
    parser.add_argument("--all", action="store_true", help="Re-run ALL simulations from scratch")
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

    # Step 1: Run selective simulation tasks if requested or missing
    if args.jupiter or args.all or not (os.path.exists(fig1) and os.path.exists(fig2)):
        print("--> Running examples/jupiter_cooling.py (Vector Graphics)...")
        env = os.environ.copy()
        env["PYTHONPATH"] = repo_dir
        subprocess.run([sys.executable, os.path.join(repo_dir, "examples", "jupiter_cooling.py")], cwd=repo_dir, env=env, check=True)

    if args.population or args.all or not os.path.exists(fig3):
        print("--> Running examples/hot_jupiter_population_study.py (Vector Graphics)...")
        env = os.environ.copy()
        env["PYTHONPATH"] = repo_dir
        subprocess.run([sys.executable, os.path.join(repo_dir, "examples", "hot_jupiter_population_study.py")], cwd=repo_dir, env=env, check=True)

    # Step 2: Sync vector figures to paper/figures/
    print("--> Syncing vector PDF figures to paper/figures/...")
    for fig_path in [fig1, fig2, fig3]:
        if os.path.exists(fig_path):
            shutil.copy(fig_path, figures_dir)
            print(f"    Synced {os.path.basename(fig_path)}")

    # Step 3: Fast pdflatex compilation
    print("--> Compiling paper.tex to PDF using pdflatex...")
    tex_file = "paper.tex"

    # Pass 1
    res1 = subprocess.run(["pdflatex", "-interaction=nonstopmode", tex_file], cwd=paper_dir, capture_output=True, text=True)
    if res1.returncode != 0:
        print("pdflatex Pass 1 Warning/Error:")
        print(res1.stdout[-1000:])

    # Pass 2
    subprocess.run(["pdflatex", "-interaction=nonstopmode", tex_file], cwd=paper_dir, capture_output=True, text=True)

    pdf_file = os.path.join(paper_dir, "paper.pdf")
    if os.path.exists(pdf_file):
        size_kb = os.path.getsize(pdf_file) / 1024.0
        print(f"\n✅ SUCCESS: Compiled {pdf_file} ({size_kb:.1f} KB)")
    else:
        print("\n❌ ERROR: Failed to generate paper.pdf")
        sys.exit(1)


if __name__ == "__main__":
    main()
