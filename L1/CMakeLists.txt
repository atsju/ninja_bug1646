cmake_minimum_required (VERSION 3.15)

add_library(L1A STATIC EXCLUDE_FROM_ALL)
target_include_directories(L1A PUBLIC   A)
target_sources(L1A PRIVATE  A/L1A.c)
target_link_libraries(L1A   PUBLIC  L2A
                                    L2B
                                    L2C)

add_library(L1B STATIC EXCLUDE_FROM_ALL)
target_include_directories(L1B PUBLIC   B)
target_sources(L1B PRIVATE  B/L1B.c)
target_link_libraries(L1B   PUBLIC  L2A
                                    L2B
                                    L2C)

add_library(L1C STATIC EXCLUDE_FROM_ALL)
target_include_directories(L1C PUBLIC   B)
target_sources(L1C PRIVATE  C/L1C.c)
target_link_libraries(L1C   PUBLIC  L2A
                                    L2B
                                    L2C)
