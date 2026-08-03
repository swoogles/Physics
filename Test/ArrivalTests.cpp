//
// The cadence that decides when extra groups drop into a run already in
// progress. Pure timing: no simulation, no window, no GL.
//

#include "catch.hpp"

#include "../src/Physics/ArrivalSchedule.h"

TEST_CASE("Nothing arrives before the first slot", "[arrivals]") {
    ArrivalSchedule schedule;
    schedule.firstAt = 20.0;
    schedule.everySeconds = 20.0;
    schedule.limit = 5;

    REQUIRE_FALSE(schedule.due(0.0));
    REQUIRE_FALSE(schedule.due(19.0));
    REQUIRE(schedule.arrivalsSoFar() == 0);
}

TEST_CASE("The first group arrives on its slot, once", "[arrivals]") {
    ArrivalSchedule schedule;
    schedule.firstAt = 20.0;
    schedule.everySeconds = 20.0;

    REQUIRE(schedule.due(20.0));
    REQUIRE(schedule.arrivalsSoFar() == 1);

    // The clock is polled every frame; the same second must not re-fire.
    REQUIRE_FALSE(schedule.due(20.0));
    REQUIRE_FALSE(schedule.due(21.0));
    REQUIRE(schedule.arrivalsSoFar() == 1);
}

TEST_CASE("Arrivals are spaced by the interval", "[arrivals]") {
    ArrivalSchedule schedule;
    schedule.firstAt = 5.0;
    schedule.everySeconds = 5.0;
    schedule.limit = 4;

    REQUIRE(schedule.due(5.0));
    REQUIRE_FALSE(schedule.due(9.0));
    REQUIRE(schedule.due(10.0));
    REQUIRE_FALSE(schedule.due(14.0));
    REQUIRE(schedule.due(15.0));
    REQUIRE(schedule.arrivalsSoFar() == 3);
}

TEST_CASE("The limit is a hard stop", "[arrivals]") {
    ArrivalSchedule schedule;
    schedule.firstAt = 1.0;
    schedule.everySeconds = 1.0;
    schedule.limit = 3;

    for (double now = 1.0; now < 100.0; now += 1.0) {
        schedule.due(now);
    }

    REQUIRE(schedule.arrivalsSoFar() == 3);
    REQUIRE_FALSE(schedule.due(1000.0));
}

TEST_CASE("A jump in the clock delivers one group, not the backlog", "[arrivals]") {
    ArrivalSchedule schedule;
    schedule.firstAt = 10.0;
    schedule.everySeconds = 10.0;
    schedule.limit = 5;

    // A long pause, or a very slow frame: five slots have gone by.
    REQUIRE(schedule.due(60.0));
    REQUIRE(schedule.arrivalsSoFar() == 1);

    // ...and the schedule resumes from there rather than firing every frame
    // until it has caught up.
    REQUIRE_FALSE(schedule.due(61.0));
    REQUIRE_FALSE(schedule.due(69.0));
    REQUIRE(schedule.due(70.0));
    REQUIRE(schedule.arrivalsSoFar() == 2);
}

TEST_CASE("Arrivals can be switched off entirely", "[arrivals]") {
    SECTION("by interval") {
        ArrivalSchedule schedule;
        schedule.everySeconds = 0.0;
        REQUIRE_FALSE(schedule.due(1000.0));
    }

    SECTION("by limit") {
        ArrivalSchedule schedule;
        schedule.everySeconds = 5.0;
        schedule.limit = 0;
        REQUIRE_FALSE(schedule.due(1000.0));
    }
}

TEST_CASE("Every five seconds, up to a limit", "[arrivals]") {
    // The case this whole mechanism exists for.
    ArrivalSchedule schedule;
    schedule.firstAt = 5.0;
    schedule.everySeconds = 5.0;
    schedule.limit = 6;

    int arrived = 0;
    for (int frame = 0; frame <= 24 * 60; frame++) {
        if (schedule.due(frame / 24.0)) {
            arrived++;
        }
    }

    REQUIRE(arrived == 6);
}
