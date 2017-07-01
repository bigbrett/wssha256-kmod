SET( MODULE_NAME wssha256kern)
SET( MODULE_SOURCE_FILES ${MODULE_NAME}.c)
SET( MODULE_FILE ${MODULE_NAME}.ko)
SET( MODULE_OUTPUT_DIR ${CMAKE_BINARY_DIR}/modules/${MODULE_NAME})
ADD_CUSTOM_TARGET( ${MODULE_NAME} ALL DEPENDS ${MODULE_OUTPUT_DIR}/${MODULE_FILE})

# HACK: If we pass the KERNEL_SRC variable on the command line (in Yocto recipe) then use that as
# the search path for the kernel headers. Otherwise, we are not running under Yocto and can use
# regular CMake Variables to compile the kernel module
IF(${KERNEL_SRC})
    MESSAGE("KERNEL_SRC DEFINED!")
ELSE()
    MESSAGE("KERNEL_SRC UNDEFINED!")
    SET( KERNEL_SRC "/lib/modules/${CMAKE_SYSTEM_VERSION}/build")
ENDIF()

#SET( KERNEL_SRC "/lib/modules/${CMAKE_SYSTEM_VERSION}/build")
SET( KBUILD_COMMAND ${CMAKE_MAKE_PROGRAM} -C ${KERNEL_SRC} M=${MODULE_OUTPUT_DIR} modules)

# Set the Kbuild file as the CMake "configuration file"
CONFIGURE_FILE( Kbuild.in ${MODULE_OUTPUT_DIR}/Kbuild)

# Configure each source file 
FOREACH( MODULE_SOURCE_FILE ${MODULE_SOURCE_FILES} )
	CONFIGURE_FILE(
		${MODULE_SOURCE_FILE}
		${MODULE_OUTPUT_DIR}/${MODULE_SOURCE_FILE}
		COPYONLY
	)
ENDFOREACH( MODULE_SOURCE_FILE )

# Invoke Kbuild like the usual Makefile approach would
ADD_CUSTOM_COMMAND(
	OUTPUT ${MODULE_OUTPUT_DIR}/${MODULE_FILE}
	COMMAND ${KBUILD_COMMAND}
	WORKING_DIRECTORY ${MODULE_OUTPUT_DIR}
	DEPENDS ${MODULE_SOURCE_FILES} Kbuild.in
	VERBATIM
)

