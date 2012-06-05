# Headers for the GLr::opengl namespace

SET (
	GLr_MOC_HDRS
	${GLr_MOC_HDRS}
	
)

SET (
	GLr_HDRS
	${GLr_HDRS}
	
	${CMAKE_CURRENT_LIST_DIR}/buffers/BufferObject_GL2.hpp
	${CMAKE_CURRENT_LIST_DIR}/buffers/BufferObject_GL3.hpp
	${CMAKE_CURRENT_LIST_DIR}/buffers/BufferObject.hpp
	${CMAKE_CURRENT_LIST_DIR}/buffers/IBufferObject.hpp
	
	${CMAKE_CURRENT_LIST_DIR}/shading/Program.hpp
	${CMAKE_CURRENT_LIST_DIR}/shading/Shader.hpp
	
	${CMAKE_CURRENT_LIST_DIR}/state/VertexArrayObject.hpp
	
	${CMAKE_CURRENT_LIST_DIR}/GLWidget.hpp
)
