#define _GNU_SOURCE // implicit syscall(2) not POSIX (its a test macros)

#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<sys/syscall.h>
#include<sys/stat.h>
#include<fcntl.h>  // for flags
#include<string.h> // for strerror
#include<stdlib.h> // for strtol
// for stat
#include<time.h>
#include<pwd.h>
#include<grp.h>
#include<sys/sysmacros.h> // for major and minor
#include <inttypes.h> // for intmax_t

// deprecated so use _GNU_SOURCE to get sys_errlist
// extern const char *const sys_errlist[];
// extern int sys_nerr;

int show_menu(const char* menu[], int size){
	putchar('\n');
	for (int i = 0; i < size; ++i){
		printf("%d - %s\n", i, menu[i]);
	}
	return 0;
}

// progs 

int prog1(){
	const char filename[] = "file.txt";
	const char text_to_write[] = "sample_text\n";
	const char *error_msg;
	int fd =  open(filename, O_RDWR, 0644);
	if (fd == -1){
		// 2 - ENOENT, 13 - EACCES
		printf("errno: %d\n", errno);
		printf("strerror: %s\n", strerror(errno));
		perror("perror");
		return errno;
	}
	 write(fd, text_to_write, sizeof(text_to_write)-1);
	 close(fd);
	return 0;
}

int prog2(){
	char filename[256];
	char buf[256];
	const char text_to_write1[] = "this is my text\n";
	const char text_to_write2[] = "and no one\ncan take it away\n";
	int fd, mode;
	ssize_t bytes;
	printf("Set filename\n");
	scanf("%s", filename);	
	printf("Set permission\n");
	scanf("%4s", buf); // 0644 - for example
	mode = strtol(buf, NULL, 8);
	fd =  open( filename, O_CREAT | O_WRONLY | O_TRUNC, mode); // you can't open created file without write permission
	if (fd == -1)
		perror("Can't create file");
	bytes =  write(fd, text_to_write1, sizeof(text_to_write1)-1);
	bytes +=  write(fd, text_to_write2, sizeof(text_to_write2)-1);
	printf("%ld bytes were written\n", bytes);
	if (bytes <= 0) perror("Error");
	 close( fd);

	fd =  open( filename, O_RDONLY);
	bytes =  read( fd, buf, 255);
	printf("%ld bytes were read\n", bytes);
	if (bytes <= 0) perror("Error");
	 close( fd);

	fd =  open( filename, O_RDWR);
	if (fd == -1){
		perror("Can't open file");
	}else{
		printf("How do you open it!?\n");
		 close( fd);
	}
	return 0;
}

int prog4(){
	char filename[256];
	char buf[256];
	const char *menu[] = {"Exit", "Write", "Read", "Change seek"};
	const char *seek_menu[] = {"Exit", "SEEK_CUR", "SEEK_SET", "SEEK_END"};
	int seeks[] = {SEEK_CUR, SEEK_SET, SEEK_END};
	int fd, mode, r_count;
	int option, option1;
	ssize_t bytes;
	long pos;
	printf("Set filename\n");
	scanf("%s", filename);	
	printf("Set permission\n");
	scanf("%4s", buf); // 0644
	mode = strtol(buf, NULL, 8);
	fd =  open( filename, O_CREAT | O_RDWR | O_TRUNC, mode);
	if (fd == -1) {
		perror("Can't create file");
		return errno;
	}
	printf("File %s were created", filename);
	option = 1;
	while (option != 0){
		show_menu(menu, sizeof(menu)/sizeof(menu[0]));
		scanf("%d", &option);
		switch(option){
			case 1: // write
				printf("Enter what to write to the file\n");
				getchar();
				scanf("%255[^\n]%n", buf, &r_count);
				getchar();
				bytes =  write( fd, buf, r_count);
				printf("%ld bytes were written\n", bytes);
			break; 
			case 2: // read
				buf[0] = '\0'; // clearing buffer
				bytes =  read( fd, buf, 255);
				printf("%ld bytes were read\n", bytes);
				if (bytes > 0) printf("%.*s\n", (int)bytes, buf); //write only bytes symbols
			break;
			case 3: // change seek
				show_menu(seek_menu, sizeof(seek_menu)/sizeof(seek_menu[0]));
				scanf("%d", &option1);
				if ((option1 > 0) && (option1 < 4)){
					printf("Enter seek:\n");
					scanf("%ld", &pos);
					 lseek( fd, pos, seeks[option1-1]);
					printf("Seek changed to %ld\n",  lseek( fd, 0, SEEK_CUR));
				}
			break;
			default:	
			break;
		}
	}
	printf("File %s is closing...\n", filename);
	 close(fd);
	return 0;
}

