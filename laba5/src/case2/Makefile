CC = gcc
EXEC = run

$(EXEC): main.o ../libq.so
	$(CC) -o $@ $< -rdynamic -ldl -I../


main.o: main.c
	$(CC) -c -I../ main.c

../libq.so: ../q.o
	$(CC) -shared -o ../libq.so ../q.o

../q.o: ../q.c
	$(CC) -c ../q.c

clean:
	rm -f *.o *.so $(EXEC)