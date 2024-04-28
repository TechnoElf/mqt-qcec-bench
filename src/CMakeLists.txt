add_executable(
        ${PROJECT_NAME}
        main.cpp
        config.cpp
        instance.cpp
        bench.cpp)

# set include directories
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_BINARY_DIR}/include)

# link to the MQT::QCEC libraries
target_link_libraries(${PROJECT_NAME} PUBLIC MQT::QCEC)
target_link_libraries(${PROJECT_NAME} PRIVATE MQT::ProjectWarnings
        MQT::ProjectOptions)
