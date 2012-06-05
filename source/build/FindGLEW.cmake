# Try to find GLEW library and include path.
# Once done this will define
#
# GLEW_FOUND
# GLEW_INCLUDES
# GLEW_LIBRARY
# GLEW_MX_LIBRARY
# 

IF (WIN32)
	FIND_PATH( GLEW_INCLUDE_PATH GL/glew.h
		$ENV{PROGRAMFILES}/GLEW/include
		DOC "The directory where GL/glew.h resides")
	FIND_LIBRARY( GLEW_LIBRARY
		NAMES glew GLEW glew32 glew32s
		PATHS
		$ENV{PROGRAMFILES}/GLEW/lib
		DOC "The GLEW library")
ELSE (WIN32)
	FIND_PATH( GLEW_INCLUDES GL/glew.h
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
		/usr/local/Cellar # brew on Mac OS X
		DOC "The directory where GL/glew.h resides")
	FIND_LIBRARY( GLEW_LIBRARY
		NAMES GLEW glew
		PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
		DOC "The GLEW library")
	FIND_LIBRARY( GLEWMX_LIBRARY
		NAMES GLEWmx glewmx
		PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
		/usr/local/Cellar
		DOC "The GLEWmx library")
ENDIF (WIN32)

IF (GLEW_INCLUDES)
	SET( GLEW_FOUND 1 CACHE STRING "Set to 1 if GLEW is found, 0 otherwise")
ELSE (GLEW_INCLUDES)
	SET( GLEW_FOUND 0 CACHE STRING "Set to 1 if GLEW is found, 0 otherwise")
ENDIF (GLEW_INCLUDES)

MARK_AS_ADVANCED( GLEW_FOUND )
