CC = gcc
EXEC = run

$(EXEC): main.o ../libq.so
	$(CC) -o $@ $< -rdynamic -ldl -I../
	# $(CC) -o $@ $< -rdynamic -ldl -I../ -L../ -lq -Wl,-rpath,../

main.o: main.c
	$(CC) -c -I../ main.c

../libq.so: ../q.o
	$(CC) -shared -o ../libq.so ../q.o

../q.o: ../q.c
	$(CC) -c ../q.c
	# $(CC) -c -fPIC q.c

clean:
	rm -f *.o *.so $(EXEC)