include_directories(${QT_QTCORE_INCLUDE_DIR}) 
include_directories(${QT_QTGUI_INCLUDE_DIR}) 
include_directories(${PROJECT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${FCITX4_FCITX_INCLUDE_DIRS})
include_directories(${FCITX4_FCITX_CONFIG_INCLUDE_DIRS})
include_directories(${FCITX4_FCITX_UTILS_INCLUDE_DIRS})

set(FcitxSkinViewer_SRCS
    main.cpp   
    MainWindow.cpp
    MyLoadConfig.cpp
)

qt4_automoc(${FcitxSkinViewer_SRCS})
qt4_wrap_ui(FcitxSkinViewer_SRCS MainWindow.ui)
add_executable(fcitx-skin-viewer ${FcitxSkinViewer_SRCS})
target_link_libraries(fcitx-skin-viewer ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${FCITX4_FCITX_CONFIG_LIBRARIES})
