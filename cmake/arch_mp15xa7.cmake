if(NOT TARGET arch_mp15x_a7)

add_library(arch_mp15x_a7 INTERFACE)

set(ARCH_MP15x_A7_FLAGS
	-fno-exceptions
	-fno-math-errno
	-mcpu=cortex-a7
	-mlittle-endian
	-mfpu=neon-vfpv4
	-mfloat-abi=hard
	-mthumb-interwork
	-mno-unaligned-access
	-mtune=cortex-a7
	-mvectorize-with-neon-quad
	-funsafe-math-optimizations
)

target_compile_options(arch_mp15x_a7 INTERFACE
	${ARCH_MP15x_A7_FLAGS}
)

target_link_options(arch_mp15x_a7 INTERFACE
	${ARCH_MP15x_A7_FLAGS}
)

target_compile_options(arch_mp15x_a7 INTERFACE
	${ARCH_MP15x_A7_FLAGS}
	"SHELL:--param l1-cache-size=32"
	"SHELL:--param l1-cache-line-size=64"
	"SHELL:--param l2-cache-size=256"
	"$<$<CONFIG:Debug>:-O0>"
	"$<$<CONFIG:Debug>:-g3>"
	"$<$<CONFIG:Release>:-O3>"
	"$<$<CONFIG:RelWithDebInfo>:-O3>"
	"$<$<CONFIG:RelWithDebInfo>:-g3>"
	-fno-common
	-fdata-sections
	-ffunction-sections
	-nostdlib
	-nostartfiles
	-fno-unwind-tables
	-Wall
	-Werror=return-type
	-Wno-psabi
	-Wno-double-promotion
	-Wno-attributes
 	-shared
 	-fPIC
	"$<$<COMPILE_LANGUAGE:CXX>:-Wno-register>"
	"$<$<COMPILE_LANGUAGE:CXX>:-Wno-volatile>"
	"$<$<COMPILE_LANGUAGE:CXX>:-ffold-simple-inlines>"
	"$<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>"
)

endif()
