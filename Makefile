GCC=gcc
Cflags= -Werror -Wall 
thread= -lpthread


.PHONY: all clean

all: Main


Main: FabricationPlantManager.c AssemblyManager
	$(GCC) FabricationPlantManager.c -o Main $(Cflags) 
	

AssemblyManager: AssemblyManager.c
	$(GCC) AssemblyManager.c -o assembly  $(Cflags) $(thread)

run: Main
	./Main

clean: -rm Main


