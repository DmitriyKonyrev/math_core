if(__FILTER_SOURCES_INCLUDED)
	return()
endif()
set(__FILTER_SOURCES_INCLUDED TRUE)


function(filter_sources filtred_list source_list regex)
	set(list)
	foreach(f ${${source_list}})
		string(REGEX MATCH "${regex}" qrc ${f})
		set(list ${list} ${qrc})
	endforeach()
	set(${filtred_list} ${list} PARENT_SCOPE)
endfunction()
