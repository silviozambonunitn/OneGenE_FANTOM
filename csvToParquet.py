import pandas as pd

# Read CSV file into a DataFrame
df = pd.read_csv('/home/silvio/onegene/FANTOM_unified.csv')

# Write DataFrame to a Parquet file
df.to_parquet('/home/silvio/onegene/FANTOM_unified.parquet')