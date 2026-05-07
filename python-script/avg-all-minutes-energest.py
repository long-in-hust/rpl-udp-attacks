#!/usr/bin/python3
import pandas as pd

file_path = "../output-artefacts/energest-csv/energy_results_decrank_attack.csv"

df = pd.read_csv(file_path, sep=";")
df.columns = df.columns.str.strip()
df["State"] = df["State"].str.strip()
df["Value"] = pd.to_numeric(df["Value"], errors="coerce")

# Keep only the metrics you want
metrics = df[df["State"].isin(["CPU", "Radio total"])]

# Average across the whole CSV
result = (
    metrics.groupby("State", as_index=False)["Value"]
    .mean()
    .rename(columns={"Value": "AverageValue"})
)

print(result.to_string(index=False))

# Optional: save the averages
result.to_csv("average_cpu_radio_total.csv", index=False)
print("\nSaved to average_cpu_radio_total.csv")