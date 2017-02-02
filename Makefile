default:
	gcc -pthread -o lab2b SortedList.c lab2b.c
clean:
	$(RM) lab2b *.o *~
dist:
	tar -cvzf lab2b-404428077.tar.gz lab2b.c SortedList.c SortedList.h Makefile README graph1-costperoperation.png graph2-Synchronizations.png

