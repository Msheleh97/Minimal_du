#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "threads.h"
#include "threads_free.h"

int exit_code = 0;

//Static functions declaration
static void set_directory(const char *directory_name);
static int count_directory_size(char *dirname, int size);

int count__dirsize_without_threads(int argc,char *argv[]){
  struct stat buff;
  char cwd[PATH_MAX];
  getcwd(cwd, sizeof(cwd));

  for (int i = 1; i < argc; i++) {
    if(lstat(argv[i],&buff) != 0){
      fprintf(stderr, "EROR: file %s don't exist\n", argv[i]);
      exit(1);
    }
    if(S_ISDIR(buff.st_mode)){
      printf("%d      %s\n",count_directory_size(argv[i],buff.st_nlink*(LINK_SIZE/BLOCK)), argv[i]);
      set_directory(cwd);
    }
    else if(S_ISREG(buff.st_mode)){
       printf("%ld      %s\n",buff.st_blocks,argv[i]);
    }
  }
  return exit_code;
}


//####################################### Static functions ####################################################//

/**
 * @name  count_directory_size.
 * @brief The function is used to traverse a directory recursively and find its size. 
 *
 * @param  dirname the directory name.
 * @param  size    the size of the current file.

 * @return the total size of the main directory.
 */
static int count_directory_size(char *dirname, int size){
  DIR *dp;
  struct dirent *di_str;
  struct stat stat_struct;
  if ((dp = opendir(dirname)) == NULL){
    fprintf(stderr, "EROR: Can't open the file:  %s \n",dirname); 
  }
  set_directory(dirname); 
  while ((di_str = readdir(dp)) != NULL){
   if (lstat(di_str->d_name, &stat_struct) != 0) { 
        perror(dirname);
        exit(1);
      }
      else if (S_ISDIR(stat_struct.st_mode)){
        if (S_ISDIR(stat_struct.st_mode) && strcmp(di_str->d_name, ".") != 0 && strcmp(di_str->d_name, "..") != 0 && stat_struct.st_mode & S_IRUSR){
          size+=count_directory_size(di_str->d_name,size);
          set_directory("..");
        }else if(!(stat_struct.st_mode & S_IRUSR)){
          size+= LINK_SIZE/BLOCK;
          fprintf(stderr,"permisson denied, can not open file: '%s/%s'\n",dirname,di_str->d_name);
          exit_code = 1;
        }
      }
      else if(S_ISREG(stat_struct.st_mode)){
        size+= stat_struct.st_blocks;
      }
  }
  closedir(dp);
  return size;
}

/**
 * @name  set_directory.
 * @brief The function uses to change the current directory.
 * 
 * @param  directory_name The directory path.
 * @return Nothing.
 */
static void set_directory(const char *directory_name){
  if (chdir(directory_name) < 0) {
    perror(directory_name);
    exit(1);
  }
}


