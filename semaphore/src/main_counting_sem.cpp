/********************************************************************************
 * @brief Demonstration of counting semaphores in C++.
 ********************************************************************************/
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdint>
#include <semaphore.h>

namespace {

/********************************************************************************
 * @brief Counting semaphores for synchronized usage of the terminal and
 *        shared memory. The max count of the semaphores are set to 1, which
 *        makes them function as binary semaphores in this case, but we can
 *        set the max count to everyting between [1, 65 535].
 ********************************************************************************/
counting_semaphore<1> sem_console{};
counting_semaphore<1> sem_shared_mem{};

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
 *        specified frequency. Semaphores are used to synchronize the usage
 *        of the shared resources.
 * 
 * @param thread_id
 *        Unique identifier of the thread.
 * @param print_interval_ms
 *        The time interval between each print, measured in milliseconds.
 ********************************************************************************/
void RunThread(const uint16_t thread_id, const uint16_t print_interval_ms) {
    const auto adjusted_print_interval_ms{static_cast<uint16_t>(print_interval_ms > 10 ? print_interval_ms - 10 : 10)};
    while (1) {
        sem_shared_mem.take();
        const auto num_prints_copy{++num_prints};
        sem_shared_mem.release();

        sem_console.take();
        std::cout << "--------------------------------------------------------------------------------\n";
        std::cout << "Running thread with ID " << thread_id << "!\n";
        std::cout << "Number of performed prints: " << num_prints_copy << "\n";
        std::cout << "--------------------------------------------------------------------------------\n\n";
        Delay_ms(10);
        sem_console.release();
        Delay_ms(adjusted_print_interval_ms);
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