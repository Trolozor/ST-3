// Copyright 2024 Dostavalov Semyon

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdint>
#include "TimedDoor.h"

class MockTimerClient : public TimerClient {
 public:
    MOCK_METHOD(void, Timeout, ());
};

class TimedDoorTest : public ::testing::Test {
 protected:
    TimedDoor* door;
    MockTimerClient mockClient;

    void SetUp() override {
        door = new TimedDoor(2);
    }

    void TearDown() override {
        testing::Mock::VerifyAndClear(&mockClient);
    }
};

TEST_F(TimedDoorTest, Door_Already_Lock) {
    EXPECT_FALSE(door->isDoorOpened());
}

TEST_F(TimedDoorTest, Door_Unlock) {
    door->unlock();
    EXPECT_TRUE(door->isDoorOpened());
}

TEST_F(TimedDoorTest, Door_Unlock_Twice) {
    door->unlock();
    EXPECT_THROW(door->unlock(), std::logic_error);
}

TEST_F(TimedDoorTest, LockDoor) {
    door->unlock();
    EXPECT_TRUE(door->isDoorOpened());
    door->lock();
    EXPECT_FALSE(door->isDoorOpened());
}
TEST_F(TimedDoorTest, Door_Lock_Twice) {
    EXPECT_THROW(door->lock(), std::logic_error);
}

TEST_F(TimedDoorTest, ThrowStateWhenDoorOpenedTooLong) {
    door->unlock();
    EXPECT_THROW(door->throwState(), std::runtime_error);
}

TEST_F(TimedDoorTest, ThrowStateWhenDoorNotOpenedTooLong) {
    EXPECT_NO_THROW(door->throwState());
}

TEST_F(TimedDoorTest, ThrowLogicErrorWhenUnlockingAlreadyOpenedDoor) {
    door->unlock();
    EXPECT_THROW(door->unlock(), std::logic_error);
}

TEST_F(TimedDoorTest, ThrowLogicErrorWhenLockingAlreadyClosedDoor) {
    EXPECT_THROW(door->lock(), std::logic_error);
}

TEST_F(TimedDoorTest, TimerClientTimeoutCalledAfterRegistering) {
    EXPECT_CALL(mockClient, Timeout()).Times(1);
    Timer timer;
    timer.tregister(door->getTimeOut(), &mockClient);
}

TEST_F(TimedDoorTest, TimerClientTimeoutNotCalledBeforeTimeout) {
    EXPECT_CALL(mockClient, Timeout()).Times(0);
    Timer timer;
    timer.tregister(door->getTimeOut() - 1, &mockClient);
}

TEST_F(TimedDoorTest, TimerClientTimeoutCalledAfterTimeout) {
    door->unlock();
    EXPECT_CALL(mockClient, Timeout()).Times(1);
    Timer timer;
    timer.tregister(door->getTimeOut(), &mockClient);
}

TEST_F(TimedDoorTest, TimerClientTimeoutCalledOnce) {
    door->unlock();
    EXPECT_CALL(mockClient, Timeout()).Times(1);
    Timer timer;
    timer.tregister(door->getTimeOut() * 2, &mockClient);
}
