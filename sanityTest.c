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

/*
* Tests symlink
*/
void test2(void){

  int testNum = 2;
  printf(2, "TEST %d:\n", testNum);

  char *oldpath1 = "c";
  char *newpath1 = "a/b.txt";
  if(symlink(oldpath1, newpath1) == -1)
    printf(2, "PASSED!\n");
  else
    printf(2, "FAILED!\n");
   


  char *oldpath3 = "c";
  char *newpath3 = "a.txt";
  int ret;
  if((ret = symlink(oldpath3, newpath3)) == -1)
    printf(2, "FAILED!\n");
  else if(ret == 0)
    printf(2, "PASSED!\n");
  else
    printf(2, "FAILED!\n");



  printf(1, "\n");
  // printf(2, "TEST %d PASSED!\n\n", testNum);
}

/*
* Tests readlink
*/
void test3(void){

  int testNum = 3;
  printf(2, "TEST %d:\n", testNum);


  /*
  * tests pathname which does not exists
  */
  char *pathname1 = "bla.txt";
  size_t bufsize1 = 20;
  char buf1[bufsize1];
  int ret;

  if((ret = readlink(pathname1, buf1, bufsize1)) == -1)
      printf(2, "PASSED!\n");
  else
    printf(2, "FAILED!\n");

  /*
  * tests pathname which is NOT symbolic link
  */
  char *pathname2 = "cat";
  size_t bufsize2 = 20;
  char buf2[bufsize2];
  if((ret = readlink(pathname2, buf2, bufsize2)) == -1)
      printf(2, "PASSED!\n");
  else
    printf(2, "FAILED!\n");


  /*
  * tests pathname which is NOT symbolic link
  */
  char *pathname3 = "alon.txt";
  size_t bufsize3 = strlen(pathname3)-2;
  char buf3[bufsize3];
  int fd;
  fd = open(pathname3, O_CREATE|O_RDWR);

  if((ret = readlink(pathname3, buf3, bufsize3)) == -1)
      printf(2, "PASSED!\n");
  else
    printf(2, "FAILED!\n");

  close(fd);


  char *pathname4 = "eran.txt";
  char *pathnameTMP = "tmp.txt";
  char* content = "hello world!";
  size_t bufsize4 = strlen(pathnameTMP)+1;
  char buf4[bufsize4];
  fd = open(pathnameTMP, O_CREATE|O_RDWR);
  if(write(fd, content, strlen(content)) != strlen(content)) {
    printf(2, "test3: write error\n");
  }

  if((ret = symlink(pathnameTMP, pathname4)) != 0)
    printf(2, "FAILED1!\n");

  if((ret = readlink(pathname4, buf4, bufsize4)) == -1)
    printf(2, "FAILED2!\n");
  else if(ret == 0){
    if(strcmp(buf4, pathnameTMP) == 0)
      printf(2, "PASSED!\n");
    else
      printf(2, "FAILED3!\n");
  }
  else
    printf(2, "FAILED4!\n");

  close(fd);


  printf(1, "\n");
  printf(2, "TEST %d PASSED!\n\n", testNum);
}

void init_buf(char init_char, char* buf, int buf_size){

  for(int i=0; i < buf_size; i++)
    buf[i] = init_char;
}

void test4(void){

  int testNum = 4;
  printf(2, "TEST %d:\n", testNum);

  char *oldpath1 = "yosi.txt";
  char* content = "Hello World!";
  int fd;

  fd = open(oldpath1, O_CREATE|O_RDWR);

  if(write(fd, content, strlen(content)) != strlen(content)) {
    printf(2, "test4: write error\n");
  }

  char *newpath1 = "ben.txt";
  if(symlink(oldpath1, newpath1) == -1)
    printf(2, "FAILED!\n");
  else
    printf(2, "PASSED!\n");


  printf(1, "\n");
  // printf(2, "TEST %d PASSED!\n\n", testNum);
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

    // TEST(test1);
    TEST(test2);
    // TEST(test3);
    TEST(test4);

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



