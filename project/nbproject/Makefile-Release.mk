#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/Algorithm.o \
	${OBJECTDIR}/src/Problem.o \
	${OBJECTDIR}/src/Properties.o \
	${OBJECTDIR}/src/Solution.o \
	${OBJECTDIR}/src/UMApHLP_Benders.o \
	${OBJECTDIR}/src/UMApHLP_Model.o \
	${OBJECTDIR}/src/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m64 -fopenmp -lpthread
CXXFLAGS=-m64 -fopenmp -lpthread

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/opt/gurobi702/linux64/lib

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/project

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/project: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/project ${OBJECTFILES} ${LDLIBSOPTIONS} -lgurobi_c++ -lgurobi70 -lm -lpthread

${OBJECTDIR}/src/Algorithm.o: src/Algorithm.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -D_GLIBCXX_USE_CXX11_ABI=0 -I/opt/gurobi702/linux64/include -Ilib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Algorithm.o src/Algorithm.cpp

${OBJECTDIR}/src/Problem.o: src/Problem.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -D_GLIBCXX_USE_CXX11_ABI=0 -I/opt/gurobi702/linux64/include -Ilib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Problem.o src/Problem.cpp

${OBJECTDIR}/src/Properties.o: src/Properties.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -D_GLIBCXX_USE_CXX11_ABI=0 -I/opt/gurobi702/linux64/include -Ilib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Properties.o src/Properties.cpp

${OBJECTDIR}/src/Solution.o: src/Solution.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -D_GLIBCXX_USE_CXX11_ABI=0 -I/opt/gurobi702/linux64/include -Ilib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/Solution.o src/Solution.cpp

${OBJECTDIR}/src/UMApHLP_Benders.o: src/UMApHLP_Benders.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -D_GLIBCXX_USE_CXX11_ABI=0 -I/opt/gurobi702/linux64/include -Ilib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/UMApHLP_Benders.o src/UMApHLP_Benders.cpp

${OBJECTDIR}/src/UMApHLP_Model.o: src/UMApHLP_Model.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -D_GLIBCXX_USE_CXX11_ABI=0 -I/opt/gurobi702/linux64/include -Ilib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/UMApHLP_Model.o src/UMApHLP_Model.cpp

${OBJECTDIR}/src/main.o: src/main.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -D_GLIBCXX_USE_CXX11_ABI=0 -I/opt/gurobi702/linux64/include -Ilib -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/main.o src/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
