set(DE_OS DE_OS_HORIZON)
set(DE_CPU DE_CPU_ARM_64)

if (NOT DEFINED ENV{DEVKITPRO})
	message(FATAL_ERROR "DEVKITPRO must point to the devkitPro installation")
endif ()

include("$ENV{DEVKITPRO}/cmake/Switch.cmake")
