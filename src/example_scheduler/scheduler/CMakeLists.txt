add_library(scheduler STATIC
    scheduler.c
    scheduler.h
)

target_link_libraries(scheduler PRIVATE stdbigos)
target_include_directories(scheduler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})