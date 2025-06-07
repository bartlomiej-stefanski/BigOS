add_subdirectory(scheduler)
SETUP_EXECUTABLE(example_scheduler)

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld)

target_link_options(example_scheduler PRIVATE -T ${LINKER_SCRIPT})
set_target_properties(example_scheduler
    PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})


target_link_libraries(example_scheduler PRIVATE scheduler)
target_link_libraries(example_scheduler PRIVATE Debug)
target_compile_definitions(example_scheduler PRIVATE __DEBUG__)

target_link_libraries(example_scheduler PRIVATE drivers)

COMPILE_BINARY(example_scheduler)
ADD_QEMU_TARGET(example_scheduler BIOS_IMAGE)
