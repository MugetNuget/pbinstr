add_library(pbinstr STATIC
    src/pbinstr.c
)

target_include_directories(pbinstr PUBLIC include)

target_link_libraries(pbinstr PUBLIC pico_stdlib)