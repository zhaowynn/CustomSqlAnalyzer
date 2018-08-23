#include "rwlock.h"

RWLock::RWLock() : write_locked_(false), read_locked_(0) {
}

void RWLock::LockRead() {
    std::unique_lock < std::mutex > monitor(mutex_);
    while (write_locked_) {
        reading_allowed_.wait(monitor);
    }
    read_locked_++;
}

void RWLock::UnlockRead() {
    std::unique_lock < std::mutex > monitor(mutex_);
    read_locked_--;
    if (read_locked_ == 0) {
        writing_allowed_.notify_all();
    }
}

void RWLock::LockWrite() {
    std::unique_lock < std::mutex > monitor(mutex_);
    while (write_locked_ && read_locked_ != 0) {
        writing_allowed_.wait(monitor);
    }
    write_locked_ = true;
}

void RWLock::UnlockWrite() {
    std::unique_lock < std::mutex > monitor(mutex_);
    write_locked_ = false;
    reading_allowed_.notify_all();
}

ReadLock::ReadLock(RWLock& lock): lock_(lock) {
    lock_.LockRead();
}

ReadLock::~ReadLock() {
    lock_.UnlockRead();
}

WriteLock::WriteLock(RWLock& lock): lock_(lock) {
    lock_.LockWrite();
}

WriteLock::~WriteLock() {
    lock_.UnlockWrite();
}

