library(data.table)
pearsonMatrix = fread(file = "/home/silvio/onegene/FANTOM_PearsonMatrixR.csv",
                      sep = ',',
                      header = FALSE)
freqMatrix = fread(file = "/home/silvio/onegene/FANTOM_RelativeFrequencyMatrix.csv",
                   sep = ';',
                   header = TRUE)

#test = cbind(freqMatrix$RelativeFrequency, pearsonMatrix$V3)

library(dplyr)
table=left_join(freqMatrix, pearsonMatrix, join_by(Seed == V1, Leaf == V2))

fwrite(
  table,
  file = "/home/silvio/onegene/unified.csv",
  eol = '\n',
  quote = FALSE,
  row.names = FALSE,
  col.names = FALSE
)