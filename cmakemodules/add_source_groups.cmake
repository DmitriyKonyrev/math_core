function(add_source_groups moduleName)	

set(options OPTIONAL FAST)
set(oneValueArgs DESTINATION)
set(multiValueArgs SUBFOLDERS SUBMODULES)
cmake_parse_arguments(PARAM "${options}" "${oneValueArgs}"
                        "${multiValueArgs}" ${ARGN} )                  

if (PARAM_UNPARSED_ARGUMENTS)
		message(FATAL_ERROR "Unknown keywords given to add_source_groups(): \"${PARAM_UNPARSED_ARGUMENTS}\"")
endif()

if (PARAM_SUBFOLDERS)
    foreach(submodule ${PARAM_SUBFOLDERS})
		set(regName "${moduleName}\\${submodule}")
        add_source_groups(${regName})        
	endforeach()
endif()

if (PARAM_SUBMODULES)
    foreach(submodule ${PARAM_SUBMODULES})
		set(regName "${moduleName}/${submodule}\\")
        add_source_groups(${regName})        
	endforeach()
endif()

set(regName ${moduleName})
string (REPLACE "\\" "/resources/" regName ${regName})
set(regexstring  ".+${regName}[^.{1}:]+[.](png|qrc|html|js|css)$")
set(groupName  "${moduleName}\\Resource Files")
string (REPLACE "/" "\\" groupName ${groupName})
source_group(${groupName} REGULAR_EXPRESSION  ${regexstring})

set(regName ${moduleName})
string (REPLACE "\\" "/" regName ${regName})
set(regexstring  ".+${regName}[^.{1}:]+.+hp?p?$")
set(groupName  "${moduleName}\\Header Files")
string (REPLACE "/" "\\" groupName ${groupName})
source_group(${groupName} REGULAR_EXPRESSION  ${regexstring})

set(regName ${moduleName})
string (REPLACE "\\" "/" regName ${regName})
set(regexstring  ".+${regName}[^.{1}:]+.+cpp$")
set(groupName  "${moduleName}\\Source Files")
string (REPLACE "/" "\\" groupName ${groupName})
source_group(${groupName} REGULAR_EXPRESSION  ${regexstring})

endfunction()
