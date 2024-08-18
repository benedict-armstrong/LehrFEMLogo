cmake_minimum_required(VERSION 3.10)

set(SOURCES
  radauthreetimestepping_main.cc
  radauthreetimestepping.h
  radauthreetimestepping.cc
  eigenCSV.h
  eigenCSV.cc
)

set(LIBRARIES
  Eigen3::Eigen
  LF::lf.assemble
  LF::lf.base
  LF::lf.geometry
  LF::lf.io
  LF::lf.mesh.hybrid2d
  LF::lf.mesh.utils
  LF::lf.mesh.test_utils
  LF::lf.uscalfe
)

set(TARGET
  ${PROJECT_NAME}
)

set(OUTPUT_NAME
  ${PROJECT_NAME}
)


# Build main application
add_executable(${TARGET} ${SOURCES})
set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${OUTPUT_NAME})
target_compile_definitions(${TARGET} PRIVATE CURRENT_SOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\")
target_compile_definitions(${TARGET} PRIVATE CURRENT_BINARY_DIR=\"${CMAKE_CURRENT_BINARY_DIR}\")
# For including all symbols in the executable: Does not work on Max OS X
# target_link_libraries(${TARGET} PUBLIC   "-Wl,--whole-archive" ${LIBRARIES} "-Wl,--no-whole-archive")
target_link_libraries(${TARGET} PUBLIC ${LIBRARIES})

add_library(${TARGET}.static STATIC ${SOURCES})
set_target_properties(${TARGET}.static PROPERTIES OUTPUT_NAME ${OUTPUT_NAME}.static)
target_compile_definitions(${TARGET}.static PRIVATE CURRENT_SOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\")
target_compile_definitions(${TARGET}.static PRIVATE CURRENT_BINARY_DIR=\"${CMAKE_CURRENT_BINARY_DIR}\")
target_link_libraries(${TARGET}.static PUBLIC ${LIBRARIES})

# Build tests if exist
# if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test)
#     build_test(${TEST_TARGET} ${PROBLEM_TARGET} ${DIR} ${TEST_TARGET})
# endif()