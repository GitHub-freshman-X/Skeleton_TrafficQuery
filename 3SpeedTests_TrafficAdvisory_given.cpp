#include <string>
#include "catch.hpp"
#include "TrafficAdvisory.h"
#include "UtilitiesSpeedTest.h"
using std::string;

TEST_CASE("TrafficAdvisory::scale varies from 2-8 vertecies and 1-28 edges automatic finding path performed with 50 trial runs", "[part=0][weight=12]"){
    SpeedTest<string, int, TrafficAdvisory> speedTest;
    speedTest.run();
}