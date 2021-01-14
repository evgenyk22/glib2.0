cmake_minimum_required(VERSION 2.6.4)
project(libgliblearn_proj)
set(CMAKE_BUILD_TYPE Debug)

# /mnt/c/learn/bitfield
find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)


include_directories(${GLIB_INCLUDE_DIRS} /mnt/c/learn/bitfield  glist )
link_directories(${GLIB_LIBRARY_DIRS} /mnt/c/learn/bitfield )

add_executable(libgliblearn_proj main.c 
                                 arrayofstrings.c
                                 ghash/ghashex.c 
                                 bitfields/bfnormalize.c 
                                 bitfields/bfieldstest1.c
                                 glist/glist.c 
                                 garray/garray.c
                                 gltree/gltree.c
                                 queue/gqueue.c
                                 asyncq/asyncq.c
                                 gmainloop/gmailoop.c
              )

add_definitions(${GLIB_CFLAGS_OTHER})
target_link_libraries(libgliblearn_proj ${GLIB_LIBRARIES} bitfield )