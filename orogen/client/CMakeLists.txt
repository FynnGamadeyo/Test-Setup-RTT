PROJECT(client)
cmake_minimum_required(VERSION 2.6)
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++0x" )

SET (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/.orogen/config")

INCLUDE(clientBase)

