if (BIGOS_BUILD_TESTS)
    SETUP_EXECUTABLE(test_bigos)

    set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld)

    target_link_options(test_bigos PRIVATE -T ${LINKER_SCRIPT})
    set_target_properties(test_bigos PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

    target_link_libraries(test_bigos PRIVATE stdbigos vfs Taste)

    COMPILE_BINARY(test_bigos)
    ADD_TASTE_TEST(test_bigos)
endif()
