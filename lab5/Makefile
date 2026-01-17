CC      = gcc
CFLAGS  = -g
RM      = rm -f

all: task1 task2 task3 task4 task5 task6 task7 task8 task9 task10 task11

task1: task1.c
	$(CC) $(CFLAGS) -o task1 task1.c

task2: task2.c
	$(CC) $(CFLAGS) -o task2 task2.c

task3: task3.c
	$(CC) $(CFLAGS) -o task3 task3.c

task4: task4.c
	$(CC) $(CFLAGS) -o task4 task4.c

task5: task5.c
	$(CC) $(CFLAGS) -o task5 task5.c

task6: task6_server.c task6_client.c
	$(CC) $(CFLAGS) -o task6_server task6_server.c
	$(CC) $(CFLAGS) -o task6_client task6_client.c

task7: task7_server.c task7_client.c
	$(CC) $(CFLAGS) -o task7_server task7_server.c
	$(CC) $(CFLAGS) -o task7_client task7_client.c

task9: task9_server.c task9_client.c
	$(CC) $(CFLAGS) -o task9_server task9_server.c
	$(CC) $(CFLAGS) -o task9_client task9_client.c

task10: task10_server.c task10_client.c
	$(CC) $(CFLAGS) -o task10_server task10_server.c
	$(CC) $(CFLAGS) -o task10_client task10_client.c

task11: task11.c
	$(CC) $(CFLAGS) -o task11 task11.c

clean:
	$(RM) task1 task2 task3 task4 task5 task6_server task6_client task7 task8 task10_server task10_client task11 task7_server task7_client task9_server task9_client

.PHONY: clean
