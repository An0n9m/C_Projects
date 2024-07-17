#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>

const char*  KEY = "]!xc!W1(RLhY!>";

int main(int argc, char* argv[]){
    if(argc<2){
        printf("Usage: %s -p (print out decrypted bytes), -r (replace source file) <string (file address)> <password (optional)>\n",argv[0]);
        return 1;
}
    else if(argc==4){
        KEY=argv[3];	
}
    else if(argc>4){
	perror("Too many arguments\n");
	return 2;
}

short keysize=strlen(KEY);
const char* file=argv[2];
int fd=open(file,O_RDONLY);
if(fd==-1){
	perror("Can not open file or wrong file name ");
	return 3;
}
struct stat file_info;

if (fstat(fd, &file_info) == -1) { //Getting File Size
    perror("fstat");
    return 4;
}

long long file_size = file_info.st_size;
char *buf=(char *)malloc(file_size*sizeof(char));
if(buf==0){
	printf("Can not allocate: %lld memory\n",file_size);
}

long long size=read(fd,buf,file_size); //Reading File
printf("File size(decrypted): %lld bytes\n", file_size);
close(fd);

for(int i=0;i<size;i++)buf[i]=buf[i]^KEY[i%keysize]; //Xoring With Our Key						  

if(strcmp(argv[1],"-r")==0){
fd=open(file,O_CREAT | O_WRONLY, 0644);
if(fd==-1){
	perror("Can not write to file ");
	free(buf);
	return 5;
}
write(fd,buf,size);
close(fd);
}
else if(strcmp(argv[1],"-p")==0){
for (int i=0;i<size;i++)printf("%c",buf[i]);
}
free(buf);
return 0; //Written by An0n9m
}

