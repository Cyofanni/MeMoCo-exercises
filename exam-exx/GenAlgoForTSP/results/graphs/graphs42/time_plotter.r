#run 'png(time.png)' from R shell
generations <- c(100,200,300)
time1 <- c(8.08,9.22,9.18)    #no simulated annealing
time2 <- c(5.01,5.18,5.56)    #simulated annealing


plot(generations, time1, main="Running time against number of generations for 800-500 indiv.", sub="",  xlab="generations", ylab="time (in seconds)",
xlim=c(100, 300), ylim=c(0, 10), lty=1)

lines(generations,time1,col="red",lty=1)


points(generations, time2, col="blue", pch="*")
lines(generations, time2, col="blue",lty=2)


legend(101, 3, legend=c("Without S.A.,800 indiv.", "With S.A.,500 indiv."),
       col=c("red", "blue"), lty=1:2, cex=0.8)
#run 'dev.off()' from R shell

