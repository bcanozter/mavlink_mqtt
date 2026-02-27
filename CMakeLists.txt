project( mavlink_mqtt )
cmake_minimum_required( VERSION 3.16 )

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(INCLUDE_DIR
    ${CMAKE_SOURCE_DIR}/include
)

find_package(MAVLink REQUIRED)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/mqtt_client.cpp
    src/mavlink_manager.cpp
    src/vehicle.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${INCLUDE_DIR} 
    src
)

target_link_libraries( ${PROJECT_NAME}  MAVLink::mavlink paho-mqttpp3 paho-mqtt3as)