library(readxl)
library(data.table)
samples = fread(
  file = "/storage/shared/fantom/hgnc_data_mat.csv",
  sep = ',',
  header = TRUE,
  data.table = F
)
rownames(samples) = samples$V1 #Setting the rownames from the first column
samples = samples[, -1] #Deleting the first column
rotated = t(samples) #To rotate the matrix, might delete now if not for efficiency

output = data.frame()

n = 1
for (a in 1:nrow(rotated)) {
  for (b in 1:nrow(rotated)) {
    #check if already calculated
    if (TRUE) {
      res = cor(rotated[, a], rotated[, b])
      output[n, 1] = colnames(rotated)[a]
      output[n, 2] = colnames(rotated)[b]
      output[n, 3] = res
      n = n + 1
    }
  }
}

write.csv(output,
          file = "/storage/shared/fantom/FANTOM_PearsonMatrix.csv",
          eol = '\n',
          quote = FALSE)

library(ggplot2)
p = ggplot(data = output[, 3]) + geom_density() + geom_vline(aes(xintercept = mean(output[, 3])),
                                                             color = "blue",
                                                             size = 1)
ggsave(
  filename = "corrDistr.jpeg",
  path = "/storage/shared/fantom/",
  plot = p,
  device = "svg"
)

#s=summary(res) #trash
#write.csv(res, file="/storage/shared/fantom/FANTOM_PM_Summary.csv", eol = '\n', quote = FALSE)
