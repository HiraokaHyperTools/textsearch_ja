cmake_minimum_required(VERSION 3.12.2)
set(CMAKE_BUILD_TYPE Release)

# PGVER?

if(false)
elseif(PGVER STREQUAL PG91)
	set(PGVERDIR "9.1")
elseif(PGVER STREQUAL PG92)
	set(PGVERDIR "9.2")
elseif(PGVER STREQUAL PG93)
	set(PGVERDIR "9.3")
elseif(PGVER STREQUAL PG94)
	set(PGVERDIR "9.4")
elseif(PGVER STREQUAL PG95)
	set(PGVERDIR "9.5")
elseif(PGVER STREQUAL PG96)
	set(PGVERDIR "9.6")
elseif(PGVER STREQUAL PG100)
	set(PGVERDIR "10.0")
elseif(PGVER STREQUAL PG110)
	set(PGVERDIR "11.0")
elseif(PGVER STREQUAL PG120)
	set(PGVERDIR "12.0")
elseif(PGVER STREQUAL PG130)
	set(PGVERDIR "13.0")
else()
	message(SEND_ERROR "PGVER: PG91 PG92 PG93 PG94 PG95 PG96 PG100 PG110 PG120 PG130")
endif()

add_definitions(-D${PGVER})

# CPU?

if(false)
elseif(CPU STREQUAL x86)
	# ok
	set(INCLUDE_MECAB "libmecab-sdk")
	set(LIBPATH_MECAB "libmecab-sdk")
elseif(CPU STREQUAL x64)
	# ok
	set(INCLUDE_MECAB "libmecab-sdk64")
	set(LIBPATH_MECAB "libmecab-sdk64")
else()
	# ng
	message(SEND_ERROR "CPU: x86 x64")
endif()

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /SAFESEH:NO")

add_compile_options(
	"$<$<CONFIG:RELEASE>:-MT>" # <RuntimeLibrary>MultiThreaded</RuntimeLibrary>
	"$<$<CONFIG:DEBUG>:-MTd>" # <RuntimeLibrary>MultiThreadedDebug</RuntimeLibrary>
	)

find_library(
	POSTGRES_LIBRARY
	NAMES postgres
	PATHS "PGfiles/${PGVERDIR}/lib_${CPU}"
	)
find_library(
	MECAB_LIBRARY
	NAMES libmecab
	PATHS "${LIBPATH_MECAB}"
	)
include_directories(
	include
	"PGfiles/${PGVERDIR}/include"
	"PGfiles/${PGVERDIR}/include/server"
	"PGfiles/${PGVERDIR}/include_${CPU}"
	"PGfiles/${PGVERDIR}/include_${CPU}/server"
	"${INCLUDE_MECAB}"
	)

# textsearch_ja
add_library(textsearch_ja SHARED
	textsearch_ja.c
	encoding_eucjp.c
	encoding_utf8.c
	)
target_link_libraries(textsearch_ja
	${POSTGRES_LIBRARY} ${MECAB_LIBRARY}
)
