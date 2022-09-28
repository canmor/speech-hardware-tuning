set(GTEST_URL "https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip" CACHE STRING "Googletest URL")
set(GTEST_URL_HASH "MD5=52943a59cefce0ae0491d4d2412c120b" CACHE STRING "Googletest URL HASH")
set(GTEST_GIT_REPOSITORY "https://github.com/google/googletest.git" CACHE STRING "Googletest Git Repository URL")
set(GTEST_GIT_TAG "release-1.11.0" CACHE STRING "Googletest Git Repository URL")

if (NOT TARGET GTest::gtest)
    find_package(GTest QUIET)
    if (NOT GTest_FOUND)
        include(FetchContent)
        if (GTEST_URL STREQUAL "")
            FetchContent_Declare(gtest GIT_REPOSITORY ${GTEST_GIT_REPOSITORY} GIT_TAG ${GTEST_GIT_TAG})
        else ()
            FetchContent_Declare(gtest URL ${GTEST_URL} URL_HASH ${GTEST_URL_HASH})
        endif()
        FetchContent_MakeAvailable(gtest)
    endif ()
endif ()