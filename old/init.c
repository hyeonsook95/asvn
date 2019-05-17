#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

/*
 /home/h/.asvn--conf --authz
                      -passwd
               -repos              
*/

int main()
{
  char *path = "";
  char orgpath[1024];  //HOME path

  memset(orgpath, '\0', 1024);

  if((path = getenv("HOME")) == NULL) {
    fprintf(stderr, "Current working directory get error: %s\n", strerror(errno));
    return -1;
  }

  strcat(path, "/.asvn");
  strcpy(orgpath, path);

  if(mkdir(path, 0777) == -1) {
    fprintf(stderr, "%s directory make error: %s\n", path, strerror(errno));
    return -1;
  }
  

  /* .asvn/conf folder */
  strcat(path, "/conf");
  if(mkdir(path, 0777) == -1) {
    fprintf(stderr, "%s directory make error: %s\n", path, strerror(errno));
    return -1;
  }

  /* .asvn/conf/authz */
  strcpy(path, orgpath);
  strcat(path, "/conf/authz");
  if(creat(path, 0644) == -1) {
    fprintf(stderr, "%s file make error: %s\n", path, strerror(errno));
    return -1;
  }

  /* .asvn/conf/passwd */
  strcpy(path, orgpath);
  strcat(path, "/conf/passwd");
  if(creat(path, 0644) == -1) {
    fprintf(stderr, "%s file make error: %s\n", path, strerror(errno));
    return -1;
  }

  /* .asvn/repos file */
  strcpy(path, orgpath);
  strcat(path, "/repos");
  if(creat(path, 0644) == -1) {
    fprintf(stderr, "%s folder make error: %s\n", path, strerror(errno));
    return -1;
  }

  /* .asvn/.asubversion */
  strcpy(path, orgpath);
  strcat(path, "/.asubversion");
  if(mkdir(path, 0777) == -1) {
    fprintf(stderr, "%s directory make error: %s\n", path, strerror(errno));
    return -1;
  }

}
