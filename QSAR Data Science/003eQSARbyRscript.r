# script 003cQSARbyRscript.r
# source(003dQSARbyRscript.r)
echo=FALSE;eval=TRUE
------------------------------------------------ 
# options(continue=" ") 
print("___QSAR with R program __file 003e.QSARbyR.txt____")
print("___R program for QSAR workshop, 27 X 2021")
print("Professor: FCQ, BUAP, Dr. Tom Scior")
# tscior@gmail.com FCQ BUAP
print("Name: ITESM, Nezih Nieto")
# a01732702@tec.mx 
print("_____________ASCII code to generate keyboard characters")
print(" Alt 34 \" ")
print(" Alt 47 / ")
print(" Alt 92 \ ")
print(" pAlt 126  ~ ")
print("___QSAR by SLR and MLR___with___R commands , cmds")
# print(" older version from ___QSAR workshop 27 viii  19, by Dr. Tom Scior")
print("------------------------- Starting with the R demo for QSAR now  ------")
getwd()
# setwd("C:\\19TSdata\\19_QSAR_Taller_FCQ_Ma27vii19")
getwd()
setwd("D:\\21_Data_work_Dell_29vii21\\21_Unit_TecMont_ITESM\\R_curso_taller_v19_FCQ\\004_R_QSAR_scripts+data_ok")
print("In the next step, please choose the file 004edemoQSARinput.csv ")
DS=read.csv(file.choose())
print(" first 6 lines of DataSet in 004edemoQSARinput.csv here : ")
head(DS)
str(DS)
summary(DS)
#
# 1a. Analisis preliminar (Grafico)
print(" buscar solo los valores relevantes para un LM en la tabla=matriz=data.frame in R ")
DS4LM <- data.frame(DS$DV, DS$IDV1, DS$IDV2, DS$IDV3)
print(" hacer un analisis preliminar mediante una x,y grafico= plot in R ")
plot(DS4LM)
#
# 1.b. Analisis preliminar (Numerico)
#         Correlaciones entre los valores de variables, vars 
#         Valores cercanos a 1 y -1 indican alta correlacion entre 
#         las variables. Valores cercanos a 0 indican correlacion 
#         baja o no correlacion.
print(" correlation between variables, DV, IDV1, IDV2, IDV3 in a correlation matrix CORMAT ")
print(" interpretar valores cercanos a 1 y -1 que indican alta correlacion entre vars ") 
print(" y valores cercanos a 0 indican una mala correlacion o ninguna correlacion ")
CORMAT = cor(with(DS, cbind(DV, IDV1, IDV2, IDV3)))
print(CORMAT)
#
print(" concluimos del analisis preliminar numerico y grafico: .... ") 
print(" parece haber correlaci�n entre las variables IDVs = x versus DV = y ")
print(" la DV reelmente depende de las IDVs = InDependent Variables ")
#
print(" negative/positive value = decreasing/increasing line in a x,y plot ")
par(mfrow=c(2,2), bg = "green", bty= "n", cex=.75, col="blue")
plot(DS$IDV1, DS$DV)
plot(DS$IDV2, DS$DV)
plot(DS$IDV3, DS$DV)
plot(0)
# END PLOT ########################### END PLOT
############################ END CORrelation MATrix infos 
#
#
par(mfrow=c(1,1), bg = "red")
plot(1)
#
# 
############################# START 
print (" tomar DS4LM es lo mismo que CORMAT,  o sea DataSet for Linear Model ")  
cor(DS4LM)
#
print(" correlation : compare two variables y = DV vs x = IDV ")
print(" correlation plot = x,y plot = 2D with pairs of (x,y) points = pointwise ")
print(" correlation matrix and correlation graph are the same, please compare ")
# 
par(mfrow=c(2,2), bg = "yellow", bty= "n", cex=.75)
plot(DS4LM)
# plot(y ~ x, data=ds)
# Do not plot MLR only x against y correlattions in a 2D graph
#
# END PLOT ########################### END PLOT
#
#
par(mfrow=c(1,1), bg = "red")
plot(1)
#
# 
############################## START
# 2. Construccion del modelo multiple linear regression = MLR
print(" regression between x and y = simple linear model = SLR ")
print(" regression with 2 variables or more = DV, IDV1 , IDV2 , IDV3 etc ")
print(" regression plot, regression to the mean value = least square line in plot ") 
print(" coeficiente de x,y correlacion r = pendiente de la recta ")
print(" coeficiente de determinacion R2 = goodness of fit, predicting Y by X ") 
print(" R2 da la proporcion de varianza en Y inducida por X ") 
print(" R2 = 0.7 =  7 x 7 = 49 = 50 % la mitad de la varianza en Y se explica por X ")
SLR1 = lm(DV ~ IDV1, data = DS)
SLR2 = lm(DV ~ IDV2, data = DS)
SLR3 = lm(DV ~ IDV3, data = DS)
print(" verify correlation by R2 values") 
summary(SLR1)$r.squared
summary(SLR2)$r.squared
summary(SLR3)$r.squared
#
print(" R2 indica que el modelo dos con IDV2 versus DV is the best = highest R2 ")
#
################################ END 
# 3a. Q S A R 
print(" many IDVs and one DV for multiple linear regression model = MLR ")
MLR1 = lm(DV ~ IDV1 + IDV2 + IDV3, data = DS)
print(" many IDVs yield better MLR models , see R2 here : " )
summary(MLR1)$r.squared
print(" ____ Q S A R ______  o sea la ecuacion lineal y = a + bx + b2x2 ..." )
print(" ver el coeficiente de determinacion = R2 ")
print(" mirar el Multiple_R_Cuadrado que indica que porcentaje % de la ")
print(" la variabilidad de los valores se explica por la ecuacion lineal = modelo QSAR ")
###################################
# 3b.)  QSAR eq 
print(" El   Q S A R   tiene la eq grl y = Intercept + beta1 x1 + beta2 x2 + beta3 x3 ... ")
print(" los coefficientes beta son estimatados, vease in summary los Estimates ")
# Resultado Evaluacion: El modelo es 
#           y= ? +? x1 + ? x2 + ? x3
# 3.c Resultado QSAR modelo 
print(" write   Q S A R   equation with DV = intercept + beta1 IDV1 + beta2 IDV2 ....")
summary(MLR1)
print(" test all possible combinations of IDVs and see R2 " )
print(" choose final model with R2>7 and few IDVs ")
print(" 6 : 1 = number of Mols VS. number of IDVs, eg. 12:2, 24:4, 48:8 etc.")
print(" Q S A R  model is :  DV = 3.394 + 0.00666 IDV1 + 0.00005 IDV2 - 0.00738 IDV3 ")
print(" take five decimal positions after decimal point because of IDV2 with e-5 power ")
#
#
par(mfrow=c(1,1), bg = "red")
plot(1)
#
# 3c.)    GRAPHICAL TEST of the QSAR model by four MLR plots
##############################################
par(mfrow=c(2,2), bg = "green", bty= "n", cex=.75)
# plot(SLR1)
# plot(SLR2)
# plot(SLR3)
plot(MLR1)
# END PLOT ########################### END PLOT
###
# 4) Homocedasticidad (hipotesis de varianza constante)
print(" If in MLR, the variance of the dependent variable is the same for all input data = Homoscedasticity.")
print(" It facilitates analysis because most methods are based on the assumption of equal variance = Homoscedasticity.")
print(" Ho Hipotesis de varianza sin cambios entre los valores = homocedasticity ") 
print(" mirar el grafico llamado Residuals vs Fitted ")
print(" observar que la anchura de los valores es aproximadamente igual = spread ")
###
##############################################
# 5) Test de la distribucion Normal, normalidad 
print(" Shapiro Test is a statistical significance test with Ho = Dnorm, ")
print(" for small sample size n=20,30,40, ... But if n>>100, trival differences ")
print(" Mirar el grafico llamado Normal_Q_-_Q , con la linea de regresion calculada ")
print(" donde los valores deben de estar lo mas proximos posibles a la linea discontinua ")
print(" regression : compare NORMAL Q-Q plots of SLR models ")
print(" good correlation shows straight line in Normal_Q_-_Q ")
## NORMALITY TEST 
shapiro.test(DS$DV)
print("The null-hypothesis H0 of this statistical significance test is that the population is normally distributed.") 
print("sample size >>30, 80, this test may detect even trivial differences from the null hypothesis : ")
print(" i.e. they are detected but practically insignificant. Better is Q-Q plot. ")
print("if the p value is greater than the chosen alpha level, H0 is valid ")
# The significance level alpha, is the probability of the study rejecting the null hypothesis H0, 
# given that the null hypothesis were true, and is chosen before data collection. 
# The p-value of a event/value, p, is the probability of obtaining an extreme value, 
# given that the null hypothesis H0 were true. 
print("The occurance of a value, = event, is statistically significant, when p >= alpha . alpha=0.05=5%")
##
########## END OF QSAR DEMO
# 6)  QUESTIONS 
print(" TAREA buscar el modelo QSAR final entre los posibles modelos ") 
print(" calcular todas las combinaciones, DV versus una, dos o tres IDVs y anotar R2 " )
print(" test all possible combinations of IDVs and see R2 " )
print(" choose final model with R2>7 and few IDVs ")
print(" AND choose another final model with best R2>>> over all other R2 and many IDVs ")
#
#### The END 
print("_END__R DEMO for QSAR workshop")
print("_END__R DEMO END, Laboratorio de Simulaciones Moleculares,  T H A N K    Y O U  :-) ")
