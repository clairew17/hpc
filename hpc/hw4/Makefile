all:
	icpc trap_mutex.c -o trap_mutex -lpthread
	icpc trap_busywait.c -o trap_busywait -lpthread
mac:
	gcc trap_mutex.c -o trap_mutex -lpthread -std=gnu99 -g
	gcc trap_busywait.c -o trap_busywait -lpthread -std=gnu99 -g

clean:
	rm -f trap
