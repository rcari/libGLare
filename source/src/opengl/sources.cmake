# Sources for namespace GLr::opengl

SET (
	GLr_SRCS
	${GLr_SRCS}
	
	${CMAKE_CURRENT_LIST_DIR}/buffers/BufferObject_GL2.cpp
	${CMAKE_CURRENT_LIST_DIR}/buffers/BufferObject_GL3.cpp
	${CMAKE_CURRENT_LIST_DIR}/buffers/BufferObject.cpp
	${CMAKE_CURRENT_LIST_DIR}/buffers/IBufferObject.cpp
	
	${CMAKE_CURRENT_LIST_DIR}/shading/Program.cpp
	${CMAKE_CURRENT_LIST_DIR}/shading/Shader.cpp
	
	${CMAKE_CURRENT_LIST_DIR}/state/VertexArrayObject.cpp
	
	${CMAKE_CURRENT_LIST_DIR}/GLWidget.cpp
)
