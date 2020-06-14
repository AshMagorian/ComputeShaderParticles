cmake_minimum_required(VERSION 2.6)
project(MYENGINE)

include_directories(
src
contrib
)

if(MSVC)
  include_directories(
    contrib/msvc-windows-x86/glew/include
    contrib/msvc-windows-x86/SDL2/include
    contrib/msvc-windows-x86/openal-soft/include
    contrib/msvc-windows-x86/ogg/include
    contrib/msvc-windows-x86/vorbis/include
  )

  link_directories(
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/SDL2/lib
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/glew/lib
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/openal-soft/lib
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/ogg/lib
    ${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/vorbis/lib
  )
endif()

if(MSVC)
  add_definitions(-DSDL_MAIN_HANDLED)
endif()	

add_library(CSEngine
	src/CSEngine/CSEngine.h
	src/CSEngine/ShaderProgram.h
	src/CSEngine/ShaderProgram.cpp
	src/CSEngine/Application.h
	src/CSEngine/Application.cpp
	src/CSEngine/Exception.h
	src/CSEngine/Exception.cpp
	src/CSEngine/ParticlesVA.h
	src/CSEngine/ParticlesVA.cpp
)

if(MSVC)
  target_link_libraries(CSEngine
    SDL2 glew32 opengl32 OpenAL32 ogg vorbis vorbisfile
  )
else()
  target_link_libraries(CSEngine
    SDL2 GLU GLEW GL AL ogg vorbis vorbisfile
  )
endif()

add_executable(game
	src/game/main.cpp
)

target_link_libraries(game CSEngine)

if(MSVC)
  add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/SDL2/bin/SDL2.dll"
      "$<TARGET_FILE_DIR:game>"
  )

  add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/glew/bin/glew32.dll"
      "$<TARGET_FILE_DIR:game>"
  )

  add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_SOURCE_DIR}/contrib/msvc-windows-x86/openal-soft/bin/OpenAL32.dll"
      "$<TARGET_FILE_DIR:game>"
  )
endif()