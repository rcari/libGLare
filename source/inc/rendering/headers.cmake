# Headers for the GLr::rendering namespace

SET (
	GLr_MOC_HDRS
	${GLr_MOC_HDRS}
	
	${CMAKE_CURRENT_LIST_DIR}/Renderer.hpp
)

SET (
	GLr_HDRS
	${GLr_HDRS}
	
	${CMAKE_CURRENT_LIST_DIR}/ICamera.hpp
	${CMAKE_CURRENT_LIST_DIR}/IContext.hpp
	${CMAKE_CURRENT_LIST_DIR}/IRendererObject.hpp
	${CMAKE_CURRENT_LIST_DIR}/PrivateGLWidget.hpp
)
