//  Project             :  Challenge
//  Author              :  Patrik Michlo
//  Date of creation    :  2020-03-11
//  Workfile            :  LockGuard.h
//--------------------------------------------------------------------------
//  Description: Implementation of mutex wrapper for owning the mutex for the duration of a scoped block
//--------------------------------------------------------------------------
#pragma once

#include <mutex>

namespace Challenge
{

class lock_guard
{
public:
  lock_guard(std::mutex& mutex) : _mutex(mutex) { _mutex.lock(); }
  ~lock_guard() { _mutex.unlock(); }

  // Make copy constructor and assignment operator inaccessible
  lock_guard(const lock_guard&) = delete;
  lock_guard& operator=(const lock_guard&) = delete;

private:
  std::mutex& _mutex;
};

} // namespace