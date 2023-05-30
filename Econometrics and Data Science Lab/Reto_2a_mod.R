library(readr)
library(grid)
library(gridExtra)
library(readxl)
library(dplyr)
library(tidyverse)

Del_mun <-read.csv("~/Downloads/Del_mun.csv", comment.char="#")
coneval_2010_2015<-read_csv("~/Downloads/coneval_2010_2015.csv", 
col_types = cols(cve_mun = col_number(), 
pob_2010 = col_number(), pob_2015 = col_number(), 
pobr_por_2010 = col_number(), pobr_por_2015 = col_number(),
pobr_pob_2010 = col_number(), pobr_pob_2015 = col_number() ))

library(gdata)
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

base_reg$tot_del_pob<-base_reg$tot_del/base_reg$pob_2015
base_reg$pobr_pob_2015_pob<-base_reg$pobr_pob_2015/base_reg$pob_2015
keep(base_reg,sure=TRUE)
linearMod <- lm(base_reg$tot_del ~ base_reg$pobr_pob_2015)
summary(linearMod)

linearMod <- lm(base_reg$tot_del_pob ~ base_reg$pobr_pob_2015_pob)
summary(linearMod)

scatter.smooth(base_reg$pobr_pob_2015_pob,base_reg$tot_del_pob)
title("Relación entre delitos y pobreza en México (2015)")
plot(base_reg$pobr_pob_2015_pob,base_reg$tot_del_pob)

linearMod <- lm(base_reg$tot_del_pob ~ base_reg$pobr_pob_2015_pob)
summary(linearMod)

base_reg$tot_del_dm<-base_reg$tot_del/10000
base_reg$pobr_pob_2015_dm<-base_reg$pobr_pob_2015/10000
linearMod <- lm(base_reg$tot_del_dm ~ base_reg$pobr_pob_2015_dm)
summary(linearMod)
scatter.smooth(base_reg$pobr_pob_2015_pob,base_reg$tot_del_pob)

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

linearMod <- lm(tot_del_ind ~ pobr_ind,data=base_reg)
summary(linearMod)

X <- matrix(c(rep(1, times = nrow(base_reg)),
              base_reg$pobr_ind), 
            ncol = 2)
H <- X %*% solve(t(X) %*% X) %*% t(X)
H1<-data.frame(x1 = base_reg$pobr_ind,
               H = diag(H))
summary(H1$H)
outlier <- hatvalues(linearMod) > 2 * mean(hatvalues(linearMod))
base_reg$outlier<-ifelse((hatvalues(linearMod) > 2 * mean(hatvalues(linearMod)))=="TRUE",1,0)
View(base_reg[outlier, ])

linearMod <- lm(tot_del_ind[outlier==0] ~ pobr_ind[outlier==0] ,data=base_reg)
summary(linearMod)
scatter.smooth(base_reg$tot_del_ind,base_reg$pobr_ind)

linearMod <- lm(tot_del_ind[outlier==0] ~ pobr_ind[outlier==0] + pmuj[outlier==0],data=base_reg)


