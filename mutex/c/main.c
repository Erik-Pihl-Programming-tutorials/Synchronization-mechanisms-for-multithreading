#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>

/********************************************************************************
 * @brief Structure containing thread arguments.
 * 
 * @param id
 *        Unique identifier of the thread.
 * @param print_interval_ms
 *        The time interval between each print, measured in milliseconds.
 ********************************************************************************/
struct thread_args {
    const uint16_t id;
    const uint16_t print_interval_ms;
};

/********************************************************************************
 * @brief Mutex used for synchronizing shared resources between threads.
 ********************************************************************************/
static pthread_mutex_t mutex;

/********************************************************************************
 * @brief Stores the number of performed prints.
 ********************************************************************************/
static uint16_t num_prints = 0;

/********************************************************************************
 * @brief Blocks the calling thread for specified delay time in milliseconds.
 * 
 * @param delay_time_ms
 *        The time to block the calling thread for, measured in milliseconds.
 ********************************************************************************/
static inline void delay_ms(const uint16_t delay_time_ms) {
    usleep(delay_time_ms * 1000);
}

/********************************************************************************
 * @brief Runs the calling thread continuously by printing the thread ID with
 *        specified frequency. 
 * 
 * @note  
 *        A mutex is used to make sure that only one thread has access to the
 *        terminal at any given time. We wait 10 ms after performing a print 
 *        before unlocking the mutex to make sure that the print is completed 
 *        before making the terminal accessible to other threads.
 * 
 * @param args
 *        Thread-specific arguments passed as a reference to a thread_args
 *        structure. 
 * @return 
 *        A nullptr, since thread functions in C need to return some kind of
 *        pointer.
 ********************************************************************************/
static void* run_thread(void* args) {
    struct thread_args* self = (struct thread_args*)(args);
    while (1) {
        pthread_mutex_lock(&mutex);
        printf("--------------------------------------------------------------------------------\n");
        printf("Running thread with ID %hu!\n", self->id);
        printf("Number of performed prints: %hu\n", ++num_prints);
        printf("--------------------------------------------------------------------------------\n\n");
        delay_ms(10);
        pthread_mutex_unlock(&mutex);
        delay_ms(self->print_interval_ms);
    }
    return 0;
}

/********************************************************************************
 * @brief Creates and runs two threads with different parameters.
 * 
 * @note
 *        When creating threads, specify the starting routine and it's input
 *        arguments. Synchronize the threads by calling the join function.
 ********************************************************************************/
int main(void) {
    struct thread_args args1 = {1, 1000}, args2 = {2, 1000};
    pthread_t t1, t2;

    pthread_mutex_init(&mutex, 0);
    pthread_create(&t1, 0, run_thread, &args1);
    pthread_create(&t2, 0, run_thread, &args2);

    pthread_join(t1, 0);
    pthread_join(t2, 0);
    pthread_mutex_destroy(&mutex);
    return 0;
}