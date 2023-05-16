library(readxl)
library(data.table)
samples = fread(
  file = "/storage/shared/fantom/hgnc_data_mat.csv",
  sep = ',',
  header = TRUE,
  data.table = F
)
samples[2:nrow(samples), ]
rownames(samples) = samples$V1 #Setting the rownames from the first column
samples = samples[, -1] #Deleting the first column
rotated = t(samples) #To rotate the matrix
res = cor(rotated, method = "pearson")
write.csv(res,
          file = "/storage/shared/fantom/FANTOM_PearsonMatrix.csv",
          eol = '\n',
          quote = FALSE)

#s=summary(res) #trash
#write.csv(res, file="/storage/shared/fantom/FANTOM_PM_Summary.csv", eol = '\n', quote = FALSE)
