# samples = fread(
#   file = "/home/silvio/Documents/OneGenE_FANTOM/hgnc_data_mat.csv",
#   sep = ',',
#   header = TRUE,
#   data.table = F
# )
# rownames(samples) = samples$V1 #Setting the rownames from the first column
# samples = samples[,-1] #Deleting the first column
# rotated = t(samples) #To rotate the matrix, might delete now if not for efficiency of iteration
#
# output = data.frame()
#
# n = 1
# for (a in 1:nrow(rotated)) {
#   for (b in 1:nrow(rotated)) {
#     #must check if already calculated
#     if (!(colnames(rotated)[a] == colnames(rotated)[b])) {
#       res = cor(rotated[, a], rotated[, b])
#       output[n, 1] = colnames(rotated)[a]
#       output[n, 2] = colnames(rotated)[b]
#       output[n, 3] = res
#       n = n + 1
#     }
#   }
# }
#
# fwrite(
#   output,
#   file = "/home/silvio/Documents/OneGenE_FANTOM/FANTOM_PearsonMatrix.csv",
#   eol = '\n',
#   quote = FALSE,
#   row.names = FALSE,
#   col.names = FALSE
# )

library(data.table)
matrix = fread(
  file = "/home/silvio/Documents/OneGenE_FANTOM/pearsonMatrix2.csv",
  sep = ',',
  header = FALSE
)

library(ggplot2)
ggplot() + geom_density(aes(matrix[, 3])) + geom_vline(aes(xintercept = mean(matrix[, 3])),
                                                       color = "blue",
                                                       size = 1)
ggsave("pearsonPlot.png", plot1, width = 7, height = 5, units = "in", dpi = 300, device = "png")
stats=matrix[,summary(V3)] #change V3 to correct index


