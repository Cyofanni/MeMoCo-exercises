#run 'png(quality.png)' from R shell
generations <- c(100,200,300)
quality <- c(95.86, 95.40, 99.32)

plot(generations, quality, main="Quality against number of generations for 500 individuals", sub="",  xlab="generations", ylab="quality (in % of the optimum)",
xlim=c(100, 300), ylim=c(90, 100), lty=1)

lines(generations,quality,col="blue",lty=1)
#run 'dev.off()' from R shell

