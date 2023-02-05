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

target_compile_features(${TARGET} PRIVATE cxx_std_20)