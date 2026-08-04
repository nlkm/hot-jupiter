import csv

with open("outputs/nasa_exoplanet_archive_hot_jupiters_342.csv", "r") as f:
    reader = csv.DictReader(f)
    rows = list(reader)

latex_lines = []
latex_lines.append(r"\small")
latex_lines.append(r"\begin{longtable}{r l c c c c c p{4.2cm}}")
latex_lines.append(r"\caption{Complete NASA Exoplanet Archive catalog of confirmed transiting Hot Jupiters ($N = 342$, $P < 10\text{ d}, a < 0.10\text{ AU}$) with peer-reviewed literature discovery citations used for demographic evaluation.} \label{tab:full_342_catalog} \\")
latex_lines.append(r"\hline")
latex_lines.append(r"\textbf{\#} & \textbf{Planet Name} & \textbf{P [d]} & \textbf{a [AU]} & \textbf{$M_p$ [$M_{\mathrm{J}}$]} & \textbf{$R_p$ [$R_{\mathrm{J}}$]} & \textbf{$T_{\mathrm{eq}}$ [K]} & \textbf{Discovery / Reference} \\")
latex_lines.append(r"\hline")
latex_lines.append(r"\endfirsthead")
latex_lines.append(r"\multicolumn{8}{c}{\tablename\ \thetable{} -- continued from previous page} \\")
latex_lines.append(r"\hline")
latex_lines.append(r"\textbf{\#} & \textbf{Planet Name} & \textbf{P [d]} & \textbf{a [AU]} & \textbf{$M_p$ [$M_{\mathrm{J}}$]} & \textbf{$R_p$ [$R_{\mathrm{J}}$]} & \textbf{$T_{\mathrm{eq}}$ [K]} & \textbf{Discovery / Reference} \\")
latex_lines.append(r"\hline")
latex_lines.append(r"\endhead")
latex_lines.append(r"\hline \multicolumn{8}{r}{Continued on next page} \\ \hline")
latex_lines.append(r"\endfoot")
latex_lines.append(r"\hline")
latex_lines.append(r"\endlastfoot")

for r in rows:
    p_name = r['planet_name'].replace("_", r"\_")
    ref = r['reference'].replace("&", r"\&")
    line = f"{r['system_id']} & {p_name} & {r['period_days']} & {r['a_au']} & {r['M_p_Mjup']} & {r['R_p_Rjup']} & {r['T_eq_K']} & {ref} \\\\"
    latex_lines.append(line)

latex_lines.append(r"\end{longtable}")

with open("paper/sections/table_342_planets.tex", "w") as out:
    out.write("\n".join(latex_lines))

print(f"Generated formatted LaTeX longtable with discovery citations for all N = {len(rows)} planets in paper/sections/table_342_planets.tex")
