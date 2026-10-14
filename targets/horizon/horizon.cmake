message("*** Horizon target")
set(DEQP_TARGET_NAME "Horizon")

if (NOT NINTENDO_SWITCH)
	message(FATAL_ERROR "Use the Horizon toolchain with devkitPro/libnx")
endif ()

set(DEQP_HORIZON_VULKAN_LIBRARY "" CACHE FILEPATH "Read-only mesa-switch loaderless libvulkan.a")
if (NOT EXISTS "${DEQP_HORIZON_VULKAN_LIBRARY}")
	message(FATAL_ERROR "DEQP_HORIZON_VULKAN_LIBRARY must name an existing mesa-switch libvulkan.a")
endif ()

set(TCUTIL_PLATFORM_SRCS horizon/tcuHorizonPlatform.cpp)
set(TCUTIL_PLATFORM_LIBS "${DEQP_HORIZON_VULKAN_LIBRARY}" expat zstd z nx m)

# Newlib/libnx provides the POSIX APIs used by the portability libraries.
add_definitions(-D_XOPEN_SOURCE=700 -D_DEFAULT_SOURCE)
