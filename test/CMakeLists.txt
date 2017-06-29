# compile test program wssha256-kmodtest.c into executable wssha256-kmodtest
ADD_EXECUTABLE(wssha256-kmodtest wssha256-kmodtest.c)

# link against libwssha256-uapi when building the hwuser binary.
TARGET_LINK_LIBRARIES(wssha256-kmodtest wssha256-uapi)
