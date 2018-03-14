#pragma once

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#define MAX_LEN 100
#define MAX_PATH 260
#define ID_LEN 7

// get key
char *GetIniKeyString(char *title,char *key,char *filename) 
{ 
	FILE *fp; 
	char szLine[1024];
	static char tmpstr[1024];
	int rtnval;
	int i = 0; 
	int flag = 0; 
	char *tmp;

	if((fp = fopen(filename, "r")) == NULL) 
	{ 
		printf("have   no   such   file \n");
		return ""; 
	}
	while(!feof(fp)) 
	{ 
		rtnval = fgetc(fp); 
		if(rtnval == EOF) 
		{ 
			break; 
		} 
		else 
		{ 
			szLine[i++] = rtnval; 
		} 
		if(rtnval == '\n') 
		{ 
			szLine[--i] = '\0';
			i = 0; 
			tmp = strchr(szLine, '='); 

			if(( tmp != NULL )&&(flag == 1)) 
			{ 
				if(strstr(szLine,key)!=NULL) 
				{ 
					// 
					if ('#' == szLine[0])
					{
					}
					else if ( '/' == szLine[0] && '/' == szLine[1] )
					{
						
					}
					else
					{
						//ÕÒ´òkey¶ÔÓ¦±äÁ¿
						strcpy(tmpstr,tmp+1); 
						fclose(fp);
						return tmpstr; 
					}
				} 
			}
			else 
			{ 
				strcpy(tmpstr,"["); 
				strcat(tmpstr,title); 
				strcat(tmpstr,"]");
				if( strncmp(tmpstr,szLine,strlen(tmpstr)) == 0 ) 
				{
					//ÕÒµ½title
					flag = 1; 
				}
			}
		}
	}
	fclose(fp); 
	return ""; 
}

