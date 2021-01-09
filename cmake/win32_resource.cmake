##
# Win32 resource compiler and linker cmake script.
# 
# - Works with MSVC and MinGW build.
#
# // you can set the following variables:
#
# RESOURCE_INCLUDE_DIR - The include path where the resource header file exists.
#    DEFAULT= ${PROJECT_SOURCE_DIR}/include
# RESOURCE_HEADER_FILE - The resource header file defining resource IDs.
#    DEFAULT=resource.h
#    This file must be created in the ${RESOURCE_INCLUDE_DIR} path.
# RESOURCE_MANIFEST - The path to the application manifest file.
# RESOURCE_EXTRA_CODE - extra resource code to be appended.
#    DEFAULT=
# RESOURCE_OUTPUT - The output resource file's path. -- *Compiled output*.
# RESOURCE_INPUT - The input resource file's path. -- *Generated input*.
#
# generate_resource() - The function that generates the resource header file and the resource object file.
# link_resource(TARGET) - The function for linking resource output to target.
# generate_resource_id(ID) - The function for reserving resource IDs.
# add_resource_icon(ID ICON_FILE) - The function for adding an icon resource.
# add_resource_string(ID STRING) - The function for adding a string value to the string table.
# begin_resource_menu(ID) - Starts the definition of a menu resource. Don't call any other non-menu function until you end this.
# end_resource_menu() - Ends the previous defifnition of a menu resource.
# begin_resource_menu_popup(NAME) - Starts a menu popup definition.
# end_resource_menu_popup() - Ends a privious menu popup definition.
# add_resource_menu_item(ID NAME) - Adds a menu item. Call this function only inbetween menu or popup menu.
#
##
if( NOT DEFINED RESOURCE_INCLUDE_DIR )
	set( RESOURCE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include" )
endif()
if( NOT DEFINED RESOURCE_HEADER_FILE )
	set( RESOURCE_HEADER_FILE "resource.h" )
endif()
if( NOT DEFINED RESOURCE_EXTRA_CODE )
	set( RESOURCE_EXTRA_CODE "" )
endif()
set( RESOURCE_MANIFEST_CODE "" )
set( RESOURCE_ICON_CODE "" )
set( RESOURCE_STRING_TABLE "" )
set( RESOURCE_STRING_TABLE_CODE "" )
set( RESOURCE_MENU_CODE "" )
set( RESOURCE_OUTPUT "${PROJECT_BINARY_DIR}/resource.res" )
set( RESOURCE_INPUT "${PROJECT_BINARY_DIR}/resource.rc" )
set( RESOURCE_HEADER_CONTENT "" )
set( RESOURCE_ID_INDEX 200 )

if( MSVC )
	add_custom_command( OUTPUT "${RESOURCE_OUTPUT}"
		COMMAND "RC.exe" "/nologo" "/r" "/i" "${RESOURCE_INCLUDE_DIR}" "/fo" "${RESOURCE_OUTPUT}" "${RESOURCE_INPUT}"
		MAIN_DEPENDENCY "${RESOURCE_INPUT}"
		DEPENDS "${RESOURCE_MANIFEST}"
	)
elseif( MINGW )
	add_custom_command( OUTPUT "${RESOURCE_OUTPUT}"
		COMMAND "windres.exe" "-J" "rc" "-O" "coff" "-I" "${RESOURCE_INCLUDE_DIR}" "-o" "${RESOURCE_OUTPUT}" "-i" "${RESOURCE_INPUT}"
		MAIN_DEPENDENCY "${RESOURCE_INPUT}"
		DEPENDS "${RESOURCE_MANIFEST}"
	)
endif()

add_custom_target( win32_resource ALL
	DEPENDS "${RESOURCE_OUTPUT}"
	COMMENT "Compiling resources..."
	SOURCES "${RESOURCE_INPUT}"
)

function( generate_resource )
	set( MANIFEST_CODE "" )
	if( DEFINED RESOURCE_MANIFEST )
		set( MANIFEST_CODE "1 RT_MANIFEST \"${RESOURCE_MANIFEST}\"\n" )
	endif()
	set( RESOURCE_CONTENT "#define WIN32_LEAN_AND_MEAN 1\n#include <windows.h>\n#include \<${RESOURCE_HEADER_FILE}\>\n\n${MANIFEST_CODE}\n${RESOURCE_ICON_CODE}\n${RESOURCE_STRING_TABLE_CODE}\n${RESOURCE_MENU_CODE}\n${RESOURCE_EXTRA_CODE}\n" )
	file( GENERATE OUTPUT "${RESOURCE_INPUT}" CONTENT "${RESOURCE_CONTENT}" )
	file( GENERATE OUTPUT "${RESOURCE_INCLUDE_DIR}/${RESOURCE_HEADER_FILE}" CONTENT "#pragma once\n\n${RESOURCE_HEADER_CONTENT}" )
endfunction( generate_resource )

function( link_resource TARGET )
	add_dependencies( "${TARGET}" win32_resource )
	target_link_libraries( "${TARGET}" PRIVATE "${RESOURCE_OUTPUT}" )
	if( MSVC )
		target_link_options( "${TARGET}" PRIVATE "/MANIFEST:NO" )
	endif()
endfunction( link_resource )

function( generate_resource_id ID )
	math( EXPR RESOURCE_ID_INDEX "${RESOURCE_ID_INDEX} + 1" )
	set( RESOURCE_ID_INDEX ${RESOURCE_ID_INDEX} PARENT_SCOPE )
	set( RESOURCE_HEADER_CONTENT "${RESOURCE_HEADER_CONTENT}#define ${ID} ${RESOURCE_ID_INDEX}\n" PARENT_SCOPE )
endfunction( generate_resource_id )

function( add_resource_icon ID ICON_PATH )
	math( EXPR RESOURCE_ID_INDEX "${RESOURCE_ID_INDEX} + 1" )
	set( RESOURCE_ID_INDEX ${RESOURCE_ID_INDEX} PARENT_SCOPE )
	set( RESOURCE_HEADER_CONTENT "${RESOURCE_HEADER_CONTENT}#define ${ID} ${RESOURCE_ID_INDEX}\n" PARENT_SCOPE )
	set( RESOURCE_ICON_CODE "${RESOURCE_ICON_CODE}${ID} ICON \"${ICON_PATH}\"\n" PARENT_SCOPE )
endfunction( add_resource_icon )

function( add_resource_string ID VALUE )
	math( EXPR RESOURCE_ID_INDEX "${RESOURCE_ID_INDEX} + 1" )
	set( RESOURCE_ID_INDEX ${RESOURCE_ID_INDEX} PARENT_SCOPE )
	set( RESOURCE_HEADER_CONTENT "${RESOURCE_HEADER_CONTENT}#define ${ID} ${RESOURCE_ID_INDEX}\n" PARENT_SCOPE )
	string( APPEND RESOURCE_STRING_TABLE "\t${ID}, \"${VALUE}\"\n" )
	set( RESOURCE_STRING_TABLE_CODE "STRINGTABLE\nBEGIN\n${RESOURCE_STRING_TABLE}END\n" PARENT_SCOPE )
endfunction( add_resource_string )

function( begin_resource_menu ID )
	math( EXPR RESOURCE_ID_INDEX "${RESOURCE_ID_INDEX} + 1" )
	set( RESOURCE_ID_INDEX ${RESOURCE_ID_INDEX} PARENT_SCOPE )
	set( RESOURCE_HEADER_CONTENT "${RESOURCE_HEADER_CONTENT}#define ${ID} ${RESOURCE_ID_INDEX}\n" PARENT_SCOPE )
	set( RESOURCE_MENU_CODE "${RESOURCE_MENU_CODE}${ID} MENU\n\{\n" PARENT_SCOPE )
endfunction( begin_resource_menu )

function( end_resource_menu )
	set( RESOURCE_MENU_CODE "${RESOURCE_MENU_CODE}\}\n" PARENT_SCOPE )
endfunction( end_resource_menu )

function( add_resource_menu_item ID VALUE )
	math( EXPR RESOURCE_ID_INDEX "${RESOURCE_ID_INDEX} + 1" )
	set( RESOURCE_ID_INDEX ${RESOURCE_ID_INDEX} PARENT_SCOPE )
	set( RESOURCE_HEADER_CONTENT "${RESOURCE_HEADER_CONTENT}#define ${ID} ${RESOURCE_ID_INDEX}\n" PARENT_SCOPE )
	set( RESOURCE_MENU_CODE "${RESOURCE_MENU_CODE}\tMENUITEM \"${VALUE}\", ${ID}\n" PARENT_SCOPE )
endfunction( add_resource_menu_item )

function( begin_resource_menu_popup VALUE )
	set( RESOURCE_MENU_CODE "${RESOURCE_MENU_CODE}POPUP \"${VALUE}\"\n{\n" PARENT_SCOPE )
endfunction( begin_resource_menu_popup )

function( end_resource_menu_popup )
	set( RESOURCE_MENU_CODE "${RESOURCE_MENU_CODE}\}\n" PARENT_SCOPE )
endfunction( end_resource_menu_popup )
