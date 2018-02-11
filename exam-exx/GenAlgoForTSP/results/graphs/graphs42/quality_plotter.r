#run 'png(quality.png)' from R shell
generations <- c(100,200,300)
quality1 <- c(90.24,91.08,95.6)    #no simulated annealing
quality2 <- c(90.9,93.25,97.51)	   #simulated annealing


plot(generations, quality1, main="Quality against number of generations for 800-500 individuals", sub="",  xlab="generations", ylab="quality (in % of the optimum)",
xlim=c(100, 300), ylim=c(90, 100), lty=1)

lines(generations,quality1,col="blue",lty=1)


points(generations, quality2, col="red", pch="*")
lines(generations, quality2, col="red",lty=2)

legend(101, 98, legend=c("With S.A.,500 indiv.", "Without S.A.,800 indiv."),
       col=c("red", "blue"), lty=2:1, cex=0.8)

#run 'dev.off()' from R shell
