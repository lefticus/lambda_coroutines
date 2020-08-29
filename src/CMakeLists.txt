
# Generic test that uses conan libs
add_executable(lambda_coroutines main.cpp)
target_link_libraries(
  lambda_coroutines
  PRIVATE project_options
          project_warnings
          CONAN_PKG::docopt.cpp
          CONAN_PKG::fmt)

target_include_directories(lambda_coroutines PRIVATE "${CMAKE_SOURCE_DIR}/include")