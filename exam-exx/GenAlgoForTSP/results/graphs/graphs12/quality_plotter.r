#run 'png(quality.png)' from R shell
generations <- c(10,20,100)
quality <- c(97.32,97.21,100)

plot(generations, quality, main="Quality against number of generations for 300 individuals", sub="",  xlab="generations", ylab="quality (in % of the optimum)",
xlim=c(0, 100), ylim=c(90, 100), lty=1)

lines(generations,quality,col="blue",lty=1)
#run 'dev.off()' from R shell

