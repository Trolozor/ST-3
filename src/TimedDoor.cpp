// Copyright 2024 Dostavalov Semyon

#include "TimedDoor.h"

DoorTimerAdapter::DoorTimerAdapter(TimedDoor& door) : door(door) {}

void DoorTimerAdapter::Timeout() {
    if (door.isDoorOpened()) {
        throw std::runtime_error("Door is opened too long");
    }
}

TimedDoor::TimedDoor(int timeout) : isOpened(false), iTimeout(timeout) {
    adapter = new DoorTimerAdapter(*this);
}

bool TimedDoor::isDoorOpened() {
    return isOpened;
}

void TimedDoor::unlock() {
    if (isOpened) {
        throw std::logic_error("Door already opened");
    }
    isOpened = true;
}

void TimedDoor::lock() {
    if (!isOpened) {
        throw std::logic_error("Door already closed");
    }
    isOpened = false;
}

void TimedDoor::throwState() {
    adapter->Timeout();
}

void Timer::sleep(int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

int TimedDoor::getTimeOut() const {
    return iTimeout;
}

void Timer::tregister(int timeout, TimerClient* client) {
    sleep(timeout);
    client->Timeout();
}
