library(readxl)
samples = fread(file="~/Documents/OneGenE_FANTOM/pearsonCorrelation/hgnc_data_mat.csv", sep=',', header = TRUE, data.table=F)
samples[2:nrow(samples),]
rownames(samples) <- samples$V1
samples <- samples[,-1]
res <- cor(samples)
write.csv(res, file="~/Documents/OneGenE_FANTOM/pearsonCorrelation/FANTOM_OneGenE_PearsonMatrix.csv", eol = '\n', quote = FALSE)
#which(res == min(res), arr.ind = TRUE)
median(res)
mean(res)
