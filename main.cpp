//  Project             :  Challenge
//  Author              :  Patrik Michlo
//  Date of creation    :  2020-03-11
//  Workfile            :  main.cpp
//--------------------------------------------------------------------------
//  Description: Main function for a Challenge project
//--------------------------------------------------------------------------
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>

#include "LockGuard.h"

// Mutex and condition_variable for synchronization of threads
std::mutex mutex;
std::condition_variable conditionVariable;

// Variables for conditon_variable wait() predicates
bool blockThread1 = true;
bool blockThread2 = true;
bool blockThread3 = true;

int main()
{
  std::cout << "Main: Starting all threads." << std::endl;

  std::thread thread1([]() {
    // Random number generator
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(1, 5); // range is from 1 to 5

    while (true)
    {
      std::unique_lock<std::mutex> lock(mutex);

      std::cout << "Thread1: Started waiting." << std::endl;

      conditionVariable.wait(lock, []()
      {
        return blockThread1 == false;
      });

      std::cout << "Thread1: Signal received - doing work." << std::endl;

      int random = dist(mt);
      std::cout << "Thread1: Sleep for " << random << " seconds."  << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(random));

      blockThread1 = true;
      blockThread2 = false;

      std::cout << "Thread1: Done with work - signal." << std::endl;
      conditionVariable.notify_all();
    }
  });

  std::thread thread2([]() {
    while (true)
    {
      std::unique_lock<std::mutex> lock(mutex);

      std::cout << "Thread2: Started waiting." << std::endl;

      conditionVariable.wait(lock, []()
      {
        return blockThread2 == false;
      });

      std::cout << "Thread2: Signal received - doing work." << std::endl;

      blockThread2 = true;
      blockThread3 = false;

      std::cout << "Thread2: Done with work - signal." << std::endl;
      conditionVariable.notify_all();
    }
  });

  std::thread thread3([]() {
    while (true)
    {
      std::unique_lock<std::mutex> lock(mutex);

      std::cout << "Thread3: Started waiting." << std::endl;

      conditionVariable.wait(lock, []()
      {
        return blockThread3 == false;
      });

      std::cout << "Thread3: Signal received - doing work." << std::endl;

      blockThread3 = true;
      blockThread1 = false;

      std::cout << "Thread3: Done with work - signal." << std::endl;
      conditionVariable.notify_all();
    }
  });

  {
    // Protect common resource with custom implementation of lock_guard
    Challenge::lock_guard lock(mutex);
    std::cout << "Main: Starting Thread 1." << std::endl;
    blockThread1 = false;
    conditionVariable.notify_all();
  }
  thread1.join();
  thread2.join();
  thread3.join();
}