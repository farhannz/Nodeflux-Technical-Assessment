cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(Nodeflux-Assessment)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

include_directories(${CMAKE_SOURCE_DIR}/include)

# SUBMODULES
include_directories(${CMAKE_SOURCE_DIR}/submodules/cpp-base64)

add_executable(${PROJECT_NAME}  ${CMAKE_SOURCE_DIR}/submodules/cpp-base64/base64.cpp src/main.cpp)

find_package(OpenCV REQUIRED)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES})


find_package(libasyik REQUIRED)
if(libasyik_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${libasyik_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} libasyik)
endif()

find_package(Boost COMPONENTS context fiber url date_time REQUIRED)
if(Boost_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} Boost::fiber Boost::context Boost::date_time Boost::url) 
endif()


find_package(SOCI REQUIRED)
if(SOCI_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC /usr/include/postgresql)
    target_include_directories(${PROJECT_NAME} PUBLIC /usr/local/include/soci)
    target_include_directories(${PROJECT_NAME} PUBLIC /usr/local/include/soci/postgresql)
    target_include_directories(${PROJECT_NAME} PUBLIC /usr/local/include/soci/sqlite3)
    target_link_libraries(${PROJECT_NAME} SOCI::soci_core SOCI::soci_postgresql SOCI::soci_sqlite3)
endif()


find_package(spdlog REQUIRED)
if(spdlog_FOUND)
    target_link_libraries(${PROJECT_NAME} spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
endif()

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} Threads::Threads)

find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenSSL::SSL)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
