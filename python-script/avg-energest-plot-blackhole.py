#!/usr/bin/env python3
from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd


BASE_DIR = Path(__file__).resolve().parent
INPUT_DIR = BASE_DIR.parent / "output-artefacts" / "energest-csv-blackhole"
FIGURES_DIR = BASE_DIR.parent / "output-artefacts" / "figures"
MINUTE_TICKS = 60_000_000
METRICS = ["CPU", "Radio Total"]


def read_energy_csv(file_path: Path) -> pd.DataFrame:
    df = pd.read_csv(file_path, sep=";")
    df.columns = df.columns.str.strip()

    for column in ["Time", "MoteID", "State", "Value"]:
        if column not in df.columns:
            raise ValueError(f"{file_path.name} is missing required column '{column}'")

    df["State"] = df["State"].astype(str).str.strip()
    df["StateNorm"] = df["State"].str.lower()
    df["MoteID"] = pd.to_numeric(df["MoteID"], errors="coerce")
    df["Value"] = pd.to_numeric(df["Value"], errors="coerce")
    df["Time"] = pd.to_numeric(df["Time"], errors="coerce")
    df = df.dropna(subset=["Time", "MoteID", "Value"])
    df["MoteID"] = df["MoteID"].astype(int)
    df["Minute"] = (df["Time"] // MINUTE_TICKS).astype(int)
    return df[df["Minute"] >= 1]


def load_scenario_series(file_path: Path, metric: str) -> pd.Series:
    df = read_energy_csv(file_path)
    metric_norm = metric.lower()

    metric_df = df[df["StateNorm"] == metric_norm].copy()
    if metric_df.empty and metric_norm == "radio total":
        radio_parts = df[df["StateNorm"].isin(["radio tx", "radio rx"])].copy()
        if radio_parts.empty:
            return pd.Series(dtype=float)

        radio_parts = (
            radio_parts.pivot_table(
                index=["Minute", "MoteID"],
                columns="StateNorm",
                values="Value",
                aggfunc="mean",
            )
            .reset_index()
            .fillna(0)
        )
        radio_parts["Value"] = radio_parts["radio tx"] + radio_parts["radio rx"]
        metric_df = radio_parts[["Minute", "MoteID", "Value"]]
    elif metric_df.empty:
        return pd.Series(dtype=float)

    per_node_df = metric_df.groupby(["Minute", "MoteID"], as_index=False)["Value"].mean()
    return per_node_df.groupby("Minute", as_index=True)["Value"].mean().sort_index()


def plot_scenario(metric: str, scenario_label: str, series_map: dict[str, pd.Series], output_path: Path) -> None:
    plt.figure(figsize=(12, 6))

    plotted = False
    for label in ["normal", "attack", "prevention"]:
        series = series_map.get(label)
        if series is None or series.empty:
            continue
        plt.plot(series.index, series.values, marker="o", label=label.capitalize())
        plotted = True

    if not plotted:
        plt.close()
        return

    plt.title(f"Average Network Energy Metrics - {scenario_label}")
    plt.xlabel("Simulation Time (minutes)")
    plt.ylabel("Average Ticks (Energy Consumption)")
    plt.grid(True, linestyle="--", alpha=0.7)
    plt.legend()
    plt.xticks(rotation=45)
    plt.tight_layout()
    plt.savefig(output_path)
    plt.close()
    print(f"Saved {output_path}")


def main() -> None:
    FIGURES_DIR.mkdir(parents=True, exist_ok=True)

    normal_file = INPUT_DIR / "energy_results_blackhole_noattack.csv"
    if not normal_file.exists():
        raise FileNotFoundError(f"{normal_file} not found")

    normal_series = {metric: load_scenario_series(normal_file, metric) for metric in METRICS}

    attack_files: dict[str, dict[str, Path]] = {}
    for file_path in sorted(INPUT_DIR.glob("energy_results_*_*.csv")):
        scenario_key = file_path.stem[len("energy_results_"):]
        if scenario_key.endswith("_attack"):
            attack_type = scenario_key[: -len("_attack")]
            attack_files.setdefault(attack_type, {})["attack"] = file_path
        elif scenario_key.endswith("_prevention"):
            attack_type = scenario_key[: -len("_prevention")]
            attack_files.setdefault(attack_type, {})["prevention"] = file_path

    if not attack_files:
        print(f"No attack/prevention CSV files found in {INPUT_DIR}")
        return

    for attack_type, files_by_role in sorted(attack_files.items()):
        series_by_role = {"normal": normal_series}
        for role in ["attack", "prevention"]:
            file_path = files_by_role.get(role)
            if file_path is None:
                continue
            series_by_role[role] = {
                metric: load_scenario_series(file_path, metric) for metric in METRICS
            }

        for metric in METRICS:
            metric_series_map = {"normal": normal_series[metric]}
            if "attack" in series_by_role:
                metric_series_map["attack"] = series_by_role["attack"][metric]
            if "prevention" in series_by_role:
                metric_series_map["prevention"] = series_by_role["prevention"][metric]

            output_path = FIGURES_DIR / f"energy_plot_{attack_type}_{metric.lower().replace(' ', '_')}.png"
            plot_scenario(metric, attack_type, metric_series_map, output_path)


if __name__ == "__main__":
    main()