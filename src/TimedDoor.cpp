// Copyright 2024 Dostavalov Semyon

#include "TimedDoor.h"

DoorTimerAdapter::DoorTimerAdapter(TimedDoor& door_) : door(door_) {}

void DoorTimerAdapter::Timeout() {
    door.throwState();
}

TimedDoor::TimedDoor(int timeout) : iTimeout(timeout), isOpened(false) {
     adapter = new DoorTimerAdapter(*this);
}

bool TimedDoor::isDoorOpened() {
    return isOpened;
}

void TimedDoor::unlock() {
    if (isDoorOpened()) {
        throw std::logic_error("Door opened");
    }
    isOpened = true;
    adapter->Timeout();
}

void TimedDoor::lock() {
    if (!isDoorOpened()) {
        throw std::logic_error("Door closed");
    }
    isOpened = false;
}

int TimedDoor::getTimeOut() const {
    return iTimeout;
}

void TimedDoor::throwState() {
    if (isDoorOpened()) {
        throw std::runtime_error("Timeout");
    }
}

void Timer::tregister(int timeout, TimerClient* client_) {
    this->client = client_;
    sleep(timeout);
    client_->Timeout();
}

void Timer::sleep(int timeout) {
    std::this_thread::sleep_for(std::chrono::seconds(timeout));
}
// Copyright 2024 Dostavalov Semyon

#include "TimedDoor.h"

DoorTimerAdapter::DoorTimerAdapter(TimedDoor& door_) : door(door_) {}

void DoorTimerAdapter::Timeout() {
    door.throwState();
}

TimedDoor::TimedDoor(int timeout) : iTimeout(timeout), isOpened(false) {
     adapter = new DoorTimerAdapter(*this);
}

bool TimedDoor::isDoorOpened() {
    return isOpened;
}

void TimedDoor::unlock() {
    if (isDoorOpened()) {
        throw std::logic_error("Door opened");
    }
    isOpened = true;
    adapter->Timeout();
}

void TimedDoor::lock() {
    if (!isDoorOpened()) {
        throw std::logic_error("Door closed");
    }
    isOpened = false;
}

int TimedDoor::getTimeOut() const {
    return iTimeout;
}

void TimedDoor::throwState() {
    if (isDoorOpened()) {
        throw std::runtime_error("Timeout");
    }
}

void Timer::tregister(int timeout, TimerClient* client_) {
    this->client = client_;
    sleep(timeout);
    client_->Timeout();
}

void Timer::sleep(int timeout) {
    std::this_thread::sleep_for(std::chrono::seconds(timeout));
}
