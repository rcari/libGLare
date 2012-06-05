# Headers for the GLr::wrappers namespace

SET (
	GLr_MOC_HDRS
	${GLr_MOC_HDRS}
	
	${CMAKE_CURRENT_LIST_DIR}/scene/WVisualScene.hpp
)

SET (
	GLr_HDRS
	${GLr_HDRS}
	
	${CMAKE_CURRENT_LIST_DIR}/DummyWrapper.hpp
	${CMAKE_CURRENT_LIST_DIR}/IWrapperGenerator.hpp
	${CMAKE_CURRENT_LIST_DIR}/WElementInstance.hpp
	${CMAKE_CURRENT_LIST_DIR}/geometry/IWMesh.hpp
	${CMAKE_CURRENT_LIST_DIR}/geometry/IWTrianglesPatch.hpp
	${CMAKE_CURRENT_LIST_DIR}/geometry/WGeometry.hpp
	${CMAKE_CURRENT_LIST_DIR}/geometry/WMesh_GL2.hpp
	${CMAKE_CURRENT_LIST_DIR}/geometry/WTrianglesPatch_GL2.hpp
	${CMAKE_CURRENT_LIST_DIR}/scene/WMatrix.hpp
	${CMAKE_CURRENT_LIST_DIR}/scene/WNode.hpp
	${CMAKE_CURRENT_LIST_DIR}/scene/WRotation.hpp
	${CMAKE_CURRENT_LIST_DIR}/scene/WScale.hpp
	${CMAKE_CURRENT_LIST_DIR}/scene/WTransformation.hpp
	${CMAKE_CURRENT_LIST_DIR}/scene/WTranslation.hpp
)
