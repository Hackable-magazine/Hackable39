# Version minimum de CMake
cmake_minimum_required(VERSION 3.12)

# Importation du SDK
# A faire avant d'appeler project()
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Nom du projet
project(pwmtest)

# Définition répertoire
add_definitions("-DMY_SOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\"")
# Définition nom de projet
add_definitions("-DGIT_REPO=\"${CMAKE_PROJECT_NAME}\"")
# Ajout chemin de recherche de modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")
# Ajout module
include(GetGitRevisionDescription)
# Récupération du SHA1
get_git_head_revision(GIT_REFSPEC GIT_SHA1)
# Définition Macro
add_definitions("-DGIT_SHA1=\"${GIT_SHA1}\"")

# Initialisation SDK
pico_sdk_init()

# sources
set(SOURCE_FILES main.c)

# binaire et sources
add_executable(${CMAKE_PROJECT_NAME}
  ${SOURCE_FILES}
)

# Ajout pico_stdlib pour les fonctions courantes
target_link_libraries(${CMAKE_PROJECT_NAME}
  pico_stdlib
  hardware_pwm
)

# enable usat output, disable usb output
pico_enable_stdio_usb(${CMAKE_PROJECT_NAME} 0)
pico_enable_stdio_uart(${CMAKE_PROJECT_NAME} 1)

# Création des fichiers dis/bin/hex/uf2
pico_add_extra_outputs(${CMAKE_PROJECT_NAME})

# "make clean" doit aussi supprimer map/dis/bin/hex/uf2
set_directory_properties(PROPERTIES
   ADDITIONAL_MAKE_CLEAN_FILES
   "${CMAKE_PROJECT_NAME}.bin;${CMAKE_PROJECT_NAME}.dis;\
${CMAKE_PROJECT_NAME}.elf.map;${CMAKE_PROJECT_NAME}.hex;\
${CMAKE_PROJECT_NAME}.uf2"
)