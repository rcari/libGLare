# Sources for namespace GLr::wrappers

SET (
	GLr_SRCS
	${GLr_SRCS}
	
	${CMAKE_CURRENT_LIST_DIR}/DummyWrapper.cpp
	${CMAKE_CURRENT_LIST_DIR}/WElementInstance.cpp
	${CMAKE_CURRENT_LIST_DIR}/geometry/IWMesh.cpp
	${CMAKE_CURRENT_LIST_DIR}/geometry/IWTrianglesPatch.cpp
	${CMAKE_CURRENT_LIST_DIR}/geometry/WGeometry.cpp
	${CMAKE_CURRENT_LIST_DIR}/geometry/WMesh_GL2.cpp
	${CMAKE_CURRENT_LIST_DIR}/geometry/WTrianglesPatch_GL2.cpp
	${CMAKE_CURRENT_LIST_DIR}/scene/WMatrix.cpp
	${CMAKE_CURRENT_LIST_DIR}/scene/WNode.cpp
	${CMAKE_CURRENT_LIST_DIR}/scene/WRotation.cpp
	${CMAKE_CURRENT_LIST_DIR}/scene/WScale.cpp
	${CMAKE_CURRENT_LIST_DIR}/scene/WTransformation.cpp
	${CMAKE_CURRENT_LIST_DIR}/scene/WTranslation.cpp
	${CMAKE_CURRENT_LIST_DIR}/scene/WVisualScene.cpp
)
