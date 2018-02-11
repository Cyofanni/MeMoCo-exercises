#run 'png(time.png)' from R shell
generations <- c(100,300,500)
time2 <- c(87.23,91.3,90.0)    #simulated annealing


plot(generations, time2, main="Running time against number of generations for 1000 indiv.", sub="",  xlab="generations", ylab="time (in seconds)",
xlim=c(100, 500), ylim=c(80, 100), lty=1)

lines(generations,time2,col="red",lty=1)

#run 'dev.off()' from R shell

