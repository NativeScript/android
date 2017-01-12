//
// Created by plamen5kov on 9/10/16.
//

#ifndef READWRITELOCK_H
#define READWRITELOCK_H

#include <iostream>
#include <mutex>
#include <condition_variable>

namespace tns {

class ReadWriteLock {
    public:
        ReadWriteLock();

        void AquireReadLock();
        void ReleaseReadUnlock();
        void AquireWriteLock();
        void ReleaseWriteUnlock();

    private:
        std::mutex shared;
        std::condition_variable reader_gate;
        std::condition_variable writer_gate;
        int active_readers;
        int active_writers;
        int waiting_writers;
};
}

#endif //READWRITELOCK_H
