studentMgr:cgic.o cJSON.o ctemplate.o list.o studentMgr.o teacher.o netdisk.o
	gcc -o studentMgr.cgi $^ -L. -lm -g 
cgic.o:cgic.c
	gcc -c $<

cJSON.o:cJSON.c
	gcc -c $< -lm

ctemplate.o:ctemplate.c
	gcc -c $< -I . -g

list.o:list.c
	gcc -c $<

teacher.o:teacher.c
	gcc -c $<

netdisk.o:netdisk.c
	gcc -c $<

studentMgr.o:studentMgr.c
	gcc -c $<

clean:
	rm *.o -rf
