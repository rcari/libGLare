CMAKE_MINIMUM_REQUIRED ( VERSION 2.6 )

# -- Qt 4.8.0 minimum --
FIND_PACKAGE( Qt4 4.8.0 COMPONENTS QtCore QtGui QtOpenGL REQUIRED )
SET( QT_USE_OPENGL TRUE )
INCLUDE( ${QT_USE_FILE} )

# -- OpenGL --
FIND_PACKAGE( OpenGL REQUIRED )

# -- GLEW --
SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/build" )
FIND_PACKAGE( GLEW REQUIRED )

INCLUDE( build/ModuleInfos.cmake )

PROJECT ( GLare C CXX )

# Headers
INCLUDE_DIRECTORIES ( ${GLARE_INCLUDES} )

### ------------- GLr -------------
SET (
		GLr_SRCS
		
		src/GLrApplication.cpp
		src/GLrEngine.cpp
	)

INCLUDE ( src/graph/sources.cmake )
INCLUDE ( src/opengl/sources.cmake )
INCLUDE ( src/rendering/sources.cmake )
INCLUDE ( src/wrappers/sources.cmake )

SET (
		GLr_HDRS
		
		inc/GLrApplication.hpp
		inc/GLrExport.hpp
	)
	
SET (
		GLr_MOC_HDRS
		
		inc/GLrEngine.hpp
	)

INCLUDE ( inc/graph/headers.cmake )
INCLUDE ( inc/opengl/headers.cmake )
INCLUDE ( inc/rendering/headers.cmake )
INCLUDE ( inc/wrappers/headers.cmake )

### ------------- MOC --------------
QT4_WRAP_CPP ( GLr_MOC_SRCS ${GLr_MOC_HDRS} ) 

# Target library
ADD_LIBRARY ( ${${MODULE_VAR_NAME}_LIBRARY} SHARED ${GLr_SRCS} ${GLr_MOC_SRCS} ${GLr_HDRS} ${GLr_MOC_HDRS} )

# Mac OS specifics
if ( APPLE )
	set_target_properties ( 
		${GLARE_LIBRARY} PROPERTIES
		COMPILE_DEFINITIONS "_K_BUILD_GLARE;_GLARE_VERSION=\"${GLARE_VERSION_STRING}\";_K_MACX;${DEBUG_DEFINES};GLEW_MX"
		COMPILE_FLAGS -fno-common
		#LINK_FLAGS "-undefined dynamic_lookup"
		VERSION ${GLARE_VERSION_STRING}
	)
	target_link_libraries ( ${GLARE_LIBRARY} ${NIGEL_LIBRARY} ${GLEWMX_LIBRARY} ${OPENGL_LIBRARIES} ${QT_QTOPENGL_LIBRARY} ${QT_QTGUI_LIBRARY} )
# Windows specifics
elseif ( WIN32 )
	IF ( MSVC )
		SOURCE_GROUP ( "Header Files" FILES ${GLr_HDRS} ${GLr_MOC_HDRS} )
		SOURCE_GROUP ( "Source Files" FILES ${GLr_SRCS} )
		SOURCE_GROUP ( "Moc Files" FILES ${GLr_MOC_SRCS} )
	ENDIF ( MSVC )
	set_target_properties ( 
		${${MODULE_VAR_NAME}_LIBRARY} PROPERTIES
		COMPILE_DEFINITIONS "_K_BUILD_GLARE;_GLARE_VERSION=\"${GLARE_VERSION_STRING}\";_K_WIN32;${DEBUG_DEFINES};GLEW_MX"
	)
	target_link_libraries ( ${GLARE_LIBRARY} ${NIGEL_LIBRARY} ${GLEWMX_LIBRARY} ${OPENGL_LIBRARIES} ${QT_QTOPENGL_LIBRARY} ${QT_QTGUI_LIBRARY} )
# Linux specifics
elseif ( UNIX )
	set_target_properties ( 
		${GLARE_LIBRARY} PROPERTIES
		COMPILE_DEFINITIONS "_K_BUILD_GLARE;_GLARE_VERSION=\"${GLARE_VERSION_STRING}\";_K_UNIX;${DEBUG_DEFINES};GLEW_MX"
		VERSION ${GLARE_VERSION_STRING}
	)
	target_link_libraries ( ${GLARE_LIBRARY} ${NIGEL_LIBRARY} ${GLEWMX_LIBRARY} ${OPENGL_LIBRARIES} ${QT_QTOPENGL_LIBRARY} ${QT_QTGUI_LIBRARY} )
endif ( APPLE )

# Documentation
if ( DOXYGEN_FOUND )
	set( DOXYGEN_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/../doc/html )
	add_custom_command (
		OUTPUT ${DOXYGEN_OUTPUT}
		COMMAND ${CMAKE_COMMAND} -E echo_append "Building GLr Documentation..."
		COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/../doc/Doxyfile
		COMMAND ${CMAKE_COMMAND} -E echo "Done."
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../doc
	 )
	add_custom_target ( glr-doc DEPENDS ${DOXYGEN_OUTPUT} )
endif ( DOXYGEN_FOUND )
