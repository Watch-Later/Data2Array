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



macro(ADD_TEMPLATES_EX TARGET)
    
    set(OUTFILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.h")

    include_directories(${CMAKE_CURRENT_BINARY_DIR})

    set(TEMPLATES)


    foreach (it ${ARGN})
        get_filename_component(N ${it} ABSOLUTE)
        list(APPEND TEMPLATES ${N})
    endforeach(it)
    
    get_filename_component(ONAME ${OUTFILE} NAME)


    add_custom_command(
	    OUTPUT  ${OUTFILE}
	    COMMAND ${D2A_EXECUTABLE} -B -O ${OUTFILE} -I ${TEMPLATES}
	    DEPENDS ${D2A_EXECUTABLE} ${TEMPLATES}
	    COMMENT "Converting ${ONAME}"
	    )

    set(${TARGET} ${OUTFILE})



endmacro(ADD_TEMPLATES_EX)


macro(ADD_TEMPLATES_EX_DIR DIR TARGET)
    
    set(OUTFILE "${DIR}/${TARGET}.h")

    include_directories(${DIR})
    set(TEMPLATES)

    foreach (it ${ARGN})
        get_filename_component(N ${it} ABSOLUTE)
        list(APPEND TEMPLATES ${N})
    endforeach(it)
    
    get_filename_component(ONAME ${OUTFILE} NAME)


    add_custom_command(
	    OUTPUT  ${OUTFILE}
	    COMMAND ${D2A_EXECUTABLE} -C -B -O ${OUTFILE} -I ${TEMPLATES}
	    DEPENDS ${D2A_EXECUTABLE} ${TEMPLATES}
	    COMMENT "Converting ${ONAME}"
	    )

    set(${TARGET} ${OUTFILE})



endmacro(ADD_TEMPLATES_EX_DIR)


macro(ADD_TEMPLATES GENERATED)
    add_templates_impl(${GENERATED} -B ${ARGN})
endmacro(ADD_TEMPLATES)

macro(ADD_TEMPLATES_TEXT GENERATED)
    add_templates_impl(${GENERATED} -f ${ARGN})
endmacro(ADD_TEMPLATES_TEXT)

