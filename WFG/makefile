OBJ = read.o avl.o 
OPT = -O3 -DSIMPLE
CC = gcc -std=c99 -Wall $(OPT)

compile: $(OBJ) wfg.c
	$(CC) -Dopt=0 -o wfg0 wfg.c $(OBJ)
	$(CC) -Dopt=1 -o wfg1 wfg.c $(OBJ)
	$(CC) -Dopt=2 -o wfg2 wfg.c $(OBJ)
	$(CC) -Dopt=3 -o wfg3 wfg.c $(OBJ)

%.o: %.c
	$(CC) -c $<

clean: 
	rm -f wfg[0-4] exc[0-4] *.o 
	rm -rf *.dSYM