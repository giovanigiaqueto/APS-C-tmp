project(benchmark)
cmake_minimum_required(VERSION 3.10)

# especificação C11
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# pastas
set(SRCDIR "src")
set(INCDIR "inc")

# arquivos com código fonte e executável
add_executable(
    benchmark
    "${SRCDIR}/main.c"

    "${SRCDIR}/ler_arquivo.h"
    "${SRCDIR}/cmd_params.h"
    "${SRCDIR}/geracao_dados.h"
    "${SRCDIR}/algs_ordenacao.h"
    "${SRCDIR}/temporizacao.h"
    "${SRCDIR}/resultados.h"
    "${SRCDIR}/aleatorio.h"
)

# global compiler flags
set(CMAKE_C_FLAGS "-Wall -O3")

# set include dir
include_directories("${INCDIR}")

# configura as dependências e regras de compilação
include("deps/CMakeDeps.txt")

# regra de como linkar o main
target_link_libraries(
    benchmark
    ${MODULE_LIST}
)

# regra de instalação
install(
    TARGETS benchmark EXPORT benchmark-default
    DESTINATION ${CMAKE_SOURCE_DIR}
)