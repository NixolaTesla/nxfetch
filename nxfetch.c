#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <sys/statvfs.h>
#include <unistd.h>
#include <sys/wait.h>

char uptime[50];
char totalmem[50];
char usedmem[50];
char version[50];
char buffer[256];
int lines = 0;
char mem[10][256];
int count = 0;
int counting = 0;
unsigned long total;
unsigned long available;
unsigned long used;
int nicee = 0;
int nicee1 = 0;
char cpu[256];
char *part = "lsblk";
char distro[256];

int main(int argc, char *argv[]){

  struct utsname sys_info;
  struct statvfs buf;

  // V error checks V
  
  if(uname(&sys_info) == -1){
     perror("uname");
     return 1;
  }	

  if(statvfs("/", &buf) == -1){
     perror("statvfs");
     return 1;
  }


  FILE *fpos = fopen("/etc/os-release", "r");

  if(fpos != NULL){
	while(fgets(buffer, sizeof(buffer), fpos)){
	    if(sscanf(buffer, "NAME=\"%255[^\"]\"", distro) == 1){
		int nicer;
	    }	    
	}
  }

  fclose(fpos);

  printf("\nOS: %s", distro);

  printf("\nKernel: %s\n", sys_info.sysname);
  printf("Hostname: %s\n", sys_info.nodename); 

  FILE *fp1 = fopen("/proc/meminfo", "r");
  
  if(fp1 != NULL){
    while(fgets(buffer, sizeof(buffer), fp1)){

       if(sscanf(buffer, "MemTotal: %lu kB", &total) == 1){
          // extracted total
	  nicee = 1; 
       }

       if(sscanf(buffer, "MemAvailable: %lu kB", &available) == 1){
          // extracted available
	  nicee1 = 1;
       }
    }
  }

  fclose(fp1);

  used = total - available;
  float used_gb = used / 1024.0 / 1024.0;
  float total_gb = total / 1024.0 / 1024.0;

  printf("Memory: %.1f GB / %.1f GB\n", used_gb, total_gb);

  double uptime;

  FILE *fpup = fopen("/proc/uptime", "r");

  if(fpup != NULL){
	fscanf(fpup, "%lf", &uptime);
	fclose(fpup);
	int days = uptime / 86400;
	int hours = ((int)uptime % 86400) / 3600;
	int minutes = ((int)uptime % 3600) / 60;

	printf("Uptime: %d days, %d hours, %d minutes\n", days, hours, minutes);
  }

  FILE *fp2 = fopen("/proc/cpuinfo", "r");

  if(fp2 != NULL){
    while (fgets(buffer, sizeof(buffer), fp2)){
    if (sscanf(buffer, "model name : %[^\n]", cpu) == 1){
    break;
   }
  }

    fclose(fp2);
} 

  printf("CPU: %s\n", cpu);
  printf("\n ");
  pid_t pid = fork();

  if(pid == 0){
    execlp("lsblk", "lsblk", NULL);
  }

  else if(pid > 0){
     waitpid(pid, NULL, 0);
  }

  printf("\n");

  return 0;
}
