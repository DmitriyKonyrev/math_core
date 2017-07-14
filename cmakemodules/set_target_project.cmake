# Модуль для установки целевого проекта.
#
# На выбор есть: SOL и 14C, отличающиеся по составу поддерживаемых изделий

macro(set_target_project)

 set(KPAPNISO_PROJECT_TYPE "SOL" CACHE STRING "Type of target project") 
 set_property(CACHE KPAPNISO_PROJECT_TYPE PROPERTY STRINGS 
   "SOL" "14C" "DEV-ALL")
  #-------------------------------------------------------------------------------
  if(${KPAPNISO_PROJECT_TYPE} STREQUAL "SOL")
    set(3M14_PLUGIN_BUILD TRUE)
	set(3M54_PLUGIN_BUILD TRUE)
  endif()
  #-------------------------------------------------------------------------------
  if(${KPAPNISO_PROJECT_TYPE} STREQUAL "14C")
    set(3M14C_PLUGIN_BUILD TRUE)
  endif()
  #-------------------------------------------------------------------------------
  if(${KPAPNISO_PROJECT_TYPE} STREQUAL "DEV-ALL")
    set(ALL_PLUGIN_BUILD TRUE)
  endif()
endmacro(set_target_project)