cmake_minimum_required(VERSION 3.16)

project(cards)

set(CMAKE_CXX_STANDARD 14)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PANGOCAIRO REQUIRED pangocairo)

add_subdirectory(third_party/abseil-cpp)
add_executable(generate_pdf generate_pdf.cc)
target_include_directories(generate_pdf PUBLIC ${PANGOCAIRO_INCLUDE_DIRS})
target_compile_options(generate_pdf PUBLIC ${PANGOCAIRO_CFLAGS_OTHER})
target_link_libraries(generate_pdf absl::strings ${PANGOCAIRO_LIBRARIES})
