# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "button_matrix.bin"
  "button_matrix.hex"
  "button_matrix.map"
  )
endif()
