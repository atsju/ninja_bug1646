cmake_minimum_required (VERSION 3.15)

add_library(L3A STATIC EXCLUDE_FROM_ALL)
target_include_directories(L3A PUBLIC   A)
target_sources(L3A PRIVATE  A/L3A.c)
#target_link_libraries(L3A   PUBLIC  L4A)

add_library(L3B STATIC EXCLUDE_FROM_ALL)
target_include_directories(L3B PUBLIC   B)
target_sources(L3B PRIVATE  B/L3B.c)
#target_link_libraries(L3B   PUBLIC  L4A)

add_library(L3C STATIC EXCLUDE_FROM_ALL)
target_include_directories(L3C PUBLIC   C)
target_sources(L3C PRIVATE  C/L3C.c)
#target_link_libraries(L3C   PUBLIC  L4A)
