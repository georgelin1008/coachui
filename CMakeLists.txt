cmake_minimum_required(VERSION 3.16)

project(CoachUI VERSION 0.1 LANGUAGES CXX)
set(CMAKE_AUTOMOC ON)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#  Qt 6 
find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Multimedia Gui)

# 找到 GStreamer 套件
find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0 gstreamer-video-1.0)

# 找到 OpenCV 套件
find_package(OpenCV REQUIRED)

# 建立執行檔
qt_add_executable(${PROJECT_NAME}
    main.cpp
    RecorderController.cpp
    CameraController.cpp
    VideoPlayer.cpp
    VideoComposer.cpp
    src/controllers/AppController.cpp
    src/models/VideoListModel.cpp
)

# Ensure these src files are explicitly registered with the target (sometimes
# qt_add_executable autogen/path handling can miss nested-src entries). Adding
# them again via target_sources makes sure their object files are linked.
target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/controllers/AppController.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models/VideoListModel.cpp
)

# QML 
qt_add_resources(QML_RESOURCES qml.qrc)
target_sources(${PROJECT_NAME} PRIVATE ${QML_RESOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 添加 GStreamer 包含目錄和編譯選項
target_include_directories(${PROJECT_NAME} PRIVATE ${GSTREAMER_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PRIVATE ${GSTREAMER_CFLAGS_OTHER})

# Include our src headers
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Include our src headers (controllers/models are compiled into the main executable)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# 添加 OpenCV 包含目錄
target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})

# 鏈結庫
target_link_libraries(${PROJECT_NAME}
    PRIVATE Qt6::Quick Qt6::Multimedia Qt6::Gui ${GSTREAMER_LIBRARIES} ${OpenCV_LIBS}
)
