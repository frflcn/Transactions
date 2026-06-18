set(LIBREOFFICE_INCLUDE_DIRS 
    /home/trevor/Development/libreoffice26.2_sdk/inc
    /usr/lib/libreoffice/sdk/include)

set(LIBREOFFICE_LIB_DIRS
    /usr/lib/libreoffice/sdk/lib
    /usr/lib/libreoffice/program)

set(LIBREOFFICE_LIBS
    uno_cppuhelpergcc3
    uno_cppu
    uno_salhelpergcc3
    uno_sal)

target_compile_definitions(Transactions PRIVATE
    LINUX
    UNX
    GCC
    CPPU_ENV=gcc3
)