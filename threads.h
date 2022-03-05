/**
 * (Detailed description of the module.)
 *
 * This This module is responsible of calculating the given directory size with threads. 
 *
 * @author  Mohammad Mshaleh
 * @since   2021-11-15
 *
 * @{
 */
#ifndef THREADS_H
#define HREADS_H

/**
 * @name                Count_dirsize_with_threads.
 * @brief               This function is responsible for traversing and counting the total size
 *                      of the given directories with multi-threadings.
 * 
 * @param threads       The amount threads given by the user.
 * @param argv          A pointer to the name of the directories given by the user.
 * @param amount_dir    The amount of the given directories.
 * 
 * @return              0 if the operation executed without any error, 1 if not.
 */
int count_dirsize_with_threads(int threads, char *argv[], int amount_dir);

#endif
