install.packages("rgdal")
library(rgdal)
library(readr)
library(grid)
library(gridExtra)
library(readxl)
library(dplyr)
library(tidyverse)
library(gdata)

tmp <- tempdir()
url <- "http://personal.tcu.edu/kylewalker/data/mexico.zip"
file <- basename(url)
download.file(url, file)
unzip(file, exdir = tmp)
mexico <- readOGR(dsn = tmp, layer = "mexico", encoding = "UTF-8")
plot(mexico)
mexico_df <- as.data.frame(mexico, xy = TRUE)
plot(mexico)

Del_mun <-read.csv("~/Downloads/Del_mun.csv", comment.char="#")

Del_mun$tot_del<-rowSums(Del_mun[10:21],na.rm = FALSE)
Del_mun<-na.omit(Del_mun)

Del_ent <- as.data.frame(Del_mun %>%
                         group_by(cve_ent & t==2015) %>%
                         summarise(Tdel = sum(tot_del)))

colnames(Del_ent)<-c("id","Del")
estados <- merge(mexico, Del_ent)
library(RColorBrewer)
my_colors <- brewer.pal(9, "Blues") 
my_colors <- colorRampPalette(my_colors)(32)

cl_edo <- cut(estados@data$Del, 32)
my_colors <- my_colors[as.numeric(cl_edo)]
plot(estados , col=my_colors)

title("Delitos en México año 2015")



###############################################

library(cartography)
estados@data$Del<-estados@data$Del
mexico_df$Del<-estados@data$Del
choroLayer(spdf = estados, df = estados@data, var = "Del")

choroLayer(spdf = estados, var = "Del",
           legend.pos = "bottomright",
           legend.values.cex = 0.8,legend.values.rnd=2)


