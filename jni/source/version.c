#include <stdio.h>
#include <string.h>
#include "interfaceapi.h"
#include "version.h"

char Major[]="2.";
char Minor[]="4.";
char Fix[]="4";

int version_get_DM_version(char *version){
  int buffsize = sizeof(Major)+sizeof(Minor)+sizeof(Fix);
  memset(version,0,buffsize);
  strcat(version,Major);
  strcat(version,Minor);
  strcat(version,Fix);
  return 0;
}
