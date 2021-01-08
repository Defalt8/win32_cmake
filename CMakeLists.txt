cmake_minimum_required( VERSION 3.8 )

project( win )

set( MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" )

######################################
set( RESOURCE_MANIFEST "${PROJECT_SOURCE_DIR}/resources/win.exe.manifest" )
include( "toolchains/win32_resource.cmake")
add_resource_icon( ID_APP_ICON "${PROJECT_SOURCE_DIR}/resources/icons/axl.ico" )
add_resource_icon( ID_APP_ICON_ALT "${PROJECT_SOURCE_DIR}/resources/icons/axl.alt.ico" )
add_resource_string( ID_APP_NAME "Win32 App" )
begin_resource_menu( ID_MAIN_MENU )
	begin_resource_menu_popup( "&File" )
		add_resource_menu_item( ID_MAIN_MENU_ITEM_FILE_OPEN "&Open" )
		add_resource_menu_item( ID_MAIN_MENU_ITEM_FILE_EXIT "&Exit" )
	end_resource_menu_popup()
end_resource_menu()
generate_resource_id( ID_BUTTON_1 )
generate_resource()
####################################

add_executable( win WIN32 main.c )
target_include_directories( win PUBLIC "${PROJECT_SOURCE_DIR}/include" )
find_library( LIBCOMCTL ComCtl32 )
target_link_libraries( win PUBLIC ${LIBCOMCTL} )
link_resource( win )
