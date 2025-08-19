cmake_minimum_required(VERSION 3.12)
project(CacheService)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


option(BUILD_TESTS "Build tests" ON)

add_library(cache_service
    include/cache/eviction/lru.h
    include/cache/eviction/Policy.h
    include/cache/config.h
    include/cache/facade.h
    include/cache/generic_cache.h
    include/cache/ICache.h
    src/config.cpp
)


target_include_directories(cache_service
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/external
)


target_compile_features(cache_service PUBLIC cxx_std_17)


if(BUILD_TESTS)
    
    add_executable(test_cache
        tests/test_main.cpp
        tests/testCache.cpp
    )
    
    target_link_libraries(test_cache PRIVATE cache_service)
    
    target_include_directories(test_cache
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/external
    )
    
  
    include(CTest)
    add_test(NAME CacheTests COMMAND test_cache)
endif()

install(TARGETS cache_service
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/cache/
    DESTINATION include/cache
    FILES_MATCHING PATTERN "*.h"
)