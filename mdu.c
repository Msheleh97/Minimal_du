#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "threads_free.h"
#include "threads.h"


int main(int argc, char *argv[]) {
  int opt , j = 0;
  //char* quantity_of_threads;
  while ((opt = getopt(argc,argv,"j:%s")) != -1){
    switch (opt){
      case 'j':
        j++;
        //quantity_of_threads = optarg;
        break;
      case '?':
        fprintf(stderr, "Unknown flag -%c \n",optopt);
        return EXIT_FAILURE;
    }
  }
  if(optind == 2 && j == 1){
    char* amount_threads = argv[1];
    return count_dirsize_with_threads(amount_threads[2]-'0' ,argv, argc);
  }
  return count__dirsize_without_threads(argc,argv);
}


