set(D2A_EXECUTABLE Data2Array)

# ----------------------------------------------------------------------------
#  Usage:
#
#  add_templates(some_file_name.h templ1.ext templ2.ext ... etc)
#
#  set(SRCFILES 
#    some_file_name.h
#    ...
#  )
#  add_executable(SomeTarget ${SRCFILES})
#
macro(ADD_TEMPLATES GENERATED)

    get_filename_component(OUTFILE ${GENERATED} ABSOLUTE)

    set (TEMPLATES )
    foreach (it ${ARGN})
        get_filename_component(N ${it} ABSOLUTE)
        list(APPEND ${TEMPLATES} ${N})
    endforeach(it)

    get_filename_component(ONAME ${OUTFILE} NAME)
    add_custom_command(
	    OUTPUT  ${OUTFILE}
	    COMMAND ${D2A_EXECUTABLE} -O ${OUTFILE} -T -B  -I ${TEMPLATES}
	    DEPENDS ${D2A_EXECUTABLE}   ${TEMPLATES}
	    COMMENT "Building ${ONAME}"
	    )

    set(${GENERATED} ${OUTFILE})

    unset(TEMPLATES )
    unset(OUTFILE )
    unset(ONAME )
endmacro(ADD_TEMPLATES)