int file_stat(){
	char filename[256];
	struct stat st;
	printf("Enter filename:\n");
	scanf("%s", filename);
	 stat( filename, &st);
	printf("File: %-30s\n", filename);
	printf(" File type: \n  ");
    	if (S_ISREG(st.st_mode)) printf("regular file");
    	else if (S_ISDIR(st.st_mode)) printf("directory");
    	else if (S_ISCHR(st.st_mode)) printf("character device");
    	else if (S_ISBLK(st.st_mode)) printf("block device");
    	else if (S_ISFIFO(st.st_mode)) printf("FIFO-channel");
    	else if (S_ISLNK(st.st_mode)) printf("link");
    	else if (S_ISSOCK(st.st_mode)) printf("socket");
    	else printf("unknown type");
    	putchar('\n');
    	printf(" Main info \n");
    	printf(" inode: %-30jd\n", st.st_ino);
    	printf(" links: %-30jd\n", st.st_nlink);
    	printf(" size: %-30jd\n", st.st_size);
    	printf(" blocks: %-30jd\n", st.st_blocks);
    	putchar('\n');
    
    	struct passwd *pw = getpwuid(st.st_uid);
    	struct group *gr = getgrgid(st.st_gid);
    
    	printf(" Owner and group\n");
    	printf(" Owner: %-28s\n", pw ? pw->pw_name : "?");
    	printf(" UID: %-33d\n", st.st_uid);
    	printf(" Group: %-28s\n", gr ? gr->gr_name : "?");
    	printf(" GID: %-33d\n", st.st_gid);
    	putchar('\n');
    
    	printf(" Permissions: %-25o\n", st.st_mode & 0777);
    
    	char time_buf[80];
    	printf("Timestamps:\n");
    
    	struct tm *tm_info = localtime(&st.st_atime);
    	strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);
    	printf(" Access: %-30s\n", time_buf);
    
    	tm_info = localtime(&st.st_mtime);
    	strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);
    	printf(" Mod: %-25s\n", time_buf);
    
    	tm_info = localtime(&st.st_ctime);
    	strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", tm_info);
    	printf(" Change: %-20s\n", time_buf);
    	putchar('\n');
    
    	// Spec info for block and char devices
    	if (S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode)) {
        	printf("Info about device\n");
	        printf(" Major: %-32d\n", major(st.st_rdev));
        	printf(" Minor: %-32d\n", minor(st.st_rdev));
		putchar('\n');
    	}

	return 0;
}

int coping(){
	char buf[256];
	size_t r_bytes, w_bytes;
	
	while ((r_bytes =  read( STDIN_FILENO, buf, sizeof(buf))) > 0) {
        	w_bytes =  write( STDOUT_FILENO, buf, r_bytes);
        	if (w_bytes != r_bytes) {
            		perror("Can't read");
            		return -1;
        	}
    	}
    	
    	if (r_bytes == -1) {
          perror("Can't read");
          return -1;
        }

	return 0;
}

int copy_file(){
	char filename1[256], filename2[256];
	printf("Copy from file:\n");
	scanf("%s", filename1);
	printf("To file:\n");
	scanf("%s", filename2);
	int src = -1, dst = -1;
	int saved_stdin =  dup( STDIN_FILENO), saved_stdout =  dup( STDOUT_FILENO);
	if (filename1[0] != '!'){
	      src =  open( filename1, O_RDONLY);
	      if (src == -1){
		perror("Can't open file");
		 close( saved_stdin);
                 close( saved_stdout);
	        return 1;
	      }
	       close( STDIN_FILENO); // change stdin
	       dup2( src, STDIN_FILENO);
	}
	if (filename2[0] != '!'){
	      dst =  open( filename2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	      if (dst == -1){
		perror("Can't open file");
		if (src != -1) {
                   close( STDIN_FILENO);
                   dup2( saved_stdin, STDIN_FILENO);
                }
                 close( saved_stdin);
                 close( saved_stdout);
	        return 1;
	      }
	       close( STDOUT_FILENO); // change stdout
	       dup2( dst, STDOUT_FILENO);
	}	
	coping();
	 
         close( STDIN_FILENO);
         dup2( saved_stdin, STDIN_FILENO);
         close( STDOUT_FILENO);
         dup2( saved_stdout, STDOUT_FILENO);
    
         close( saved_stdin);
         close( saved_stdout);
        if (src != -1)  close(src);
        if (dst != -1)  close(dst);
	return 0;
}

int read_backwards(){
  char filename[256];
  int fd;
  printf("Enter filename:\n");
  scanf("%s", filename);
  fd =  open( filename, O_RDONLY);
  if (fd == -1){
    perror("Can't read file");
    return 1;
  }
  off_t file_size =  lseek( fd, 0, SEEK_END);
  if (file_size == -1) {
    perror("Can't get file size");
    return 1;
  }
  char ch;
  for (off_t pos = file_size - 1; pos >= 0; pos--) {
        
        if ( lseek( fd, pos, SEEK_SET) == -1) {
            perror("Can't seek in file");
            return 1;
        }
        
        if ( read( fd, &ch, 1) != 1) {
            perror("Can't read char");
            return 1;
        }

        putchar(ch);
    }
  putchar('\n');
  return 0;
}

int biggest_file(){
  char filename[256], max_filename[256];
  intmax_t max_size = 0;
  struct stat st;
  int fd;
  printf("Enter filenames: (to stop enter ctrl+D)\n");
  while (scanf("%s", filename) != -1){
     stat( filename, &st);
    if (st.st_size > max_size){
      max_size = st.st_size;
      strcpy(max_filename, filename);
    }
  }
  printf("Biggest file is %s. Its size is %jd\n", max_filename, max_size);
  return 0;
}


int main(int argc, char *argv[]){
	const char* main_menu[] = {"exit", "prog1", "prog2", "prog4", "stat", "copy_file", "read_backwards", "biggest_file"};
	int option;
	printf("Choose program\n");
	show_menu(main_menu, sizeof(main_menu)/sizeof(main_menu[0]));
	scanf("%d", &option);
	switch(option){
		case 0:
			printf("Exit\n");
		break;
		case 1:
			prog1();
		break;
		case 2:
			prog2();
		break;
		case 3:
			prog4();
		break;
		case 4:
			file_stat();
		break;
		case 5:
			copy_file();
		break;
		case 6:
			read_backwards();
		break;
		case 7:
		        biggest_file();
		break;
		default:
			fprintf(stderr, "option = %d is unavailable!\n", option);
		return -1;
	}
	return 0;
}
