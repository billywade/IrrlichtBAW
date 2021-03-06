cmake_minimum_required(VERSION 3.1)
project(Irrlicht LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11, but is it really needed?

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

SET(CMAKE_CONFIGURATION_TYPES "Debug;Release")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type: Debug or Release" FORCE)
endif()

macro(irr_adjust_flags)
	if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		add_compile_options(
			-Wextra
			-Wno-unused-parameter
			-fno-exceptions
			-fstrict-aliasing
			-msse3
			-mfpmath=sse
			-fuse-ld=gold
			
			-Wextra
			-Wno-sequence-point
			-Wno-error=ignored-attributes
			-Wno-error=unused-function
			-Wno-error=unused-variable
			-Wno-error=unused-parameter
			
			#"$<$<CONFIG:DEBUG>:-g>" # same as with -O3 below
			"$<$<CONFIG:DEBUG>:-Wall>"
			"$<$<CONFIG:DEBUG>:-fno-omit-frame-pointer>"
			"$<$<CONFIG:DEBUG>:-fstack-protector-strong>"
			#"$<$<CONFIG:RELEASE>:-O3>" # added by default and apparently cmake is not clever enough to filter duplicates for this one
			"$<$<CONFIG:RELEASE>:-fexpensive-optimizations>"
		)
		link_libraries(
			-fuse-ld=gold
		)
		if(FAST_MATH)
			add_compile_options(
				"$<$<CONFIG:RELEASE>:-ffast-math>"
			)
		endif()
		if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
			add_compile_options("$<$CONFIG:DEBUG>:-ggdb3>")
		endif()
	elseif(MSVC) # /arch:sse3 or anything like this is not needed on x64 on MSVC for enabling sse3 instructions
		# debug
		string(REPLACE "/MDd" "/MTd" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
		string(REPLACE "/W3" "/W0" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MP /fp:fast")
		string(REPLACE "/MDd" "/MTd" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
		string(REPLACE "/W3" "/W0" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
		set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MP /fp:fast")
		
		#release
		string(REPLACE "/GS" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}") # for some reason simply replacing /GS -> /GS- doesn't work... so it vanishes here and appears a few lines below!
		string(REPLACE "/O2" "/Ox" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
		string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GL /MP /Gy- /Zc:wchar_t /Zi /sdl- /GF /Ot /GS- /Oy /Oi /fp:fast")
		string(REPLACE "/GS" "" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
		string(REPLACE "/O2" "/Ox" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
		string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
		set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /GL /MP /Gy- /Zc:wchar_t /Zi /sdl- /GF /Ot /GS- /Oy /Oi /fp:fast")
	else()
		message(WARNING "UNTESTED COMPILER DETECTED, EXPECT WRONG OPTIMIZATION FLAGS! SUBMIT ISSUE ON GITHUB https://github.com/buildaworldnet/IrrlichtBAW/issues")
	endif()
endmacro()

macro(irr_adjust_definitions)
	add_definitions(
		-DPNG_THREAD_UNSAFE_OK
		-DPNG_NO_MMX_CODE
		-DPNG_NO_MNG_FEATURES
		-D_7ZIP_ST
		-DSPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS
	)

	if(WIN32)
		add_definitions(
			-DWIN32
			-D__GNUWIN32__
			-D_CRT_SECURE_NO_DEPRECATE
			-DNOMINMAX
		)
	endif()

	if(FAST_MATH)
		add_definitions(-DIRRLICHT_FAST_MATH)
	endif()
	
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
endmacro()

option(FAST_MATH "Enable fast low-precision math" ON)

option(BUILD_EXAMPLES "Enable building examples" ON)

option(BUILD_TOOLS "Enable building tools (just convert2BAW as for now)" ON)

option(NASTY_OPENSSL_WORKAROUND "Enable this if you can't get CMake to find OpenSSL and you have it in ../openssl/ relative to this CMakeLists.txt" OFF)
add_subdirectory(source/Irrlicht) # Irrlicht code

if(BUILD_TOOLS)
	add_subdirectory(tools) # convert2BAW
endif()
if(BUILD_EXAMPLES)
	add_subdirectory(examples_tests)
endif()