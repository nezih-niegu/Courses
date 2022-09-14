library(readr)
library(grid)
library(gridExtra)
library(readxl)
library(dplyr)
library(tidyverse)
library(gdata)
library(scientoText)
coneval_2010_2015 <- read_csv("C:/Users/ANP10/Desktop/Tareas tec/Semestre_Ago20/Laboratorio de Ciencia de Datos/coneval_2010_2015.csv")
    
View(coneval_2010_2015)
Del_mun <- read_csv("C:/Users/ANP10/Desktop/Tareas tec/Semestre_Ago20/Laboratorio de Ciencia de Datos/Del_mun.csv")
View(Del_mun)
coneval_2010_2015<- na.omit(coneval_2010_2015)
Del_mun<- na.omit(Del_mun)
p<-as.data.frame(coneval_2010_2015 %>%
                   group_by(ent) %>%
                   summarise(Pob = sum(pob_2015)))
View(p)

q<-as.data.frame(coneval_2010_2015 %>%
                   group_by(ent) %>%
                   summarise(mPob = mean(pob_2015)))
View(q)

grid.table(q)
grid.table(p)

base_final<- merge(Del_mun, coneval_2010_2015)
View(base_final)
base_final <-subset(base_final,t==2015)
linearMod <-lm(base_final$dic~base_final$pobr_por_2015)
summary(linearMod)
#keep(del2015_msure=TRUE)
coneval_2010_2015<-subset(coneval_2010_2015,select=-X11)
coneval_2010_2015 <- na.omit(coneval_2010_2015) 
Del_mun <-na.omit(Del_mun) 
Del_mun<-subset(Del_mun,select=-c(cve_ent,ent,mun))

base_final <- merge(coneval_2010_2015,Del_mun) 
base_final <-subset(base_final,t==2015)
keep(base_final,sure=TRUE)

base_final$tot_del<-rowSums(base_final[16:27],na.rm = FALSE)
base_reg1<-base_final[,c("cve_mun","ene","feb","mar","abr","may"
                         ,"jun","jul","ago","sep","oct","nov","dic","tot_del")]
base_reg1<-aggregate(. ~ cve_mun, data=base_reg1, sum, na.rm=TRUE)

base_reg2<-base_final[,c("cve_mun","pob_2015","pobr_pob_2015")]
base_reg2<-aggregate(. ~ cve_mun, data=base_reg2, max, na.rm=TRUE)
base_reg<-merge(base_reg1,base_reg2)
base_reg_n<-base_final[,c("cve_mun","mun","cve_ent","ent")]
#Regresión Lineal
linearMod <- lm(base_reg$tot_del ~ base_reg$pobr_pob_2015)
base_reg$tot_del_pob<-base_reg$tot_del/base_reg$pob_2015
base_reg$pobr_pob_2015_pob<-base_reg$pobr_pob_2015/base_reg$pob_2015

#Relación espuria
base_reg$pobr_ind<-base_reg$pobr_pob_2015_pob-min(base_reg$pobr_pob_2015_pob)
base_reg$pobr_ind<-(base_reg$pobr_ind/max(base_reg$pobr_ind))*100
summary(base_reg$pobr_ind)

base_reg$tot_del_ind<-base_reg$tot_del_pob-min(base_reg$tot_del_pob)
base_reg$tot_del_ind<-(base_reg$tot_del_ind/max(base_reg$tot_del_ind))*100
summary(base_reg$tot_del_ind)



linearMod <- lm(tot_del_ind ~ pobr_ind,data=base_reg)
summary(linearMod)
scatter.smooth(base_reg$pobr_ind,base_reg$tot_del_ind)
hist(base_reg$pobr_ind)
hist(base_reg$tot_del_ind)
