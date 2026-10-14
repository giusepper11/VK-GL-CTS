# cmake file for amber

set(AMBER_ENABLE_SHARED_CRT TRUE)
set(AMBER_SKIP_TESTS TRUE)
set(AMBER_SKIP_SPIRV_TOOLS TRUE)
set(AMBER_SKIP_SHADERC TRUE)
set(AMBER_SKIP_SAMPLES TRUE)
set(AMBER_ENABLE_SHARED_CRT TRUE)
set(AMBER_ENABLE_RTTI TRUE CACHE BOOL "Enable runtime type information" FORCE)
set(AMBER_CTS_INL_DIR ${PROJECT_BINARY_DIR}/external/vulkancts/framework/vulkan/generated/vulkan)
set(Vulkan-Headers_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../vulkancts/framework/vulkan/generated/vulkan/)

if (DE_COMPILER_IS_GCC OR DE_COMPILER_IS_CLANG)
       add_compile_options(-w)
elseif (DE_COMPILER_IS_MSC)
       add_compile_options(/W0)
endif ()



add_subdirectory(src)

if (DE_OS_IS_HORIZON)
	# Amber's platform detection does not recognize libnx's POSIX support.
	# Replace only its sleep adapter, keeping the downloaded sources intact.
	set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/src/sleep.cc
		DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/src PROPERTIES HEADER_FILE_ONLY TRUE)
	target_sources(libamber PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/horizon-sleep.cpp)
	# pthread functions are supplied by libnx rather than a separate library.
	foreach (LINK_PROPERTY LINK_LIBRARIES INTERFACE_LINK_LIBRARIES)
		get_target_property(HORIZON_AMBER_LIBS libamber ${LINK_PROPERTY})
		list(REMOVE_ITEM HORIZON_AMBER_LIBS pthread)
		set_property(TARGET libamber PROPERTY ${LINK_PROPERTY} "${HORIZON_AMBER_LIBS}")
	endforeach ()
endif ()
