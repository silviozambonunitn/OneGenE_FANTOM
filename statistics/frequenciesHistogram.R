library(ggplot2)
library(data.table)

#opening and parsing csv matrix file
setwd(here::here())
frequencies = fread(
  file = '/storage/shared/fantom/FANTOM_RelativeFrequencyMatrix.csv',
  sep = ';',
)

plot=ggplot(frequencies, aes(RelativeFrequency)) +
  scale_x_continuous(breaks = seq(0, 1, 0.05)) +
  geom_histogram(bins = 50) +
  theme_bw()

ggsave("histogram_plot.png", plot, width = 7, height = 5, units = "in", dpi = 300)