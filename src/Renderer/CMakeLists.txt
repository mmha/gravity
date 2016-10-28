set(SOURCE_FILES
		internal/Buffer.cpp
		internal/ProgramPipeline.cpp
		internal/Shader.cpp
		internal/ShaderDefinitons.cpp
		internal/Texture.cpp
		internal/VertexFormat.cpp
		BufferView.cpp
		Camera.cpp
		Debug.cpp
		#DrawableBehaviour.cpp
		GeometryData.cpp
		#Mesh.cpp
		PBRMaterial.cpp
		Primitive.cpp
		Sampler.cpp
		ShaderLinkingError.cpp
)

set(HEADER_FILES
		internal/Buffer.h
		internal/ProgramPipeline.h
		internal/ShaderDefinitions.h
		internal/Shader.h
		internal/Texture.h
		internal/VertexFormat.h
		BufferView.h
		Camera.h
		Context.h
		Debug.h
		#DrawableBehaviour.h
		FPSCamera.h
		GeometryData.h
		MeshDrawCommand.h
		#Mesh.h
		PBRMaterial.h
		Primitive.h
		Renderer.h
		Sampler.h
		ShaderLinkingError.h
		Technique.h
)

file(GLOB_RECURSE HEADER_FILES *.h)

find_package(Threads REQUIRED)
find_package(glbinding REQUIRED)

add_library(GERenderer STATIC ${HEADER_FILES} ${SOURCE_FILES})
set_property(TARGET GERenderer PROPERTY VERSION ${PROJECT_VERSION})
cotire(GERenderer)

target_link_libraries(GERenderer PUBLIC GECore glbinding Threads::Threads gsl::gsl)
gravity_export_target(GERenderer ${HEADER_FILES})
