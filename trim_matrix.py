#Keeps only the isoform with the lowest pn@gene for each gene
import pandas as pd

# Load data from the CSV file
df = pd.read_csv('/storage/shared/fantom/FANTOM_Matrix_names.csv', sep=';')

# Split 'Seed' into 'gene' and 'n'
df['gene'] = df['Seed'].str.split('@').str[1]
df['n'] = df['Seed'].str.split('@').str[0].str.split('p').str[1].astype(int)

# Get the index of the row with the smallest 'n' for each gene
min_n_indices = df.groupby('gene')['n'].idxmin()

# Select only the rows with these indices
df_min_n = df.loc[min_n_indices]

# Save to new CSV file
df_min_n.to_csv('/storage/shared/fantom/FANTOM_Matrix_first_isoform.csv', sep=';', index=False)
