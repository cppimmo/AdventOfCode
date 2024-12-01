cmake_minimum_required(VERSION 3.5)
project(AdventOfCode2024)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Function to set target output directories and copy executables
function(set_target_output_directory_and_copy TARGET_NAME OUTPUT_DIR)
  # Set target properties for output directories
  set_target_properties(${TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}
  )

  # Add a post-build step to copy the executable to the specified directory
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:${TARGET_NAME}> ${OUTPUT_DIR}
    COMMENT "Copying ${TARGET_NAME} to ${OUTPUT_DIR}"
  )
endfunction()

# Helper to add executables from the Day directories
function(add_day_executable DAY_NUM PART_NUM)
  # Manually zero-pad the day number
  if(DAY_NUM LESS 10)
    set(PADDED_DAY_NUM "0${DAY_NUM}")
  else()
    set(PADDED_DAY_NUM "${DAY_NUM}")
  endif()

  set(DAY_DIR "Day${PADDED_DAY_NUM}")
  set(SOURCE_FILE "${DAY_DIR}/Part${PART_NUM}.cpp")
  set(TARGET_NAME "DAY${PADDED_DAY_NUM}_PART${PART_NUM}")
  set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/${DAY_DIR}/")

  # Add the executable and link Source/ files
  add_executable(${TARGET_NAME} ${SOURCE_FILE})
  target_include_directories(${TARGET_NAME} PRIVATE Source)
  target_sources(${TARGET_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Source/AdventOfCodeStd.cpp)

  # Set output directory and copy executable
  set_target_output_directory_and_copy(${TARGET_NAME} ${OUTPUT_DIR})
endfunction()

# Add executables for each day and part
foreach(DAY_NUM RANGE 1 25)
  foreach(PART_NUM RANGE 1 2)
    # Manually zero-pad the day number for directory lookup
    if(DAY_NUM LESS 10)
      set(PADDED_DAY_NUM "0${DAY_NUM}")
    else()
      set(PADDED_DAY_NUM "${DAY_NUM}")
    endif()

    if(EXISTS "${CMAKE_SOURCE_DIR}/Day${PADDED_DAY_NUM}/Part${PART_NUM}.cpp")
      add_day_executable(${DAY_NUM} ${PART_NUM})
    endif()
  endforeach()
endforeach()
