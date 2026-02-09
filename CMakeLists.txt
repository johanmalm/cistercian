cmake_minimum_required(VERSION 3.30)
project(panel LANGUAGES C CXX)
set(QT_NO_PRIVATE_MODULE_WARNING ON)
find_package(Qt6 REQUIRED COMPONENTS Gui GuiPrivate Qml Quick WaylandClient)
find_package(LayerShellQt REQUIRED)
find_package(Wayland REQUIRED Client)
find_package(Qt6Xdg REQUIRED)

set(WLR_FOREIGN_TOPLEVEL_XML ${CMAKE_CURRENT_SOURCE_DIR}/wlr-foreign-toplevel-management-unstable-v1.xml)
qt_standard_project_setup()
qt6_add_resources(QT_RESOURCES qml.qrc)

if(ASAN)
  MESSAGE(NOTICE "Use Address Sanitizer")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fsanitize=undefined")
endif()

add_executable(panel
    main.cpp
    plugin-taskbar.cpp
    xdgimageprovider.cpp
    ${QT_RESOURCES}
)

qt6_generate_wayland_protocol_client_sources(panel FILES ${WLR_FOREIGN_TOPLEVEL_XML})

target_link_libraries(panel PRIVATE
    Qt6::Gui
    Qt6::GuiPrivate
    Qt6::Qml
    Qt6::Quick
    Qt6::WaylandClient
    Qt6Xdg
    LayerShellQtInterface
    Wayland::Client
)
