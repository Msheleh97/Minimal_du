#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "linkedList.h"
#include "threads.h"

/********** Static functions declaration ***********/
static void count_dir_size(char* path);
static void* get_directory(void* list);
static void addSize(int size, int exitCode);
static char* get_full_dir(char* path , char* name);

/*********** Static Global Variabels **************/
static List* directorys_list;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t wait = PTHREAD_COND_INITIALIZER; 
static int total_dir_size = 0, active_threads = 0, waiting_threads = 0, exit_code = 0;




int count_dirsize_with_threads(int threads, char *argv[], int amount_dir){
  for(int i = 2; i < amount_dir; i++){
    if((directorys_list = list_create()) == NULL){
      fprintf(stderr,"Could not creat directorys list");
    }
    char* path = malloc((strlen(argv[2])+1)*sizeof(char));
    strcpy(path,argv[i]);
    list_insert(directorys_list,path);
    pthread_t pthread_id[threads];

    for (int i = 0; i < threads; i++){
      if (pthread_create(&pthread_id[i],NULL,get_directory,directorys_list) != 0){
        perror("Threds: ");
        exit(1);
      } 
    }
    for (int i = 0; i < threads; i++){
      pthread_join(pthread_id[i],NULL);
    } 
    printf("%d         %s\n",total_dir_size,argv[i]);
    free(directorys_list);
    total_dir_size = 0;
  }
  return exit_code;
}


/***************** Static functions *******************/

/**
 * Name : get_directory.
 * @brief This function uses by the threads to get a directory from the directorys list and get the size of it.
 *        The main responsable of this function is to synchronize the threads.
 * @return Nothing.
 */
static void *get_directory(void* unused){
  (void)unused;
  char* dir_path;
  pthread_mutex_lock(&lock);

  while(!list_is_empty(directorys_list) || active_threads-1 > 0){
    if (!list_is_empty(directorys_list)){
      dir_path = malloc((strlen(list_inspect_first(directorys_list))+1)*sizeof(char));
      strcpy(dir_path,list_inspect_first(directorys_list));
      list_remove(directorys_list);
    }else{
      active_threads--;
      waiting_threads++;
      pthread_cond_wait(&wait,&lock);
    }
    pthread_mutex_unlock(&lock);
    if(dir_path != NULL){
      count_dir_size(dir_path);
      free(dir_path);
      dir_path = NULL;
    }
    pthread_mutex_lock(&lock);
  }
  if (waiting_threads > 0){
    pthread_cond_broadcast(&wait);
  }
  pthread_mutex_unlock(&lock);
  return NULL;
}

/**
 * Name : count_dir_size.
 * @brief The function uses to travers a directory when it find a directory it added the founded 
 *        directory to the list, and if a file get founded the size of it gets adde to the total size.
 *
 * @param  path the directory path.
 * @return Nothing.
 */
static void count_dir_size(char* path){ 
  DIR* dir;
  struct dirent *ent;
  struct stat states;
  char* full_path;
  if (lstat(path, &states) != 0){
    perror(path);
    exit(1);
  }
  if (access(path, X_OK) != 0 && access(path,R_OK) != 0){
    fprintf(stderr,"du: cannot read directory '%s': Permission denied\n", path);
    addSize(states.st_blocks,1);
    return;
  }
  if((dir = opendir(path)) == NULL){
    addSize(states.st_blocks,1);
    fprintf(stderr,"du: cannot read directory '%s': Permission denied\n",path);
    return;
  }
  addSize(states.st_blocks,0);
  while((ent=readdir(dir)) != NULL){
    full_path = get_full_dir(path,ent->d_name);
    if (lstat(full_path, &states) != 0){
      perror(full_path);
      exit(1);
    }
    if(S_ISDIR(states.st_mode)){
      if(strcmp(".", ent->d_name) != 0  && strcmp("..", ent->d_name) != 0){
        pthread_mutex_lock(&lock);
        list_insert(directorys_list,full_path);
        pthread_mutex_unlock(&lock);
      }else{
        free(full_path);
      }
    }else{
      addSize(states.st_blocks,0);
      free(full_path);
    }
  }
  closedir(dir);
}

/**
 * Name : get_full_dir.
 * @brief This function uses to concatenates a path and a file name into a new path.
 *
 * @param  path the original path.
 * @param  name the name of the file to concatenate with the original path.
 * @return The new path.
 */
static char* get_full_dir(char* path , char* name){
  size_t len1 = strlen(path);
  size_t len2 = strlen("/");
  size_t len3 = strlen(name);
  char* full = malloc((len1+len2+len3+1)*sizeof(char));
  strcpy(full,path);
  strcat(full,"/");
  strcat(full,name);
  return full;
}

/**
 * Name : addSize.
 * @brief This function uses to add a size to the totaly size in a thread safe way, and it change the exitCode value if 
 *        somthing wrong appear.
 *
 * @param  size       The size to be added.
 * @param  exitCode   The exit code to change.
 * @return Nothing.
 */
static void addSize(int size, int exitCode){
  pthread_mutex_lock(&lock);
  total_dir_size+=size;
  if (exitCode == 1){
    exit_code = 1;
  }
  pthread_mutex_unlock(&lock);
}
