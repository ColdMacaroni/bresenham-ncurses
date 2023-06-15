# Compiler and compiler flags
CC=gcc

# Show all the Warnings! May want to add -Werror as well.
CCFLAGS=-Wall -Wextra -pedantic

# Libraries to use
LIBS=

# Where to store the executable
BINDIR=bin

# Name of executable
BIN=bresenham-nc

# Directory with header files
INCLUDEDIR=include

# Directory with the .cpp files
SRCDIR=src

# Directory for object files (compiled)
OBJDIR=obj

SRCFILES=$(patsubst %,${SRCDIR}/%,main.c)
OBJFILES=$(patsubst ${SRCDIR}%.c,${OBJDIR}%.o,${SRCFILES})

# Make the executable, linking the compiled files together with the libraries
${BINDIR}/${BIN}: ${OBJFILES} ${BINDIR}
	${CC} ${CCFLAGS} ${OBJFILES} ${LIBS} -I${INCLUDEDIR} -o ${BINDIR}/${BIN}

${BINDIR}:
	mkdir -p ${BINDIR}

# Compile the files, no linking.
${OBJDIR}/%.o: ${SRCDIR}/%.c ${OBJDIR}
	${CC} ${CCFLAGS} ${OBJDIR} -c -o $@ $< -I${INCLUDEDIR}

${OBJDIR}:
	mkdir -p ${OBJDIR}

clean:
	rm -f ${OBJFILES}
	rm -f ${BINDIR}/${BIN}

