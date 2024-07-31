option(IMGUI_OPENGL OFF)
option(IMGUI_VULKAN OFF)

project(imgui)

add_library(
    imgui STATIC
        imgui/imconfig.h
        imgui/imgui.h
        imgui/imgui_internal.h
        imgui/imstb_rectpack.h
        imgui/imstb_textedit.h
        imgui/imstb_truetype.h
        imgui/backends/imgui_impl_glfw.h

        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/backends/imgui_impl_glfw.cpp
)

if (IMGUI_OPENGL)
    target_sources(
        imgui
        PUBLIC
            imgui/backends/imgui_impl_opengl3.h
            imgui/backends/imgui_impl_opengl3.cpp
    )
elseif (IMGUI_VULKAN)
    target_sources(
        imgui
        PUBLIC
            imgui/backends/imgui_impl_vulkan.h
            imgui/backends/imgui_impl_vulkan.cpp
    )
endif()

target_include_directories(
    imgui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui
        ${CMAKE_SOURCE_DIR}/Thirdparty/glfw/include
)
