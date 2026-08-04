//
// What decides that a run has room for another group. A group arrives when
// merging has thinned the run out enough that adding one brings it back to
// roughly the population it started with - so the run refills at the rate it
// actually merges, and the particle count never climbs above where it began.
//

#include "catch.hpp"

#include "../src/Physics/ArrivalSchedule.h"

TEST_CASE("A full run has no room", "[arrivals]") {
    ArrivalSchedule schedule;
    schedule.setTargetPopulation(1000);

    REQUIRE(schedule.roomFor(1000) == 0);

    // Nor does one that has somehow grown past its target.
    REQUIRE(schedule.roomFor(1200) == 0);
}

TEST_CASE("Small gaps are not worth filling", "[arrivals]") {
    ArrivalSchedule schedule;
    schedule.minimumDeficitFraction = 0.15;
    schedule.setTargetPopulation(1000);

    REQUIRE(schedule.roomFor(990) == 0);   // 1% merged away
    REQUIRE(schedule.roomFor(900) == 0);   // 10%, still not worth it
    REQUIRE(schedule.roomFor(860) == 0);   // 14%, just under the threshold
    REQUIRE(schedule.roomFor(850) == 150); // 15% - now it is worth it
}

TEST_CASE("Once the gap is worth filling, it is filled exactly", "[arrivals]") {
    ArrivalSchedule schedule;
    schedule.minimumDeficitFraction = 0.15;
    schedule.setTargetPopulation(1000);

    // The room offered is the shortfall, so an arrival lands back on target
    // rather than overshooting it.
    REQUIRE(schedule.roomFor(850) == 150);
    REQUIRE(schedule.roomFor(400) == 600);
    REQUIRE(schedule.roomFor(1) == 999);
}

TEST_CASE("A run that merges faster gets groups sooner", "[arrivals]") {
    // The point of the whole design: the rate is not set anywhere, it falls
    // out of how fast the run is actually merging.
    ArrivalSchedule eager;
    eager.minimumDeficitFraction = 0.05;
    eager.setTargetPopulation(1000);

    ArrivalSchedule patient;
    patient.minimumDeficitFraction = 0.50;
    patient.setTargetPopulation(1000);

    REQUIRE(eager.roomFor(940) > 0);
    REQUIRE(patient.roomFor(940) == 0);
    REQUIRE(patient.roomFor(490) > 0);
}

TEST_CASE("The limit is a safety net, off by default", "[arrivals]") {
    SECTION("off by default") {
        ArrivalSchedule schedule;
        schedule.setTargetPopulation(1000);

        for (int i = 0; i < 500; i++) {
            REQUIRE(schedule.roomFor(500) > 0);
            schedule.noteArrival();
        }
    }

    SECTION("enforced when set") {
        ArrivalSchedule schedule;
        schedule.limit = 3;
        schedule.setTargetPopulation(1000);

        for (int i = 0; i < 3; i++) {
            REQUIRE(schedule.roomFor(500) > 0);
            schedule.noteArrival();
        }

        REQUIRE(schedule.roomFor(500) == 0);
        REQUIRE(schedule.arrivalsSoFar() == 3);
    }
}

TEST_CASE("Nothing arrives before a target is known", "[arrivals]") {
    ArrivalSchedule schedule;
    REQUIRE(schedule.roomFor(0) == 0);
    REQUIRE(schedule.roomFor(500) == 0);
}

TEST_CASE("Refilling holds the population under its starting count", "[arrivals]") {
    // A run merging steadily, topped up whenever there is room. The ceiling is
    // what keeps a long run from bogging down.
    ArrivalSchedule schedule;
    schedule.minimumDeficitFraction = 0.15;
    schedule.setTargetPopulation(3000);

    int population = 3000;
    int arrivals = 0;
    int highWaterMark = population;

    for (int frame = 0; frame < 5000; frame++) {
        population -= 3;  // merging away steadily
        if (population < 1) {
            population = 1;
        }

        const int room = schedule.roomFor(population);
        if (room > 0) {
            population += room;
            schedule.noteArrival();
            arrivals++;
        }

        highWaterMark = std::max(highWaterMark, population);
    }

    REQUIRE(highWaterMark == 3000);      // never above where it started
    REQUIRE(arrivals > 10);              // and it kept being refilled
    REQUIRE(population > 3000 * 0.8);    // without being allowed to drain away
}
