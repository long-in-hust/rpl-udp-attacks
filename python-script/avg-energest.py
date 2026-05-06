#!/usr/bin/python3
import pandas as pd

# Load the generated CSV file
file_path = '../output-artefacts/energest-csv/energy_results_dis_attack.csv'

try:
    # Read CSV with the semicolon separator used in the Cooja script
    df = pd.read_csv(file_path, sep=';')

    # 1. Clean column names (strip whitespace)
    df.columns = df.columns.str.strip()
    
    # 2. Convert 'Time' from simulation string to seconds/minutes
    # Cooja time is often 'mm:ss.ms' or just a raw tick count
    # For simplicity, we look at unique timestamp entries as 'Periods'
    
    # 3. Group by Time and State, then sum the 'Value' (ticks) across all nodes
    # 'Value' represents the ticks spent in that state during that specific interval
    summary = df.groupby(['Time', 'State'])['Value'].sum().reset_index()

    print("--- Total Energy Metrics for All Nodes per Interval ---")
    print(summary.to_string(index=False))

    # Optional: Save summary to a new file
    summary.to_csv('total_energy_summary_dis_attack.csv', index=False)
    print("\nSummary saved to 'total_energy_summary_dis_attack.csv'")

except FileNotFoundError:
    print(f"Error: {file_path} not found. Ensure the Cooja script has finished running.")
except Exception as e:
    print(f"An error occurred: {e}")