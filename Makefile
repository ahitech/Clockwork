NAME := Clockwork
TYPE := APP
APP_MIME_SIG := "application/x-vnd.clockwork-app"
TARGET_DIR := .

SRCS = \
    src/core/HebrewDate.cpp \
    src/core/ResizableBox.cpp	\
    src/modules/TodayModule.cpp \
    src/main.cpp

LIBS = 	hdate \
		be 	\
		stdc++ \
		supc++ \
		localestub	\
		translation \
		root	\
		intl
		
LIBPATHS =	boot/system/develop/lib	\
			lib
		   
#	Additional paths to look for system headers. These use the form
#	"#include <header>". Directories that contain the files in SRCS are
#	NOT auto-included here.
SYSTEM_INCLUDE_PATHS =  \
		boot/system/develop/headers/be		\
		boot/system/develop/headers/cpp 	\
		boot/system/develop/headers/posix
		   
LOCAL_INCLUDE_PATHS =   src/core		\
						lib				\
						src/modules
LOCALES = en he ru eo

#	Specify the level of optimization that you want. Specify either NONE (O0),
#	SOME (O1), FULL (O2), or leave blank (for the default optimization level).
OPTIMIZE := NONE

#	Specify the resource definition files to use. Full or relative paths can be
#	used.
RDEFS := Genio.rdef

#	Specify the resource files to use. Full or relative paths can be used.
#	Both RDEFS and RSRCS can be utilized in the same Makefile.
RSRCS =

SYMBOLS := TRUE

DEBUGGER := TRUE

COMPILER_FLAGS := -fpermissive -std=c++20

SYMBOLS := TRUE

CATKEYS = locales/Clockwork.catkeys
CATALOGS = Clockwork.catalog


## Include the Makefile-Engine
DEVEL_DIRECTORY := \
        $(shell findpaths -r "makefile_engine" B_FIND_PATH_DEVELOP_DIRECTORY)
include $(DEVEL_DIRECTORY)/etc/makefile-engine
