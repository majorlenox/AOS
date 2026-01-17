#include<stdio.h> // scanf printf
#include<stdlib.h> // putinv

int main(){
	char pi_string[] = "PI_CONST=3.14159265358979323646264338327950"; // no const

	printf("PI_CONST=%s\n", getenv("PI_CONST")); 
	putenv(pi_string); // ENOMEM
	printf("PI_CONST=%s\n", getenv("PI_CONST"));

	return 0;
}
