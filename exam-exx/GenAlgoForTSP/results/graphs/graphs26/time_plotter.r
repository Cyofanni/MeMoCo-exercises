#run 'png(time12.png)' from R shell
generations <- c(100,200,300)
time <- c(2.17,2.24,2.27)

plot(generations, time, main="Running time against number of generations for 500 individuals", sub="",  xlab="generations", ylab="time (in seconds)",
xlim=c(100, 300), ylim=c(0, 3), lty=1)

lines(generations,time,col="red",lty=1)
#run 'dev.off()' from R shell

