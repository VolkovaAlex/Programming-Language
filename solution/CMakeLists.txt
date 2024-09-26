file(GLOB_RECURSE sources CONFIGURE_DEPENDS
    src/*.c
    src/*.h
    include/*.h
)

add_executable(image-transform ${sources} "src/image.c" "include/bmp.h" "src/bmp.c" "include/transform.h" "include/file.h" "src/file.c")
target_include_directories(image-transform PRIVATE src include)
