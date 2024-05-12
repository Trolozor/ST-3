// Copyright 2024 Dostavalov Semyon

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cstdint>
#include "TimedDoor.h"

class MockTimerClient : public TimerClient {
public:
    MOCK_METHOD(void, Timeout, (), (override));
};

class TimedDoorTest : public testing::Test {
protected:
    TimedDoor door;
    MockTimerClient mockClient;
    Timer timer;

    TimedDoorTest() : door(2), timer() {}

    void SetUp() override {
        door.unlock();
        DoorTimerAdapter adapter(door);
    }

    void TearDown() override {
        door.lock();
        testing::Mock::VerifyAndClear(&mockClient);
    }
};

TEST_F(TimedDoorTest, Door_Def_Closed) {
    EXPECT_FALSE(door.isDoorOpened());
}

TEST_F(TimedDoorTest, Door_Opened) {
    door.unlock();
    EXPECT_TRUE(door.isDoorOpened());
}

TEST_F(TimedDoorTest, Door_Opened_Twice) {
    door.unlock();
    EXPECT_THROW(door.unlock(), std::logic_error);
}

TEST_F(TimedDoorTest, Door_Opened_Wait) {
    door.unlock();
    EXPECT_THROW(door.throwState(), std::runtime_error);
}

TEST_F(TimedDoorTest, Door_Opens_With_Timer) {
    door.unlock();
    EXPECT_TRUE(door.isDoorOpened());

    std::this_thread::sleep_for(std::chrono::seconds(1));

    EXPECT_TRUE(door.isDoorOpened());
}

TEST_F(TimedDoorTest, Door_Opens_After_Timeout) {
    door.unlock();
    EXPECT_TRUE(door.isDoorOpened());

    std::this_thread::sleep_for(std::chrono::seconds(4));

    door.unlock();
    EXPECT_TRUE(door.isDoorOpened());
}

TEST_F(TimedDoorTest, Door_Closed) {
    door.unlock();
    EXPECT_TRUE(door.isDoorOpened());

    door.lock();
    EXPECT_FALSE(door.isDoorOpened());
}

TEST_F(TimedDoorTest, Door_Close_Twice) {
    EXPECT_THROW(door.lock(), std::logic_error);
}

TEST_F(TimedDoorTest, Door_Close_After_Timeout) {
    door.unlock();
    EXPECT_TRUE(door.isDoorOpened());

    std::this_thread::sleep_for(std::chrono::seconds(4));

    EXPECT_FALSE(door.isDoorOpened());
}

TEST_F(TimedDoorTest, Door_Closed_Good) {
    EXPECT_FALSE(door.isDoorOpened());

    EXPECT_NO_THROW(door.throwState());
}

TEST_F(TimedDoorTest, Timeout_Value) {
    EXPECT_EQ(door.getTimeOut(), 2);
}

TEST_F(TimedDoorTest, Timeout_Call) {
    door.unlock();
    EXPECT_TRUE(door.isDoorOpened());

    EXPECT_CALL(mockClient, Timeout());

    std::this_thread::sleep_for(std::chrono::seconds(4));
}
