file(GLOB_RECURSE KRYOS_SOURCES RECURSE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
file(GLOB_RECURSE KRYOS_HEADERS RECURSE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")

target_sources(KryosRuntime
    PUBLIC  ${KRYOS_SOURCES}
    PRIVATE ${KRYOS_HEADERS}
)
target_include_directories(KryosRuntime PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(KryosRuntime
    PUBLIC
        KRYOS_RHI_OPENGL
        # KRYOS_RHI_VULKAN
)

