/*

   sample01.c : Minimal OpenGL program 
   9 Sep 2010 by Minho Kim (minhokim@uos.ac.kr)

 */

#include <stdio.h>

void foo(int *a, int b, int *c){ 
	a = &b;b = -10; *c = 100;
	printf("a : %d, b : %d, c : %d\n", *a, b, *c);} 

int main(int argc, char* argv[]) {	int array[5], i, *ip; 
	for(i=0;i<5;i++)		array[i] =(i+1)*10; 
	ip = &array[0];	printf("%d\n",*(ip+1));
	foo(ip, *ip+1, (ip+2));
		printf("%d, %d, %d\n", *ip, *(ip+1), *(ip+2));	printf("%d, %d, %d\n", array[0], array[1], array[2]);		return 0;
}
