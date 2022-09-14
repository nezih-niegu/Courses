
library(readr)
library(grid)
library(gridExtra)
library(readxl)
library(dplyr)
library(tidyverse)
del2015 <- read_xls("C:/Users/ANP10/Desktop/Tareas tec/Semestre_Ago20/Laboratorio de Ciencia de Datos/reto2_FueCom.xls")
del2015$tot_del<- rowSums(del2015[,10:21])
View(del2015[,22])
del2015<-na.omit(del2015)
#lista2015<-as.data.frame(aggregate(del2015$tot_del, list(del2015$ent), sum))
#lista2015 <- lista2015[order(lista2015$x),]#lista de menor a mayor
#lista2015 <- lista2015[order(-lista2015$x),]#lista de menor a mayor
#lista2015
del_bja<-as.data.frame(del2015 %>%
                         group_by(t,ent,bja) %>%
                         summarise(Del = sum(tot_del)))#extraer por Campo o variable (t,ent,bja)
View(del_bja)

#colnames(del_bja)<-c("Bien Jurídico","No. Delitos")
#grid.table(del_bja)

del_bja<-as.data.frame(del2015 %>%
                         group_by(bja) %>%
                         summarise(Del = sum(tot_del)))

del_bja$t_del <- sum(del_bja$Del)
del_bja$del_con<-round((del_bja$Del/del_bja$t_del)*100,2)
View(del_bja)

del_bja$t_del <- sum(del_bja$Del)
del_bja$del_con<-round((del_bja$Del/del_bja$t_del)*100,2)
table(del2015$bja)
del_bja_s<-as.data.frame(del2015[del2015$bja=="La sociedad",] %>%
                           group_by(tdd,sdd) %>%
                           summarise(Del = sum(tot_del)))
View(del_bja_s)
del_bja_p<-as.data.frame(del2015[del2015$bja=="El patrimonio",] %>%
                           group_by(tdd,sdd) %>%
                           summarise(Del = sum(tot_del)))
View(del_bja_p)
#https://www.coneval.org.mx/Medicion/Paginas/Pobreza-municipal.aspx
#DESCARGAR PA' FUSIONAR