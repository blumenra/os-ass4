#include "types.h"
#include "stat.h"
#include "user.h"
#include "param.h"
#include "fcntl.h"
#include <math.h>

#define BSIZE      512
#define NB_1MB     2049

void runTests();
void init_buf(char init_char, char* buf, int buf_size);

void test1(void){

  int testNum = 1;
  printf(2, "TEST %d:\n", testNum);


  int buf_size = BSIZE*2100;
  char* buf = (char*) malloc(sizeof(char)*buf_size);
  init_buf('a', buf, buf_size);

  int fd;
  fd = open("alon", O_CREATE|O_RDWR);
  if(write(fd, buf, BSIZE*12) != BSIZE*12) {
    printf(1, "cat: write error\n");
    exit();
  }
  printf(1, "Finished writing 6KB (direct)\n");

  if(write(fd, buf, BSIZE*128) != BSIZE*128) {
    printf(1, "cat: write error\n");
    exit();
  }
  printf(1, "Finished writing 70KB (single indirect)\n");


  if(write(fd, buf, BSIZE*1909) != BSIZE*1909) {
    printf(1, "cat: write error\n");
    exit();
  }
  printf(1, "Finished writing 1MB\n");


  free(buf);

  // printf(2, "TEST %d PASSED!\n\n", testNum);
}

void init_buf(char init_char, char* buf, int buf_size){

  for(int i=0; i < buf_size; i++)
    buf[i] = init_char;
}


void TEST(void (*test)(void)){
  if(fork() == 0){
    test();
    exit();
  }

  wait();
}

void runTestsNTimes(int n){

  for(int i=0; i < n; i++){
    printf(1, "***ITERATION %d***\n", i+1);
    runTests();
  }
}

void runTests(){
  
  if(fork() == 0){

    TEST(test1);
    // TEST(test2);
    // TEST(test3);
    // TEST(test4);

    exit();
  }

  wait();
}

int
main(int argc, char *argv[]){
  printf(1, "Starting sanityTest Tests:\n");
  
  runTests();

  exit();
}



