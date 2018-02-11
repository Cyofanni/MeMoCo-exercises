#run 'png(quality.png)' from R shell
generations <- c(100,300,500)
quality2 <- c(87.44,87.79,90.9)	   #simulated annealing


plot(generations, quality2, main="Quality against number of generations for 1000 individuals", sub="",  xlab="generations", ylab="quality (in % of the optimum)",
xlim=c(100, 500), ylim=c(80, 100), lty=1)

lines(generations,quality2,col="blue",lty=1)
#run 'dev.off()' from R shell

