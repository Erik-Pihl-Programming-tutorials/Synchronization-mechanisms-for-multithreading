#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <cstdint>

namespace {

/********************************************************************************
 * @brief Mutex used for synchronizing shared resources between threads.
 ********************************************************************************/
std::mutex mutex{}; 

/********************************************************************************
 * @brief Stores the number of performed prints.
 ********************************************************************************/
uint16_t num_prints{};

/********************************************************************************
 * @brief Blocks the calling thread for specified delay time in milliseconds.
 * 
 * @param delay_time_ms
 *        The time to block the calling thread for, measured in milliseconds.
 ********************************************************************************/
inline void Delay_ms(const uint16_t delay_time_ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(delay_time_ms));
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
 * @param thread_id
 *        Unique identifier of the thread.
 * @param print_interval_ms
 *        The time interval between each print, measured in milliseconds.
 ********************************************************************************/
void RunThread(const uint16_t thread_id, const uint16_t print_interval_ms) {
    while (1) {
        mutex.lock();
        std::cout << "--------------------------------------------------------------------------------\n";
        std::cout << "Running thread with ID " << thread_id << "!\n";
        std::cout << "Number of performed prints: " << ++num_prints << "\n";
        std::cout << "--------------------------------------------------------------------------------\n\n";
        Delay_ms(10);
        mutex.unlock();
        Delay_ms(print_interval_ms);
    }
}
} /* namespace */

/********************************************************************************
 * @brief Creates and runs two threads with different parameters.
 * 
 * @note
 *        When creating threads, specify the starting routine and it's input
 *        arguments. Synchronize the threads by calling the join function.
 ********************************************************************************/
int main(void) {
    std::thread t1{RunThread, 1, 1000};
    std::thread t2{RunThread, 2, 1000};
    t1.join();
    t2.join();
    return 0;
}