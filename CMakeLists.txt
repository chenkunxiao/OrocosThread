cmake_minimum_required(VERSION 2.8)
project(test)
#include_directories(/home/addison/src/Orocos_Port_Test)
#ADD_SUBDIRECTORY( os )
#ADD_SUBDIRECTORY( base )
#ADD_SUBDIRECTORY( internal )
aux_source_directory(./base BASE_SRC)
aux_source_directory(./internal INTERNAL_SRC)
aux_source_directory(./os OS_SRC)
aux_source_directory(./types TYPE_SRC)
AUX_SOURCE_DIRECTORY(. DIR_SRCS)
aux_source_directory(./os/tlsf MEMORY_SRC)
list(APPEND DIR_SRCS ${MEMORY_SRC})
list(APPEND DIR_SRCS ${OS_SRC})
list(APPEND DIR_SRCS ${BASE_SRC})
list(APPEND DIR_SRCS ${INTERNAL_SRC})
list(APPEND DIR_SRCS ${TYPE_SRC})
#list(APPEND DIR_SRCS ${DIR_SRCS})
add_executable(test ${DIR_SRCS} internal/DataSource.inl)
target_link_libraries(test pthread /home/addison/Downloads/software/boost_1_53_0/stage/lib/libboost_filesystem.so.1.53.0 /home/addison/Downloads/software/boost_1_53_0/stage/lib/libboost_system.so.1.53.0)
