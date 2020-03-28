set(JAVA_PACKAGE_NAME com.google.openthread.commissioner)
set(JAVA_OUTFILE_DIR ${CMAKE_CURRENT_BINARY_DIR}/com/google/openthread/commissioner)

find_package(SWIG 4.0 REQUIRED)
find_package(Java REQUIRED)
find_package(JNI REQUIRED)

include(UseSWIG)
include(UseJava)

message("JIN_INCLUDE_DIRS ${JNI_INCLUDE_DIRS}")
include_directories(${JNI_INCLUDE_DIRS})

#set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/java/com/google/openthread/commissioner)

#list(APPEND CMAKE_SWIG_FLAGS "-package;${JAVA_PACKAGE_NAME}")

include_directories(${PROJECT_SOURCE_DIR}/include)

#set_property(SOURCE border_agent.i PROPERTY CPLUSPLUS ON)
#set_property(SOURCE defines.i PROPERTY CPLUSPLUS ON)
#set_property(SOURCE error.i PROPERTY CPLUSPLUS ON)
#set_property(SOURCE network_data.i PROPERTY CPLUSPLUS ON)
set_property(SOURCE commissioner.i PROPERTY CPLUSPLUS ON)

set(UseSWIG_TARGET_NAME_PREFERENCE LEGACY)

#swig_add_library(border-agent-java
#    TYPE SHARED
#    LANGUAGE java
#    SOURCES border_agent.i)

#swig_add_library(defines-java
#    TYPE SHARED
#    LANGUAGE java
#    SOURCES defines.i)

#swig_add_library(error-java
#    TYPE SHARED
#    LANGUAGE java
#    SOURCES error.i)

#swig_link_libraries(error-java commissioner)

#swig_add_library(network-data-java
#    TYPE SHARED
#    LANGUAGE java
#    SOURCES network_data.i)

#swig_link_libraries(network-data-java commissioner)

swig_add_library(commissioner-java
    TYPE SHARED
    LANGUAGE java
    SOURCES commissioner.i)

swig_link_libraries(commissioner-java commissioner)

install(TARGETS commissioner-java
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
