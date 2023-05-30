install.packages("rgdal")
install.packages('spDataLarge',
                 repos='https://nowosad.github.io/drat/',
                 type='source')
library(rgdal)
library(readr)
library(grid)
library(gridExtra)
library(readxl)
library(dplyr)
library(tidyverse)
library(gdata)
library(stringr)
library(rgdal)
library(sf)
library(raster)
library(dplyr)
library(spData)
library(tmap)    # for static and interactive maps
library(leaflet) # for interactive maps
library(mapview) # for interactive maps
library(ggplot2) # tidyverse data visualization package
library(shiny)   # for web applications
library(sp)
tmp <- tempdir()
url <- "http://personal.tcu.edu/kylewalker/data/mexico.zip"
file <- basename(url)
download.file(url, file)
unzip(file, exdir = tmp)
mexico <- readOGR(dsn = tmp, layer = "mexico", encoding = "UTF-8")
plot(mexico)
title("Mapa de México")

mexico_df <- as.data.frame(mexico, xy = TRUE)
plot(mexico)

Del_mun <-read.csv("~/Downloads/Del_mun.csv", comment.char="#")

Del_mun$tot_del<-rowSums(Del_mun[10:21],na.rm = FALSE)
Del_mun<-na.omit(Del_mun)

Del_mun_2015<-subset(Del_mun,t==2015)
Del_ent <- as.data.frame(Del_mun_2015 %>%
                         group_by(cve_ent) %>%
                         summarise(Tdel = sum(tot_del)))

colnames(Del_ent)<-c("id","Del")
estados <- merge(mexico, Del_ent)

library(RColorBrewer)
my_colors <- brewer.pal(9,"Blues") 
my_colors <- colorRampPalette(my_colors)(32)

cl_edo <- cut(estados@data$Del, 32)
my_colors <- my_colors[as.numeric(cl_edo)]
plot(estados , col=my_colors)

title("Delitos en México año 2015")

###############################################

library(cartography)

estados1<-st_read("~/Downloads/Mapa/00ent.shp")
colnames(estados1)<-c("id","cve_ent","nom","geometry")
estados1$id<-as.numeric(estados1$id)

plot(estados1)


#######################

Del_mun_2016<-subset(Del_mun,t==2016)
table(Del_mun_2016$ent)
Del_ent <- as.data.frame(Del_mun_2016 %>%
                           group_by(cve_ent) %>%
                           summarise(Tdel = sum(tot_del)))
colnames(Del_ent)<-c("id","Del")

estados1 <- merge(estados1, Del_ent)

choroLayer(spdf = estados1,  var = "Del",legend.pos = c(20, 20),
           legend.title.txt="Delitos", legend.title.cex=1,
           legend.values.cex = 0.8,nclass=5,
           method = "quantile")

######################
library(sf)
coneval_2010_2015<-read_csv("~/Downloads/coneval_2010_2015.csv", 
                            col_types = cols(cve_mun = col_number(), 
                                             pob_2010 = col_number(), pob_2015 = col_number(), 
                                             pobr_por_2010 = col_number(), pobr_por_2015 = col_number(),
                                             pobr_pob_2010 = col_number(), pobr_pob_2015 = col_number() ))
coneval_2010_2015<-subset(coneval_2010_2015,select=-X11)
coneval_2010_2015<-na.omit(coneval_2010_2015)
colnames(coneval_2010_2015)[1]<-c("id")
table(coneval_2010_2015$id)
Pob_ent <- as.data.frame(coneval_2010_2015 %>%
                           group_by(id) %>%
                           summarise(Tpob = sum(pob_2015)))

Pobr_ent <- as.data.frame(coneval_2010_2015 %>%
                           group_by(id) %>%
                           summarise(Tpobr = sum(pobr_pob_2015)))

Del_ent <- as.data.frame(Del_mun_2015 %>%
                           group_by(cve_ent) %>%
                           summarise(Tdel = sum(tot_del)))
colnames(Del_ent)<-c("id","Del")

base<-merge(Del_ent,Pobr_ent) 
base<-merge(base,Pob_ent)
base$del_per<-base$Del/base$Tpob
base$pobr_per<-base$Tpobr/base$Tpob

estados1<-st_read("~/Downloads/Mapa/00ent.shp")
colnames(estados1)<-c("id","cve_ent","nom","geometry")
estados1$id<-as.numeric(estados1$id)
estados1 <- merge(estados1, base)

tm_shape(estados1) +
  tm_borders()

plot(st_geometry(estados1), col = "grey60",border = "white",
     lwd=0.1, bg = "lightsteelblue1")

propSymbolsChoroLayer(x = estados1, var = "del_per", var2 = "pobr_per",
                      col = carto.pal(pal1 = "blue.pal", n1 = 3,
                                      pal2 = "red.pal", n2 = 3),
                      inches = 0.1, method = "q6",
                      border = "grey50", lwd = 1,
                      legend.var.pos = (c(100,50)),
                      legend.var2.pos = "n",
                      legend.var.values.rnd = 2,
                      legend.var2.values.rnd = 2,
                      legend.var2.title.txt = "Pobreza",
                      legend.var.title.txt = "Delitos",
                      legend.values.cex=0.8,legend.var.style = "e")

legendChoro(pos = "bottomleft", title.txt = "Title of the legend", title.cex = 0.8,
            values.cex = 0.8, breaks = c(1,2,3,4,10.27,15.2),
            col = carto.pal(pal1 = "orange.pal",n1 = 5), values.rnd =2,
            nodata = TRUE, nodata.txt = "No data available", frame = TRUE, symbol="box")