#opening and parsing csv matrix file
setwd(here::here())
frequencies = read.csv(file='/storage/shared/fantom/FANTOM_RelativeFrequencyMatrix.csv', sep=';', colClasses = c("character", "character", "numeric"))

library(ggplot2)
ggplot(frequencies, aes(RelativeFrequency))+
  scale_x_continuous(breaks=seq(0, 1, 0.05))+
  geom_histogram(bins = 50)+
  theme_bw()