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
gc(verbose = F)
#rotated = rotated[, 1:200]

library(HiClimR)
matrix = fastCor(
  rotated,
  optBLAS = TRUE,
  verbose = TRUE,
  upperTri = TRUE,
  nSplit = 35
)

fwrite(matrix,
       file = "/storage/shared/fantom/FANTOM_PearsonMatrixR_tri.csv",
       eol = '\n',
       quote = FALSE)

vectorized = c(matrix[-1, ])
rm(matrix)
gc(verbose = F)

vectorized = na.omit(vectorized)
gc(verbose = F)

length(vectorized)
print(summary(vectorized))

#To save the vector
#saveRDS(vectorized, file = "/storage/shared/fantom/corrvector.rds")

library(ggplot2)
ggplot() +
  theme_bw() +
  geom_density(aes(vectorized)) +
  geom_vline(aes(xintercept = mean(vectorized)),
             color = "blue") +
  labs(
    x = "Pearson correlation coefficient",
    y = "Density",
    title = "Distribution",
    caption = "Blue line = mean"
  ) +
  scale_x_continuous(breaks = seq(-0.75, 1, 0.25))

#To save the plot
ggsave(
  path = "/storage/shared/fantom/",
  filename = "pearsonDistr.png",
  width = 7,
  height = 5,
  units = "in",
  dpi = 300,
  device = "png"
)

#To lad the vector
v = readRDS("/storage/shared/fantom/corrvector.rds")
#To plot in two lines
library(ggplot2)
ggplot() +
  theme_bw() +
  geom_density(aes(v[c(T, F)])) +
  geom_density(aes(v[c(F, T)])) +
  labs(x = "Pearson correlation coefficient",
       y = "Density",
       title = "Distribution") +
  scale_x_continuous(breaks = seq(-0.75, 1, 0.25))
