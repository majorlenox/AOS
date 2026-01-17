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

task6: task6.c
	$(CC) $(CFLAGS) -o task6 task6.c

task7: task7.c
	$(CC) $(CFLAGS) -o task7 task7.c

task8: task8.c
	$(CC) $(CFLAGS) -o task8 task8.c

task9: task9.c
	$(CC) $(CFLAGS) -o task9 task9.c

task10: task10.c
	$(CC) $(CFLAGS) -o task10 task10.c
	$(CC) $(CFLAGS) -o task10_child task10_child.c

task11: task11.c
	$(CC) $(CFLAGS) -o task11 task11.c

clean:
	$(RM) task1 task2 task3 task4 task5 task6 task7 task8 task9 task10 task11 parents_file.txt child_file.txt task10_child

.PHONY: clean
