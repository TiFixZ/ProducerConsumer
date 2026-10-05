#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

std::queue<int> tasks;
std::mutex mtx;
std::condition_variable cv;
bool finished = false;

std::mutex output_mtx;
int processed = 0;

void producer(int start, int count)
{
    for (int i = 0; i < count; ++i)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            tasks.push(start + i);
        }
        cv.notify_one();
    }
}

void consumer(int id)
{
    while (true)
    {
        int task;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [] { return finished || !tasks.empty(); });

            if (tasks.empty())
            {
                return;
            }

            task = tasks.front();
            tasks.pop();
        }

        int result = task * task;
        {
            std::lock_guard<std::mutex> lock(output_mtx);
            std::cout << "Consumer " << id << ": " << task << " -> " << result << '\n';
            ++processed;
        }
    }
}

int main()
{
    std::thread consumer1(consumer, 1);
    std::thread consumer2(consumer, 2);
    std::thread producer1(producer, 0, 10);
    std::thread producer2(producer, 10, 10);

    producer1.join();
    producer2.join();

    {
        std::lock_guard<std::mutex> lock(mtx);
        finished = true;
    }
    cv.notify_all();

    consumer1.join();
    consumer2.join();

    std::cout << "Processed: " << processed << '\n';
    return 0;
}
