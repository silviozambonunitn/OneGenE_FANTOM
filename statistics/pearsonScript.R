library(data.table)
samples = fread(
  file = "/storage/shared/fantom/hgnc_data_mat.csv",
  sep = ',',
  header = TRUE,
  data.table = F
)
rownames(samples) = samples$V1 #Setting the rownames from the first column
samples = samples[, -1] #Deleting the first column
rotated = t(samples) #To rotate the matrix, might delete now if not for efficiency of iteration
rm(samples)
#rotated = rotated[, 1:200]

library(HiClimR)
matrix = fastCor(
  rotated,
  optBLAS = TRUE,
  verbose = TRUE,
  upperTri = TRUE,
  nSplit = 40
)

library(reshape2)
matrix=setNames(melt(matrix), c('Transcript1', 'Transcript2', 'PearsonCoeff'))
matrix=matrix[complete.cases(matrix),]

print(summary(matrix[, 3]))

fwrite(
  matrix,
  file = "/storage/shared/fantom/FANTOM_PearsonMatrixR_tri.csv",
  eol = '\n',
  quote = FALSE,
  row.names = FALSE,
  col.names = FALSE
)

library(ggplot2)
ggplot() +
  theme_bw() +
  geom_density(aes(matrix[, 3])) +
  geom_vline(aes(xintercept = mean(matrix[, 3])),
             color = "blue") +
  labs(x = "Pearson correlation coefficient", y = "Density", title = "Distribution", caption = "Blue line = mean")+
  scale_x_continuous(breaks = seq(-0.75,1,0.25))

ggsave(
  filename = "pearsonDistr.png",
  width = 7,
  height = 5,
  units = "in",
  dpi = 300,
  device = "png"
)
