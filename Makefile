NAME				=	noisegen
INI_DIR				=	/root/php-build-scripts/bin/php8/bin/php.ini
EXTENSION_DIR       =   $(shell php-config --extension-dir)
EXTENSION 			=	${NAME}.so
INI 				=	${NAME}.ini
COMPILER			=	g++
LINKER				=	g++
COMPILER_FLAGS		=	-Wall -c -O2 -std=c++11 -fpic -o
LINKER_FLAGS		=	-shared
LINKER_DEPENDENCIES	=	-lphpcpp 
RM					=	rm -f
CP					=	cp -f
MKDIR				=	mkdir -p
SOURCES				=	$(wildcard *.cpp)
OBJECTS				=	$(SOURCES:%.cpp=%.o)

#
#	From here the build instructions start
#

all:					${OBJECTS} ${EXTENSION}

${EXTENSION}:			${OBJECTS}
						${LINKER} ${LINKER_FLAGS} -o $@ ${OBJECTS} ${LINKER_DEPENDENCIES}

${OBJECTS}:
						${COMPILER} ${COMPILER_FLAGS} $@ ${@:%.o=%.cpp}

install:		
						${CP} ${EXTENSION} ${EXTENSION_DIR}
						${CP} ${INI} ${INI_DIR}
				
clean:
						${RM} ${EXTENSION} ${OBJECTS}

