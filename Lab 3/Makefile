#Define source file and compiler
objects = hello-main.o hello-funct1.o hello-funct2.o
CC= gcc -pg

#Compiler flags for compilation and debugging
CFlag = -c -g

#Regular and debug targets
Target = helloworld
DTarget = helloworld-d

#Define name of text file
Text = helloworld.txt

#Default target is helloworld
all: $(Target)

#Build helloworld target
$(Target): $(objects)
	$(CC) $(objects) -o $(Target)
	
#Build object files from source files
%.o : %.c
	$(CC) $(CFlag) $<	

#Build helloworld-d target
$(DTarget): $(objects)
	$(CC) -pg $(objects) -o $(DTarget)
	
debug: $(DTarget)

#Clean up generated files
clean:
	rm -f *.o $(Target) $(DTarget)

#Generate gprof output
gpp:
	gprof $(Target) gmon.out > $(Text)
