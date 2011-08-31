project(FcitxSkinViewer)
cmake_minimum_required(VERSION 2.6)

find_package(Qt4 REQUIRED QtCore QtGui)
find_package(Fcitx REQUIRED)

configure_file(config.h.in config.h)

add_subdirectory(src)
