

base <- read.csv("C:/Users/ANP10/Desktop/Tareas tec/Semestre_Ago20/Laboratorio de Ciencia de Datos/reto1_PIB.csv")
#creats variables that allow us to manipulate the intel
base$a <- as.numeric(substr(base$t,1,4))
base$m <- as.numeric(substr(base$t,6,7))
#We'll need a library to declare a new variable with the method make_datetime
library(lubridate)
base$fecha <- make_datetime(base$a,base$m)
attach(base)
summary(PIB)
prop.table(table(m))

library(gmodels)
CrossTable(m[a<2010], prop.r=TRUE, misssing.include=FALSE)

base$trim <- factor(base$m, levels=c(1,2,3,4), labels = c("T1", "T2", "T3", "T4"))
CrossTable(base$trim, prop.r=TRUE, missing.include=FALSE)

#aggregate(base$PIB[a==1999 & m==1], list(base$trim),mean)

aggregate(base[,2], list(base$trim),mean)

tabla1<-aggregate(base[,2], list(base$trim), mean)
tabla1
colnames(tabla1)<-c("Trimestre","PIB")
#View(tabla1)
hist(PIB)
library(ggplot2)
library(tidyverse)
library(hrbrthemes)
hist_PIB <-ggplot(base, aes(x=PIB)) + geom_histogram()
hist_PIB
#Histogram with graphic changes
hist_PIB <-ggplot(base, aes(x=PIB)) + geom_histogram(fill="#0ecfcb",color="#ffffff", alpha=0.9)+ggtitle("Histograma del PIB (1980-2020)")
hist_PIB
#Plotted Series
line_PIB <-ggplot(base, aes(x=fecha, y=PIB))+geom_line(color="#0ecfcb",size=1, alpha=0.9,linetype=1)
line_PIB

line_PIB <- base%>%filter(a>=2006)%>%ggplot(aes(x=fecha, y=PIB))+geom_line(color="#0ecfcb", size=1, alpha=0.9, linetype=1)+ theme_ipsum()+ ggtitle("Comportamiento del PIB (2006 -2020)")
line_PIB#error
#Variations ratios
base$PIB_L1 <- lag(base$PIB, n = 1L, default=NA,order_by=fecha)
base$PIB_vper<-(base$PIB-base$PIB_L1)*100/base$PIB_L1
line_PIB_vper<-ggplot(base, aes(x=fecha, y=PIB_vper))+ geom_line(color="#0ecfcb", size=1, alpha=0.9, linetype=1)
line_PIB_vper
