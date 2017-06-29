# Create libwssha256-uapi as a shared library, containing a single object file 
# wssha256-uapi.o built from source file wssha256-uapi.c in the current dir
ADD_LIBRARY(wssha256-uapi SHARED wssha256-uapi.c)
ADD_LIBRARY(wssha256-uapi-static STATIC wssha256-uapi.c)
