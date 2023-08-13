import pandas as pd
import numpy as np

# Load data from the CSV file
df = pd.read_csv('/storage/shared/fantom/FANTOM_Matrix_names.csv', delimiter=';')

# Calculate absolute values of correlations
df['PearsonCorrelation'] = df['PearsonCorrelation'].abs()

# Calculate Relative Frequency - Absolute Pearson Correlation
df['Diff_Freq_Corr'] = df['RelativeFrequency'] - df['PearsonCorrelation']

# Calculate Absolute Pearson Correlation - Relative Frequency 
df['Diff_Corr_Freq'] = df['PearsonCorrelation'] - df['RelativeFrequency']

# Find the rows with maximum difference (in both ways)
freq_corr_row = df[df['Diff_Freq_Corr'] == df['Diff_Freq_Corr'].max()]
corr_freq_row = df[df['Diff_Corr_Freq'] == df['Diff_Corr_Freq'].max()]

# If there's more than one pair with the same max difference, select the first one
max_pair_freq_corr = freq_corr_row.iloc[0]
max_pair_corr_freq = corr_freq_row.iloc[0]

# Print pairs
print(f"Relative Frequency - Pearson Correlation: Seed = {max_pair_freq_corr['Seed']}, Leaf = {max_pair_freq_corr['Leaf']}")
print(f"Pearson Correlation - Relative Frequency: Seed = {max_pair_corr_freq['Seed']}, Leaf = {max_pair_corr_freq['Leaf']}")
