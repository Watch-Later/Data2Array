set(D2A_EXECUTABLE Data2Array)

macro(ADD_TEMPLATES_IMPL GENERATED PARAMS)

    get_filename_component(OUTFILE ${GENERATED} ABSOLUTE)

    set(TEMPLATES)


    foreach (it ${ARGN})
        get_filename_component(N ${it} ABSOLUTE)
        list(APPEND TEMPLATES ${N})
    endforeach(it)
    
    get_filename_component(ONAME ${OUTFILE} NAME)


    add_custom_command(
	    OUTPUT  ${OUTFILE}
	    COMMAND ${D2A_EXECUTABLE} ${PARAMS} -O ${OUTFILE} -I ${TEMPLATES}
	    DEPENDS ${D2A_EXECUTABLE} ${TEMPLATES}
	    COMMENT "Converting ${ONAME}"
	    )

    set(${GENERATED} ${OUTFILE})

endmacro(ADD_TEMPLATES_IMPL)




macro(ADD_TEMPLATES GENERATED)
    add_templates_impl(${GENERATED} -B ${ARGN})
endmacro(ADD_TEMPLATES)

macro(ADD_TEMPLATES_TEXT GENERATED)
    add_templates_impl(${GENERATED} -f ${ARGN})
endmacro(ADD_TEMPLATES_TEXT)

