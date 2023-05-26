library(data.table)
samples = fread(
  file = "/storage/shared/fantom/hgnc_data_mat.csv",
  sep = ',',
  header = TRUE,
  data.table = F
)
rownames(samples) = samples$V1 #Setting the rownames from the first column
samples = samples[,-1] #Deleting the first column
rotated = t(samples) #To rotate the matrix, might delete now if not for efficiency of iteration
rm(samples)
library(HiClimR)
output = fastCor(
  rotated,
  upperTri = TRUE,
  optBLAS = TRUE,
  verbose = TRUE,
  nSplit = 1
)
csvout = na.omit(as.data.frame(as.table(output)))

fwrite(
  csvout,
  file = "/storage/shared/fantom/FANTOM_PearsonMatrix2.csv",
  eol = '\n',
  quote = FALSE,
  row.names = FALSE,
  col.names = FALSE
)

library(ggplot2)
plot1 = ggplot() +
  geom_density(aes(csvout[, 3])) +
  geom_vline(aes(xintercept = mean(csvout[, 3])),
             color = "blue",
             size = 1)

ggsave(
  path = "/storage/shared/fantom/",
  filename = "pearsonPlot.png",
  plot1,
  width = 7,
  height = 5,
  units = "in",
  dpi = 300,
  device = "png"
)
stats = csvout[, summary(Freq)]
