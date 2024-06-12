# ---------------------
# cmake settings
# ---------------------
cmake_minimum_required(VERSION 3.29.2)
set(CMAKE_CXX_STANDARD 20)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# ---------------------
# project
# ---------------------
project(palletfix LANGUAGES CXX ASM_MASM)
add_subdirectory(vendor)

# ---------------------
# dll
# ---------------------
file(GLOB_RECURSE DLL_SOURCES CONFIGURE_DEPENDS "src/palletfix/*.cpp" "src/palletfix/*.h" "src/palletfix/*.asm")
add_library(palletfix-dll SHARED ${DLL_SOURCES})
set_target_properties(palletfix-dll PROPERTIES OUTPUT_NAME "dwmapi")
target_compile_definitions(palletfix-dll PRIVATE ADDS _AMD64_
                                                 WIN32_LEAN_AND_MEAN)
target_link_libraries(palletfix-dll PRIVATE safetyhook Zydis)
target_include_directories(palletfix-dll PRIVATE "src/palletfix/")

# ---------------------
# folder grouping
# ---------------------
function(group_files sources)
foreach(FILE ${sources})
  get_filename_component(PARENT_DIR "${FILE}" DIRECTORY)
  string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" GROUP "${PARENT_DIR}"
  )# remove full dir
  string(REGEX MATCH "[(A-z)|(a-z)|(0-9)]+/[(A-z)|(a-z)|(0-9)]+" GROUPNAME
               ${GROUP}) # get the first two folders from dir
  string(LENGTH ${GROUPNAME} GROUPLEN) # get project name len
  string(SUBSTRING ${GROUP} ${GROUPLEN} -1 GROUP) # remove project name from
                                                  # dir
  source_group("${GROUP}" FILES "${FILE}") # group file
endforeach()
endfunction()

group_files("${DLL_SOURCES}")
