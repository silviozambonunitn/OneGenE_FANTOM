import pandas as pd

# Read the dictionary file
df_dict = pd.read_csv('/storage/shared/fantom/tcode-gene.csv', delimiter='|', header=None, index_col=0).to_dict()
df_dict = df_dict.squeeze().to_dict()

# Read the matrix file
df_matrix = pd.read_csv('/storage/shared/fantom/FANTOM_unified.csv', delimiter=';', skiprows=1, header=None)

# Apply the dictionary to the matrix and write to the output file
df_matrix[[0, 1]] = df_matrix[[0, 1]].applymap(df_dict.get)
df_matrix.to_csv("/storage/shared/fantom/FANTOM_RelativeFrequencyMatrix_names.csv", sep=";", header=['Seed;Leaf;RelativeFrequency;PearsonCorrelation;relfreq-|pearson|'], index=False)
