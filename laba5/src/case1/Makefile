CC = gcc
EXEC = run

$(EXEC): main.o ../libqueue.so
	$(CC) -o $@ $< -I../ -L../ -lqueue -Wl,-rpath,../

main.o: main.c
	$(CC) -c main.c -I../

../libqueue.so: ../q.o
	$(CC) -shared -o ../libqueue.so ../q.o

../q.o: ../q.c
	$(CC) -fPIC -c ../q.c -o $@

clean:
	rm -f *.o ../*.o ../*.so $(EXEC)