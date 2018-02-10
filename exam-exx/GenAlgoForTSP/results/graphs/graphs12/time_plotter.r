#run 'png(time12.png)' from R shell
generations <- c(10,20,100)
time <- c(0.07,0.13,0.36)

plot(generations, time, main="Running time against number of generations for 300 individuals", sub="",  xlab="generations", ylab="time (in seconds)",
xlim=c(0, 100), ylim=c(0, 1), lty=1)

lines(generations,time,col="red",lty=1)
#run 'dev.off()' from R shell

