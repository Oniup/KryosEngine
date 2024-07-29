file(GLOB_RECURSE KRYOS_SOURCES RECURSE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
file(GLOB_RECURSE KRYOS_HEADERS RECURSE ${CMAKE_CURRENT_SOURCE_DIR} "*.h")

add_executable(KryosEditor
    ${KRYOS_SOURCES}
    ${KRYOS_HEADERS}
)
target_include_directories(KryosEditor
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${KryosRuntime_SOURCE_DIRS}
)
target_link_libraries(KryosEditor
    PUBLIC
        KryosRuntime
)
