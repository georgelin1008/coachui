# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/CoachUI_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/CoachUI_autogen.dir/ParseCache.txt"
  "CoachUI_autogen"
  )
endif()
