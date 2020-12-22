cmake_minimum_required(VERSION 3.14)

project(
	args
	VERSION 0.0.1
	DESCRIPTION "Modern C++ argument parser"
	HOMEPAGE_URL https://github.com/sonullx/args
	LANGUAGES CXX
)

add_library(sonullx.args INTERFACE)

target_include_directories(
	sonullx.args
	INTERFACE
	include
)

target_compile_features(
	sonullx.args
	INTERFACE
	cxx_std_11
)

add_executable(sonullx.args.test)

target_sources(
	sonullx.args.test
	PRIVATE
	src/test.cpp
)

target_link_libraries(
	sonullx.args.test
	PRIVATE
	sonullx.args
)
