# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/QtChatClient_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/QtChatClient_autogen.dir/ParseCache.txt"
  "QtChatClient_autogen"
  )
endif()
