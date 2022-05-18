
if(NOT "/home/guy/Documents/OS/Ex3/cmake-build-debug/mattanTests/googletest-download/googletest-prefix/src/googletest-stamp/googletest-gitinfo.txt" IS_NEWER_THAN "/home/guy/Documents/OS/Ex3/cmake-build-debug/mattanTests/googletest-download/googletest-prefix/src/googletest-stamp/googletest-gitclone-lastrun.txt")
  message(STATUS "Avoiding repeated git clone, stamp file is up to date: '/home/guy/Documents/OS/Ex3/cmake-build-debug/mattanTests/googletest-download/googletest-prefix/src/googletest-stamp/googletest-gitclone-lastrun.txt'")
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E remove_directory "/home/guy/Documents/OS/Ex3/cmake-build-debug/mattanTests/googletest-main"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/home/guy/Documents/OS/Ex3/cmake-build-debug/mattanTests/googletest-main'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"  clone --no-checkout "https://github.com/google/googletest.git" "googletest-main"
    WORKING_DIRECTORY "/home/guy/Documents/OS/Ex3/cmake-build-debug/mattanTests"
    RESULT_VARIABLE error_code
    )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once:
          ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/google/googletest.git'")
endif()

execute_process(
  COMMAND "/usr/bin/git"  checkout main --
  WORKING_DIRECTORY "/home/guy/Documents/OS/Ex3/cmake-build-debug/mattanTests/googletest-main"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'main'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git"  submodule update --recursive --init 
    WORKING_DIRECTORY "/home/guy/Documents/OS/Ex3/cmake-build-debug/mattanTests/googletest-main"
    RESULT_VARIABLE error_code
    )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/home/guy/Documents/OS/Ex3/cmake-build-debug/mattanTests/googletest-main'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy
    "/home/guy/Documents/OS/Ex3/cmake-build-debug/mattanTests/googletest-download/googletest-prefix/src/googletest-stamp/googletest-gitinfo.txt"
    "/home/guy/Documents/OS/Ex3/cmake-build-debug/mattanTests/googletest-download/googletest-prefix/src/googletest-stamp/googletest-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
  )
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/home/guy/Documents/OS/Ex3/cmake-build-debug/mattanTests/googletest-download/googletest-prefix/src/googletest-stamp/googletest-gitclone-lastrun.txt'")
endif()

