setwd(here::here())
library(data.table)
samples = fread(file='pearsonCorrelation/hgnc_data_mat.csv', sep=',', header = TRUE, data.table=F)
matrix=cor(samples)