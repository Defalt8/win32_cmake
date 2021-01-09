# win32_cmake

A simple win32 app template with a cmake resource generator script

- Works with MSVC and MinGW build.

## Variables

- `RESOURCE_INCLUDE_DIR` - The include path where the resource header file exists. `DEFAULT= ${PROJECT_SOURCE_DIR}/include`
- `RESOURCE_HEADER_FILE` - The resource header file defining resource IDs. `DEFAULT=resource.h`. This file must be created in the `${RESOURCE_INCLUDE_DIR}` path.
- `RESOURCE_MANIFEST` - The path to the application manifest file.
- `RESOURCE_EXTRA_CODE` - extra resource code to be appended. `DEFAULT=`
- `RESOURCE_OUTPUT` - The output resource file's path. -- *Compiled output*.
- `RESOURCE_INPUT` - The input resource file's path. -- *Generated input*.

## Functions

- `generate_resource()` - The function that generates the resource header file and the resource object file.
- `link_resource(target)` - The function for linking resource output to target.
- `generate_resource_id(ID)` - The function for reserving resource IDs.
- `add_resource_icon(ID ICON_FILE)` - The function for adding an icon resource.
- `add_resource_string(ID STRING)` - The function for adding a string value to the string table.
- `begin_resource_menu(ID)` - Starts the definition of a menu resource. Don't call any other non-menu function until you end this.
- `end_resource_menu()` - Ends the previous defifnition of a menu resource.
- `begin_resource_menu_popup(NAME)` - Starts a menu popup definition.
- `end_resource_menu_popup()` - Ends a privious menu popup definition.
- `add_resource_menu_item(ID NAME)` - Adds a menu item. Call this function only inbetween menu or popup menu.

## A sample resource generation cmake code.

  ```
  # Setting the manifest file path. *optional.
  set( RESOURCE_MANIFEST "${PROJECT_SOURCE_DIR}/resources/win.exe.manifest" )

  # Including the cmake script.
  include( "cmake/win32_resource.cmake")

  # Adding icons.
  add_resource_icon( ID_APP_ICON "${PROJECT_SOURCE_DIR}/resources/icons/axl.ico" )
  add_resource_icon( ID_APP_ICON_ALT "${PROJECT_SOURCE_DIR}/resources/icons/axl.alt.ico" )

  # String table.
  add_resource_string( ID_APP_NAME "Win32 App" )

  # Creating a menu.
  begin_resource_menu( ID_MAIN_MENU )
  	begin_resource_menu_popup( "&File" )
  		add_resource_menu_item( ID_MAIN_MENU_ITEM_FILE_OPEN "&Open" )
  		add_resource_menu_item( ID_MAIN_MENU_ITEM_FILE_EXIT "&Exit" )
  	end_resource_menu_popup()
  end_resource_menu()

  # Reserving custom resource IDs.
  generate_resource_id( ID_BUTTON_CALL )
  generate_resource_id( ID_EDIT_NAME )

  # Final resource file generation.
  generate_resource()
  ```

## Files

- [cmake/win32_resource.cmake]("cmake/win32_resource.cmake") The win32 resource cmake script to be included in your main cmake script.
