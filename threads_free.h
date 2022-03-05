/**
 * (Detailed description of the module.)
 *
 * This This module is responsible of calculating the given directory size without threads and recursively. 
 *
 * @author  Mohammad Mshaleh
 * @since   2021-11-15
 *
 * @{
 */
#ifndef THREADS_FREE_H
#define HREADS_FREE_H
#define BITS 1024
#define LINK_SIZE 4096
#define BLOCK 512

/**
 * @name  count_dirsize_without_threads.
 * @brief The function is the main function of the calculation of directories size without using multi threads.
 *
 * @param  argc The quantity of directories to calculate.
 * @param  argv The path to the directories.

 * @return 0 if the operation executed without any error, 1 if not.
 */
int count__dirsize_without_threads(int argc,char *argv[]);

#endif
