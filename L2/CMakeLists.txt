cmake_minimum_required (VERSION 3.15)

add_library(L2A STATIC EXCLUDE_FROM_ALL)
target_include_directories(L2A PUBLIC   A)
target_sources(L2A PRIVATE  A/L2A.c)
target_link_libraries(L2A   PUBLIC  L3A L3B L3C)

add_library(L2B STATIC EXCLUDE_FROM_ALL)
target_include_directories(L2B PUBLIC   B)
target_sources(L2B PRIVATE  B/L2B.c)
target_link_libraries(L2A   PUBLIC  L3A L3B L3C)

add_library(L2C STATIC EXCLUDE_FROM_ALL)
target_include_directories(L2C PUBLIC   C)
target_sources(L2C PRIVATE  C/L2C.c)
target_link_libraries(L2A   PUBLIC  L3A L3B L3C)
