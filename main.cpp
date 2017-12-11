#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace std;

int getProcIdByName(string procName)
{
    int pid = -1;

    DIR *dp = opendir("/proc");
    if (dp != NULL)
    {
        struct dirent *dirp;
        while (pid < 0 && (dirp = readdir(dp)))
        {
            int id = atoi(dirp->d_name);
            if (id > 0)
            {
                string cmdPath = string("/proc/") + dirp->d_name + "/cmdline";
                ifstream cmdFile(cmdPath.c_str());
                string cmdLine;
                getline(cmdFile, cmdLine);
                if (!cmdLine.empty())
                {
                    size_t pos = cmdLine.find('\0');
                    if (pos != string::npos)
                        cmdLine = cmdLine.substr(0, pos);
                    pos = cmdLine.rfind('/');
                    if (pos != string::npos)
                        cmdLine = cmdLine.substr(pos + 1);
                    if (procName == cmdLine)
                        pid = id;
                }
            }
        }
    }

    closedir(dp);

    return pid;
}

 
int HEX_TO_DEC(char st[10])
{
    long i=0,s,k,p;
    s=0;	
    p=strlen(st)-1;
    while( st[i]!='\0')
    {
        switch (toupper(st[i]))
        {
        case 'A': k=10; break;
        case 'B': k=11; break;
        case 'C': k=12; break;
        case 'D': k=13; break;
        case 'E': k=14; break;
        case 'F': k=15; break;
        case '1': k=1; break;
        case '2': k=2; break;
        case '3': k=3; break;
        case '4': k=4; break;
        case '5': k=5; break;
        case '6': k=6; break;
        case '7': k=7; break;
        case '8': k=8; break;
        case '9': k=9; break;
        case '0': k=0; break;
        }
        s=s+k*pow(16,p);
        p--;
	i++;
    }
    cout<< s;
 return 0;
}

int main(int argc, char* argv[])
{
	if((argc == 2) && (strcmp(argv[1], "/?") == 0))
	{
		printf("Автор: Константинов Семен би-41\n");
		printf("Программа для вывода списка всех открытых сокетов указанного процесса\n");
	}
	else if(argc == 2)
	{
	int pid = getProcIdByName(argv[1]);
	cout << "Process name: " << argv[1] << endl;
	cout << "pid: " << pid << endl << endl;
	FILE *file;

	char b[10];
	sprintf(b, "%i", pid);
	char str[190];
	char local_adr[10];
	char local_adr2[10];
	char remote_adr[10];
	char remote_adr2[10];
	char state[10];
	char put[100];
	char path[100];
	strcpy(path, "/proc/");
	strcat(path, b);
	strcat(path, "/net/tcp");
	strcpy(put, path);
	file = fopen(put, "r");
	fgets (str, 190, file);
	while (fgets (str, 190, file) != NULL)
	{
		for(int j = 34; j<=35; j++)
		{
			state[j-34] = str[j];
		}
		if(strcmp(state, "01")==0)
		{
			cout << "Local_address: ";
			for(int j = 6; j<=13; j++)
			{
				local_adr[j-6] = str[j];
			}
			HEX_TO_DEC(local_adr);
			cout << ":";		
			for(int j = 15; j<=18; j++)
			{
				local_adr2[j-15] = str[j];
			}
			HEX_TO_DEC(local_adr2);		
			cout << endl << "Remote address: ";
			for(int j = 20; j<=27; j++)
			{
				remote_adr[j-20] = str[j];
			}
			HEX_TO_DEC(remote_adr);
			cout << ":";
			for(int j = 29; j<=32; j++)
			{
				remote_adr2[j-29] = str[j];
			}
			HEX_TO_DEC(remote_adr2);
			cout << endl;
			cout << "Type: TCP" << endl << "State: ESTABLISHED" << endl << endl;			
		}	
	}    	
	fclose(file);
	strcpy(path, "/proc/");
	strcat(path, b);
	strcat(path, "/net/udp");
	strcpy(put, path);
	file = fopen(put, "r");
	fgets (str, 190, file);
	while (fgets (str, 190, file) != NULL)
	{
		for(int j = 35; j<=36; j++)
		{
			state[j-35] = str[j];
		}
		if(strcmp(state, "07")==0)
		{
			cout << "Local_address: *";
			cout << ":";		
			for(int j = 16; j<=19; j++)
			{
				local_adr2[j-16] = str[j];
			}
			HEX_TO_DEC(local_adr2);		
			cout << endl;
			cout << "Type: UDP" << endl << endl;			
		}	
	}    
	fclose(file);
	}
	else
		cout << "Error!" << endl;	
    return 0;
}
