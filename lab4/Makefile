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

task4: task4.c task4_c.c
	$(CC) $(CFLAGS) -o task4 task4.c
	$(CC) $(CFLAGS) -o task4_c task4_c.c

task5: task5.c
	$(CC) $(CFLAGS) -o task5 task5.c

task6: task6.c
	$(CC) $(CFLAGS) -o task6 task6.c

task7: task7.c task7_writer.c task7_reader.c
	$(CC) $(CFLAGS) -o task7 task7.c
	$(CC) $(CFLAGS) -o task7_reader task7_reader.c
	$(CC) $(CFLAGS) -o task7_writer task7_writer.c

task8: task8.c task8_writer.c task8_reader.c
	$(CC) $(CFLAGS) -o task8 task8.c
	$(CC) $(CFLAGS) -o task8_reader task8_reader.c
	$(CC) $(CFLAGS) -o task8_writer task8_writer.c

task9: task9.c task9_writer.c task9_reader.c
	$(CC) $(CFLAGS) -o task9 task9.c
	$(CC) $(CFLAGS) -o task9_reader task9_reader.c
	$(CC) $(CFLAGS) -o task9_writer task9_writer.c

task10: task10.c task10_a.c
	$(CC) $(CFLAGS) -o task10 task10.c
	$(CC) $(CFLAGS) -o task10_a task10_a.c

task11: task11.c task11_a.c
	$(CC) $(CFLAGS) -o task11 task11.c
	$(CC) $(CFLAGS) -o task11_a task11_a.c

clean:
	$(RM) task1 task2 task3 task4 task5 task6 task7 task8 task9 task10 task11 task4_c fifo task7_reader task7_writer task8_reader task8_writer task9_reader task9_writer file.txt task10_a task11_a

.PHONY: clean
