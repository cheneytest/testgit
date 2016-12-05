clone_detect: conver.o clone_check.o readir.o compare.o json.o clone_detect.o
	gcc -g -o clone_detect clone_check.o conver.o readir.o compare.o json.o clone_detect.o
conver.o:
	gcc -g -c conver.c  
clone_check.o:
	gcc -g -c clone_check.c 
readir.o:
	gcc -g -c readir.c      
compare.o:
	gcc -g -c compare.c
json.o:
	gcc -g -c json.c
clone_detect.o:
	gcc -g -c clone_detect.c
clean:
	rm *.o clone_detect  