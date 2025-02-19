### SOURCE FILES: add all model files here ###
set(SOURCE_FILES
        common.h
        donthcomp.c
        Relbase.cpp Relbase.h
        Relcache.cpp Relcache.h
        Rellp.cpp Rellp.h
        reltable.c reltable.h
        relutility.c relutility.h
        xilltable.c xilltable.h
        Relphysics.cpp Relphysics.h
        writeOutfiles.c writeOutfiles.h
        Relprofile.cpp Relprofile.h
        Relxill.cpp Relxill.h
        ModelDatabase.h
        LocalModel.cpp LocalModel.h
        ModelParams.cpp ModelParams.h
        XspecSpectrum.h
        ModelInfo.h
        Relreturn_BlackBody.cpp Relreturn_BlackBody.h
        IonGradient.cpp IonGradient.h
        Relreturn_Corona.cpp Relreturn_Corona.h
        Relreturn_Datastruct.cpp Relreturn_Datastruct.h
        Relreturn_Table.cpp Relreturn_Table.h
        xspec_wrapper_lmodels.cpp xspec_wrapper_lmodels.h   #are created by the wrapper script
        Xillspec.cpp Xillspec.h
        PrimarySource.cpp PrimarySource.h
        )
############################################

set(CONFIG_FILE ${PROJECT_BINARY_DIR}/config.h)
set(SOURCE_FILES ${SOURCE_FILES} ${CONFIG_FILE})

set(EXEC_FILES_CPP test_sta)

find_package(PkgConfig REQUIRED)
pkg_check_modules(cfitsio REQUIRED IMPORTED_TARGET cfitsio)
pkg_check_modules(fftw3 REQUIRED IMPORTED_TARGET fftw3)

foreach (execfile ${EXEC_FILES_CPP})
    add_executable(${execfile} ${execfile}.cpp ${SOURCE_FILES} ${CONFIG_FILE} )
    target_link_libraries(${execfile} PkgConfig::cfitsio PkgConfig::fftw3)
    target_include_directories(${execfile} PUBLIC "${PROJECT_BINARY_DIR}" "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")  # necessary to find config file
endforeach (execfile ${EXEC_FILES_CPP})

### RELXILL LIBRARY ###
set(LIBNAME Relxill)
add_library(${LIBNAME} ${SOURCE_FILES} ${CONFIG_FILE})
target_include_directories(${LIBNAME} PUBLIC "${PROJECT_BINARY_DIR}" "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(${LIBNAME} PkgConfig::cfitsio PkgConfig::fftw3)
########################



# create everything in the real source directly (and not as default the build)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

### Target: LMODELDAT ###
set(LMODEL_NAME lmodel_relxill.dat)
if (DEFINED ENV{RELXILL_STABLE})
    add_custom_command(
            OUTPUT ${LMODEL_NAME}
            COMMAND ${CMAKE_COMMAND} -E copy ${SRC_DIR}/modelfiles/lmodel_relxill_public.dat ${LMODEL_NAME}
            COMMAND ${CMAKE_COMMAND} -E echo " ****** Creating lmodel_relxill.dat ONLY for STABLE RELEASE MODELS ******"
            DEPENDS ${SRC_DIR}/modelfiles/lmodel_relxill_public.dat
            USES_TERMINAL
    )

else()
    add_custom_command(
            OUTPUT ${LMODEL_NAME}
            COMMAND ${CMAKE_COMMAND} -E copy ${SRC_DIR}/modelfiles/lmodel_relxill_public.dat ${LMODEL_NAME}
            COMMAND ${CMAKE_COMMAND} -E cat ${SRC_DIR}/modelfiles/lmodel_relxill_devel.dat >> ${LMODEL_NAME}
            COMMAND ${CMAKE_COMMAND} -E echo " ****** Creating lmodel_relxill.dat for ALL MODELS ******"
            DEPENDS ${SRC_DIR}/modelfiles/lmodel_relxill_public.dat
            DEPENDS ${SRC_DIR}/modelfiles/lmodel_relxill_devel.dat
            USES_TERMINAL
    )
endif()

add_custom_target(lmodeldat ALL DEPENDS ${LMODEL_NAME})
#############################


find_package(Python REQUIRED Interpreter)

### Target: XSPEC_WRAPPER ###
add_custom_command(
        OUTPUT xspec_wrapper_lmodels.cpp xspec_wrapper_lmodels.h
        COMMAND Python::Interpreter ${SRC_DIR}/create_wrapper_xspec.py
        ${LMODEL_NAME} xspec_wrapper_lmodels.cpp
        COMMAND echo `pwd`
        DEPENDS lmodeldat ${SRC_DIR}/create_wrapper_xspec.py ${LMODEL_NAME}
        )

add_custom_target(xspec_wrapper ALL DEPENDS xspec_wrapper_lmodels.cpp xspec_wrapper_lmodels.h)
#############################




###  INSTALL  ###
install(FILES ${SOURCE_FILES} DESTINATION model)
install(FILES modelfiles/compile_relxill.sh ${LMODEL_NAME} DESTINATION model)

install(TARGETS ${EXEC_FILES_CPP} DESTINATION bin)




