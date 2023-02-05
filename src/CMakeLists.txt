set(TARGET RenderGraphStudy)

if(MSVC)
  # /WX if warnings should be treated as errors
  add_compile_options(/W4 /external:I${PROJECT_SOURCE_DIR}/dependencies /external:W0)
else()
  add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_executable(${TARGET}
  main.cpp)

target_link_libraries(${TARGET} PRIVATE
  Workshop
)

set(ASSETS_FOLDER ${PROJECT_SOURCE_DIR}/assets)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Assets.h.in include/Assets.h @ONLY)
target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include)

# create "assets/" folder next to the target executable if not exists
add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${TARGET}>/assets/)

# Below command copies all (changed?) files into the same folder, but it flattens the hierarchy
# add_custom_command(
#   TARGET ${TARGET} POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy_if_different
#   ${WORKSHOP_ASSETS}
#   $<TARGET_FILE_DIR:${TARGET}>/assets
#   VERBATIM)

# Copy each workshop asset, if changed, into new assets/workshop/ folder, keeping relative folder structure
foreach(file ${WORKSHOP_ASSETS})
  file(RELATIVE_PATH relPath ${WORKSHOP_ASSETS_FOLDER} ${file})
  add_custom_command(
    TARGET ${TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${file}
    $<TARGET_FILE_DIR:${TARGET}>/assets/workshop/${relPath}
    VERBATIM
  )
endforeach()

# Copy each app/target asset, if changed, in to new assets/ folder, keeping relative folder structure
file(GLOB_RECURSE ASSETS ${CMAKE_SOURCE_DIR}/assets/*)
foreach(file ${ASSETS})
  file(RELATIVE_PATH relPath ${CMAKE_SOURCE_DIR}/assets ${file})
  add_custom_command(
    TARGET ${TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${file}
    $<TARGET_FILE_DIR:${TARGET}>/assets/${relPath}
    VERBATIM
  )
endforeach()

target_compile_features(${TARGET} PRIVATE cxx_std_20)