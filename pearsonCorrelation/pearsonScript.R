library(readxl)
library(data.table)
samples = fread(file="~/Documents/OneGenE_FANTOM/pearsonCorrelation/hgnc_data_mat.csv", sep=',', header = TRUE, data.table=F) #Change if not running on the rdds pc
samples[2:nrow(samples),]
rownames(samples) <- samples$V1 #Setting the rownames from the first column
samples <- samples[,-1] #Deleting the first column
rotated=t(samples) #To rotate the matrix
res = cor(rotated, method="pearson")
write.csv(res, file="~/Documents/OneGenE_FANTOM/pearsonCorrelation/FANTOM_OneGenE_PearsonMatrix.csv", eol = '\n', quote = FALSE) #Change if not running on the rdds pc
#which(res == min(res), arr.ind = TRUE)
median(res)
mean(res)
