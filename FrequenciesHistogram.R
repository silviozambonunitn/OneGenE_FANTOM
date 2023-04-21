frequencies <- read.csv(file='matrix.csv', sep=';', colClasses = c("character","character","numeric"))
#hist(frequencies)

library(ggplot2)
ggplot(frequencies, aes(RelativeFrequency))+
  geom_histogram(bins = 50)+
  theme_bw()
#ggsave("", device = cairo_pdf, width = 7, height = 5)