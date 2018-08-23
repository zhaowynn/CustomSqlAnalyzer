#include <mutex>
#include <condition_variable>

#ifndef _RW_LOCK_H_
#define _RW_LOCK_H_

class RWLock {
public:
    RWLock();
    void LockRead();
    void UnlockRead();
    void LockWrite();
    void UnlockWrite();
private:
    std::mutex mutex_;
    std::condition_variable reading_allowed_;
    std::condition_variable writing_allowed_;
    volatile bool write_locked_;
    volatile size_t read_locked_;
};

class ReadLock final {
public:
    ReadLock(RWLock& lock);
    ~ReadLock();
    ReadLock(const ReadLock&);
    ReadLock& operator=(const ReadLock&);
private:
    RWLock& lock_;
};

class WriteLock final {
public:
    WriteLock(RWLock& lock);
    ~WriteLock();
private:
    WriteLock(const WriteLock&);
    WriteLock& operator=(const WriteLock&);
    RWLock& lock_;
};

#endif /* _RW_LOCK_H_ */
