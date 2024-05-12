// Copyright 2024 Dostavalov Semyon

#include "TimedDoor.h"

DoorTimerAdapter::DoorTimerAdapter(TimedDoor& door) : door(door) {}

void DoorTimerAdapter::Timeout() {
    door.throwState();
}

TimedDoor::TimedDoor(int timeout) : iTimeout(timeout), isOpened(false),
adapter(nullptr) {}

bool TimedDoor::isDoorOpened() {
    return isOpened;
}

void TimedDoor::unlock() {
    if (isDoorOpened()) {
        throw std::logic_error("Door opened");
    }
    isOpened = true;
    adapter = new DoorTimerAdapter(*this);
    Timer timer;
    timer.tregister(iTimeout, adapter);
}

void TimedDoor::lock() {
    if (!isDoorOpened()) {
        throw std::logic_error("Door closed");
    }
    isOpened = false;
    delete adapter;
}

int TimedDoor::getTimeOut() const {
    return iTimeout;
}

void TimedDoor::throwState() {
    if (isDoorOpened()) {
        throw std::runtime_error("Door opened long");
    }
}

void Timer::tregister(int timeout, TimerClient* client) {
    this->client = client;
    sleep(timeout);
}

void Timer::sleep(int timeout) {
    std::this_thread::sleep_for(std::chrono::seconds(timeout));
}
