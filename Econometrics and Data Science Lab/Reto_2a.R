library(readr)
library(grid)
library(gridExtra)
library(readxl)
library(dplyr)
library(tidyverse)

Del_mun <-read.csv("~/Downloads/Del_mun.csv", comment.char="#")
coneval<-read.csv("~/Downloads/coneval_2010_2015.csv")
library(gdata)
keep(del2015_msure=TRUE)
coneval_2010_2015<-subset(coneval_2010_2015,select=-X11)
coneval_2010_2015 <- na.omit(coneval_2010_2015) 
Del_mun <-na.omit(Del_mun) 

library(dplyr)
p<-as.data.frame(coneval_2010_2015 %>%
                   group_by(ent) %>%
                   summarise(Pob = sum(as.numeric(pob_2015)))
View(p)

q<-as.data.frame(coneval_2010_2015 %>%
                   group_by(ent) %>%
                   summarise(mPob = mean(pob_2015)))
View(q)

grid.table(q)

base_final <- merge(del2015_mun,coneval_2010_2015) 
base_final <-subset(base_final,t==2015)
linearMod <- lm(base_final$dic ~ base_final$pobr_por_2015)
summary(linearMod)


r<-as.data.frame(cbind(p$nom_ent,p$Pob,q$mPob))
View(r)

colnames(r)<-c("Edo","pob_tot","pob_med")
grid.table(r)
r
#p<-aggregate(coneval[, 5:8], list(coneval$nom_ent), sum,na.action = na.omit)

#"#56B4E9"
pal <- wes_palette(name = "Zissou1")
p1<-ggplot(data=p, aes(x=reorder(nom_ent,Pob),y=Pob)) +
  geom_bar(stat="identity", fill="#56B4E9",position=position_dodge()) +
  scale_fill_manual(values="#8fa38e")  
p1 + coord_flip()+labs(x = "Entidad", y = "Población")

coneval1 <- p[with(p,order(-Pob)),]
coneval1 <- coneval1[1:10,]
coneval1$Pob<-format(coneval1$Pob, big.mark = ",", decimal.mark = ".")
colnames(coneval1)<-c("Entidad","Población")
grid.table(coneval1,rownames(FALSE))

library(gdata)
keep(coneval,sure=TRUE)
del2015 <- read_excel("Downloads/Municipal-Delitos-2015-2020_mar2020/2015a.xlsx")
coneval$cve_ent<-as.numeric(coneval$cve_ent)
coneval$cve_mun<-as.numeric(coneval$cve_mun)
#coneval <- merge(del2015,coneval,by="cve_mun")
coneval <- merge(del2015,coneval) 
keep(coneval,sure=TRUE)
con<-coneval[,10:19]
View(coneval[,10:19])
coneval$tot_del<- rowSums(con)

View(coneval[,c(5,8:19,57)])

p<-as.data.frame(coneval[coneval$cve_ent,] %>%
                   group_by(bja) %>%
                   summarise(Del = sum(tot_del)))
grid.table(p)

p$t_del <- sum(p$Del)
p$per<-round((p$Del/p$t_del)*100,2)
grid.table(p)
View(p)

p1<-ggplot(data=p, aes(x=reorder(bja,-per),y=per)) +
  geom_bar(stat="identity", fill="#8fa38e",position=position_dodge())+
  scale_fill_manual(values="#8fa38e")  
p1 + coord_flip()+labs(x = "", y = "Concentración")
p1<-ggplot(data=p, aes(x=reorder(bja,-Del),y=Del)) +
  geom_bar(stat="identity", fill="#8fa38e",position=position_dodge())+
  scale_fill_manual(values="#8fa38e")  
p1 + coord_flip()+labs(x = "", y = "Delitos")

p$Del<-format(p$Del, big.mark = ",", decimal.mark = ".")

cor(coneval$tot_del, coneval$pobr_per_2015)
View(coneval[,c("nom_mun","tot_del","pobr_per_2015")])
scatter.smooth(x=coneval$tot_del, 
               y=coneval$pobr_por_2015, 
               main="Delitos ~ Pobreza") 

p<-as.data.frame(coneval %>%
                   group_by(cve_mun) %>%
                   summarise(Del = sum(tot_del)))
p1<-as.data.frame(coneval %>%
                    group_by(cve_mun) %>%
                    summarise(Pobr = max(pobr_per_2015)))
p2<-as.data.frame(coneval %>%
                    group_by(cve_mun) %>%
                    summarise(Pob = max(pob_2015)))

cor(p$Del, p1$Pobr)
scatter.smooth(p1$Pobr,p$Del)
linearMod <- lm(p$Del ~ p1$Pobr)
summary(linearMod)
con_del<-merge(p,p1)
con_del<-merge(con_del,p2)
con_del$Del_per<-con_del$Del/con_del$Pob
con_del$Pobr_per<-con_del$Pobr/con_del$Pob
con_del$Del_mil<-con_del$Del/10000
con_del$Pobr_mil<-con_del$Pobr/10000
cor(coneval$tot_del, coneval$pobr_per_2015)
cor(coneval$tot_del, coneval$pobr_per_2015)
con_del<-merge(con_del,con_nom)
cor(con_del$Del_per, con_del$Pobr_per)
scatter.smooth(con_del$Pobr_per, con_del$Del_per)

cor(con_del$Del_per, con_del$Pobr_per)
linearMod <- lm(con_del$Del_per ~ con_del$Pobr_per)
summary(linearMod)

cor(con_del$Del_mil, con_del$Pobr_mil)
scatter.smooth(con_del$Pobr_mil, con_del$Del_mil)
linearMod <- lm(con_del$Pobr_mil ~ con_del$Del_mil)
summary(linearMod)




