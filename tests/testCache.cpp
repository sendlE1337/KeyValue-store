#include "../external/catch.hpp"
#include "../include/cache/facade.h"
#include <fstream>

TEST_CASE("Basic Cache Operations", "[cache]")
{
  CacheFacade<int, std::string> cache(3, "LRU");

  SECTION("Put and Get")
  {
    cache.put(1, "one");
    REQUIRE(cache.get(1).value() == "one");
    REQUIRE(cache.size() == 1);
  }

  SECTION("Eviction when full")
  {
    cache.put(1, "one");
    cache.put(2, "two");
    cache.put(3, "three");
    cache.put(4, "four"); // Should evict 1 (LRU)

    REQUIRE_FALSE(cache.get(1).has_value());
    REQUIRE(cache.get(2).value() == "two");
    REQUIRE(cache.get(4).value() == "four");
    REQUIRE(cache.size() == 3);
  }

  SECTION("Remove items")
  {
    cache.put(1, "one");
    cache.remove(1);
    REQUIRE_FALSE(cache.get(1).has_value());
    REQUIRE(cache.size() == 0);
  }

  SECTION("Clear cache")
  {
    cache.put(1, "one");
    cache.clear();
    REQUIRE(cache.size() == 0);
    REQUIRE_FALSE(cache.get(1).has_value());
  }
}

TEST_CASE("LRU Eviction Policy", "[lru]")
{
  CacheFacade<int, std::string> cache(3, "LRU");

  SECTION("Least Recently Used is evicted")
  {
    cache.put(1, "a");
    cache.put(2, "b");
    cache.put(3, "c");

    // Access 1, then add 4 (should evict 2)
    cache.get(1);
    cache.put(4, "d");

    REQUIRE_FALSE(cache.get(2).has_value());
    REQUIRE(cache.get(1).value() == "a");
    REQUIRE(cache.get(4).value() == "d");
  }
}

TEST_CASE("Edge Cases", "[edge]")
{

  SECTION("Invalid policy throws")
  {
    REQUIRE_THROWS_AS((CacheFacade<int, std::string>(2, "INVALID")), std::invalid_argument);
  }

  SECTION("Get non-existent key returns nullopt")
  {
    CacheFacade<int, std::string> cache(2, "LRU");
    REQUIRE_FALSE(cache.get(999).has_value());
  }
}

TEST_CASE("Config Initialization", "[config]")
{
  SECTION("Load from config file")
  {
    // Create a test config
    std::ofstream("test_config.json") << R"({
            "cache_limit": 2,
            "evictionPolicy": "LRU"
        })";

    CacheFacade<int, std::string> cache("test_config.json");

    REQUIRE(cache.size() == 0);

    cache.put(1, "a");
    cache.put(2, "b");
    cache.put(3, "c"); // Should evict 1 (limit=2)

    REQUIRE_FALSE(cache.get(1).has_value());
    REQUIRE(cache.get(3).value() == "c");
  }

  SECTION("Invalid config falls back to defaults")
  {
    std::ofstream("bad_config.json") << "{}"; // Missing required fields
    CacheFacade<int, std::string> cache("bad_config.json");

    // Default limit is 1000 (from Config::reset_to_default())
    REQUIRE(cache.size() == 0);
    cache.put(1, "a");
    REQUIRE(cache.get(1).value() == "a");
  }
}