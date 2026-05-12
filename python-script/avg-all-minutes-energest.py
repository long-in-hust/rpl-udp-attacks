#!/usr/bin/python3
import glob
import os
import pandas as pd

INPUT_DIR = os.path.join("..", "output-artefacts", "energest-csv")
PATTERN = os.path.join(INPUT_DIR, "energy_results_*.csv")

files = sorted(glob.glob(PATTERN))
if not files:
    print(f"No files found matching {PATTERN}")
    raise SystemExit(1)

for fp in files:
    base = os.path.basename(fp)
    # extract scenario name from energy_results_<scenario>.csv
    if base.startswith("energy_results_") and base.endswith(".csv"):
        scenario = base[len("energy_results_"):-len(".csv")]
    else:
        scenario = os.path.splitext(base)[0]

    print(f"Processing {base} (scenario: {scenario})")

    df = pd.read_csv(fp, sep=";")
    df.columns = df.columns.str.strip()
    if "State" in df.columns:
        df["State"] = df["State"].astype(str).str.strip()
    if "Value" in df.columns:
        df["Value"] = pd.to_numeric(df["Value"], errors="coerce")

    metrics = df[df["State"].isin(["CPU", "Radio total"])].copy()

    if metrics.empty:
        print(f"  No CPU/Radio total metrics found in {base}, skipping.")
        continue

    result = (
        metrics.groupby("State", as_index=False)["Value"]
        .mean()
        .rename(columns={"Value": "AverageValue"})
    )

    out_name = f"avg_cpu_radio_total_{scenario}.csv"
    out_path = os.path.join(INPUT_DIR, out_name)
    result.to_csv(out_path, index=False, sep=";")
    print(f"  Saved averages to {out_path}")

print("Done.")