add_library(app  "") 

target_sources(app 
  PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/application.c
    ${CMAKE_CURRENT_LIST_DIR}/context/context.c
    ${CMAKE_CURRENT_LIST_DIR}/render/renderer.c
    ${CMAKE_CURRENT_LIST_DIR}/utils/utils.c
    ${CMAKE_CURRENT_LIST_DIR}/window/window.c
  PUBLIC  
    ${CMAKE_CURRENT_LIST_DIR}/application.h
    ${CMAKE_CURRENT_LIST_DIR}/headers.h
    ${CMAKE_CURRENT_LIST_DIR}/render/renderer.h
    ${CMAKE_CURRENT_LIST_DIR}/toolkit.h
    ${CMAKE_CURRENT_LIST_DIR}/context/context.h
    ${CMAKE_CURRENT_LIST_DIR}/utils/utils.h
    ${CMAKE_CURRENT_LIST_DIR}/window/window.h
)

target_include_directories(app
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )


