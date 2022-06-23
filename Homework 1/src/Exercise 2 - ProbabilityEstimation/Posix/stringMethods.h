#include <stdbool.h>
#include <string.h>

void shiftLeft(char str[],int length){
  for(int i=0;i<length-1;i++){
    str[i] = str[i+1];
  }

  str[length-1] = ' ';
}

bool endsWith(char str1[],char str2[]){
  int length1 = strlen(str1);
  int length2 = strlen(str2);
  for(int i=1;i<=length2;i++){
    if(str1[length1-i]!=str2[length2-i]){
      return false;
    }
  }
  return true;
}

void cleanStr(char str[],int length){
  for(int i=0;i<length;i++){
    str[i]=' ';
  }
}