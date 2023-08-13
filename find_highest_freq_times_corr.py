import pandas as pd
    
# Load data from the CSV file
df = pd.read_csv('/storage/shared/fantom/FANTOM_Matrix_names.csv', sep=';')

# Calculate absolute values of correlations
df['PearsonCorrelation'] = df['PearsonCorrelation'].abs()

# Calculate product of absolute correlation and relative frequency
df['Product'] = df['PearsonCorrelation'] * df['RelativeFrequency']

# Find the row with the maximum product
max_product_row = df[df['Product'] == df['Product'].max()]

# If there's more than one pair with the same max product, select the first one
max_pair = max_product_row.iloc[0]

print(max_pair)