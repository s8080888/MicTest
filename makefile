source := main.c TestAPI.c
# source := condtion_variable.c
Com = c11 -pthread
compile := gcc
obj = ${source:%.c=%.o}

do:main
	./main.exe
main:${obj}
	${compile} -std=${Com} ${obj}  -o main
%.o:%.c
	${compile} -std=${Com} $< -c 
.PHONY:clean
clean:
	@rm -rf *.o *.exe main.exe 
