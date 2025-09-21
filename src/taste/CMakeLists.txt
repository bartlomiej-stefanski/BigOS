if (BIGOS_BUILD_TESTS)
    SETUP_LIBRARY(Taste)

    target_compile_definitions(Taste PRIVATE TASTE_SERIAL=0x10000000)
endif()
