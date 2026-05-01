cmake_minimum_required(VERSION 3.20)

set(EDITOR_PROJECT_NAME AvionEngineEditor)

set(IMGUI_DIR libs/imgui)

add_library(${EDITOR_PROJECT_NAME} SHARED
    src/AvionEngineEditor/editor/editor_app.cpp
    src/AvionEngineEditor/editor/editor_gui.cpp
    src/AvionEngineEditor/ui/av_gui_context.cpp
    src/AvionEngineEditor/ui/av_gui_scoped_window.cpp
    src/AvionEngineEditor/ui/av_gui_tab_bar.cpp
    src/AvionEngineEditor/ui/av_gui_tab_item.cpp
    src/AvionEngineEditor/ui/utils/imgui_helper.cpp
    src/AvionEngineEditor/panels/inspector_panel.cpp
    src/AvionEngineEditor/panels/profiler_panel.cpp
    src/AvionEngineEditor/panels/hierarchy_panel.cpp
    src/AvionEngineEditor/panels/asset_panel.cpp
    libs/imgui/imgui_draw.cpp
    libs/imgui/imgui_tables.cpp
    libs/imgui/imgui_widgets.cpp
    libs/imgui/imgui.cpp
    libs/imgui/backends/imgui_impl_glfw.cpp
    libs/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(${EDITOR_PROJECT_NAME} 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes
    PUBLIC ${IMGUI_DIR}
    PUBLIC ${IMGUI_DIR}/backends
)

target_link_libraries(${EDITOR_PROJECT_NAME} AvionEngineCore)

target_compile_features(${EDITOR_PROJECT_NAME} PUBLIC cxx_std_23)
target_compile_options(${EDITOR_PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
target_compile_options(${EDITOR_PROJECT_NAME} PRIVATE -O3)
