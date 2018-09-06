all:
	gcc -c list.c 
	gcc -o serverA serverA.c list.o 
	gcc -o serverB serverB.c list.o
	gcc -o serverC serverC.c list.o
	gcc -o aws aws.c
	gcc -o client client.c
	gcc -o monitor monitor.c

clean:
	rm -f *.o serverA serverB serverC aws client monitor

.PHONY: serverA
serverA:
	./serverA

.PHONY: serverB
serverB:
	./serverB

.PHONY: serverC
serverC:
	./serverC

.PHONY: monitor
monitor:
	./monitor

.PHONY: aws
aws:
	./aws


