#! /usr/bin/env python3
import pandas as pd
import matplotlib.pyplot as plt

# Configuration
file_path = '../output-artefacts/energest-csv/energy_results_dis_prevented.csv'
minute_ticks = 60000000
selected_motes = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13]

try:
    # Read the CSV
    df = pd.read_csv(file_path, sep=';')
    
    # Clean column names and data
    df.columns = df.columns.str.strip()
    df['State'] = df['State'].str.strip()

    # Keep only selected motes (IDs may be strings in CSV)
    df['MoteID'] = pd.to_numeric(df['MoteID'], errors='coerce').astype(pd.Int64Dtype())
    df = df[df['MoteID'].isin(selected_motes)]

    # Convert Value to a numeric type just in case
    df['Value'] = pd.to_numeric(df['Value'])

    # Bucket samples into 1-minute intervals so the plot has one point per minute.
    # The first plotted bucket starts at the 1-minute mark.
    df['Minute'] = (pd.to_numeric(df['Time']) // minute_ticks).astype(int)
    df = df[df['Minute'] >= 1]

    # Average each mote first so nodes with more samples do not dominate the result.
    per_node_df = df.groupby(['Minute', 'MoteID', 'State'], as_index=False)['Value'].mean()

    # Then average those node-level values for each minute/state bucket.
    pivot_df = per_node_df.pivot_table(index='Minute', 
                                       columns='State', 
                                       values='Value', 
                                       aggfunc='mean').sort_index()

    # Calculate 'Radio Total' if it doesn't exist (Sum of Tx and Rx)
    if 'Radio Tx' in pivot_df.columns and 'Radio Rx' in pivot_df.columns:
        pivot_df['Radio Total'] = pivot_df['Radio Tx'] + pivot_df['Radio Rx']

    # Plot specific metrics relevant to RPL attacks
    metrics_to_plot = ['CPU', 'Radio Total']
    
    for metric in metrics_to_plot:
        if metric in pivot_df.columns:
            plt.figure(figsize=(12, 6))
            plt.plot(pivot_df.index, pivot_df[metric], marker='o', label=metric)

            plt.title('Average Network Energy Metrics (Per Node Average)')
            plt.xlabel('Simulation Time (minutes)')
            plt.ylabel('Average Ticks (Energy Consumption)')
            plt.grid(True, linestyle='--', alpha=0.7)
            plt.legend()
            plt.xticks(rotation=45)
            
            plt.tight_layout()
            if metric == 'Radio Total':
                name = 'radio_total'
            elif metric == 'CPU':
                name = 'cpu'

            plt.savefig(f'energy_plot_{name}_dis_prevented.png') # Saves the graph as an image
            plt.show()
            plt.close()

            print(f"Analysis Complete: Graph saved as 'energy_plot_{name}_dis_prevented.png'")

except FileNotFoundError:
    print(f"Error: {file_path} not found.")
except Exception as e:
    print(f"An error occurred: {e}")