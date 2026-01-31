cmake_minimum_required(VERSION 3.6.0)
project(neophysics)
include_directories(
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)
file(GLOB SRCS
	"${PROJECT_SOURCE_DIR}/include/*"
	"${PROJECT_SOURCE_DIR}/src/*"
	"${PROJECT_SOURCE_DIR}/src/neophysics/*"
	"${PROJECT_SOURCE_DIR}/src/neophysics/core3d/*"
	
)
add_library(neophysics SHARED ${SRCS})
add_library(neophysics STATIC ${SRCS})
