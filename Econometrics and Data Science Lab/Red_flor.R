data(iris)
head(iris)
set.seed(42)
fold.test <- sample(nrow(iris), nrow(iris) / 3)
test <- iris[fold.test, ]
train <- iris[-fold.test, ]

ann <- neuralnet(as.numeric(Species) ~ Sepal.Length + Sepal.Width + 
                   Petal.Length + Petal.Width, train, hidden = c(10,5))
ann

plot(ann, rep = "best")

par(mfrow=c(2,2))
gwplot(ann, selected.covariate = "Sepal.Length")
gwplot(ann, selected.covariate = "Sepal.Width")
gwplot(ann, selected.covariate = "Petal.Length")
gwplot(ann, selected.covariate = "Petal.Width")
output <- compute(ann, test[ , c("Sepal.Length", "Sepal.Width", 
                                 "Petal.Length", "Petal.Width")])

result <- data.frame(
  Real = test$Species, 
  Predicted = levels(iris$Species)[round(output$net.result)])

table(result$Predicted, result$Real)

round(prop.table(table(result$Predicted, result$Real),1),2)

