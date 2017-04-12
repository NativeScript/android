//
// Created by plamen5kov on 9/10/16.
//

#include "ReadWriteLock.h"

using namespace std;
using namespace tns;

ReadWriteLock::ReadWriteLock()
    : shared(), reader_gate(), writer_gate(), active_readers(0), waiting_writers(0), active_writers(0) {
}

void ReadWriteLock::AquireReadLock() {
    //wait till there are no waiting writers and increment active readers
    std::unique_lock<std::mutex> lk(shared);
    while ( waiting_writers != 0 ) { //starving readers and giving writer priority
        reader_gate.wait(lk);
    }
    active_readers++;
    lk.unlock();
}

void ReadWriteLock::ReleaseReadUnlock() {
    //get lock decrement active readers and notify one waiting writer
    std::unique_lock<std::mutex> lk(shared);
    active_readers--;
    lk.unlock();
    writer_gate.notify_one();
}

void ReadWriteLock::AquireWriteLock() {
    //declare waiting and wait till there are no more active readers or writers and get lock
    std::unique_lock<std::mutex> lk(shared);
    waiting_writers++;
    while ( active_readers != 0 || active_writers != 0 ) { //if any is active wait for them to finish
        writer_gate.wait(lk);
    }
    active_writers++;
    lk.unlock();
}

void ReadWriteLock::ReleaseWriteUnlock() {
    //decrement waiting and active writers
    std::unique_lock<std::mutex> lk(shared);
    waiting_writers--;
    active_writers--;
    if (waiting_writers > 0) {
        writer_gate.notify_one();    //(priority to writers)
    } else {
        reader_gate.notify_all();    //notify readers
    }
    lk.unlock();
}

