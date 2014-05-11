//==============================================================================
// Detours.cpp
//
// written by VectoR.360
//
// see http://research.microsoft.com/sn/detours/
//
//==============================================================================

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <windows.h>
#include <detours.h>

int main(int argc, char* argv[])
{
	char ip_buffer[40];
	char *ip = &ip_buffer[0];
	char dll[MAX_PATH];
	char log_path[MAX_PATH];
	char cmd_buffer[MAX_PATH];
	char *cmd = &cmd_buffer[0];
	char cur_dir[MAX_PATH+2];
	char *exe = "C:\\Program Files\\EA GAMES\\Earth & Beyond\\release\\client.exe";
    bool UseUDP = false;

	sprintf(ip, "127.0.0.1");
	GetCurrentDirectory(sizeof(dll),dll);
	strcpy(log_path, dll);
	strcat(dll, "\\ClientDetours.dll");
	strcat(log_path, "\\ClientDetours.log");

	for (int i = 1; i < argc; i++)
	{
		if (strncmp(argv[i], "/ADDR:", 6) == 0)
		{
			ip = argv[i] + 6;
		}
		else if (strncmp(argv[i], "/UDP", 4) == 0)
		{
			UseUDP = true;
            printf("Client using UDP\n");
		}
		else if (strncmp(argv[i], "/CLIENT:", 8) == 0)
		{
			exe = argv[i] + 8;
		}
	}

	_splitpath(exe, cur_dir, &cur_dir[2], NULL, NULL);
	cur_dir[1] = ':';

    if (UseUDP)
    {
		sprintf(cmd, "client.exe -SERVER_ADDR %s -PROTOCOL UDP", ip);
    }
    else
    {
		sprintf(cmd, "client.exe -SERVER_ADDR %s -PROTOCOL TCP", ip);
    }

    fprintf(stderr,"Attempting to connect to IP address: %s\n", ip);

	STARTUPINFO si = {NULL};
	PROCESS_INFORMATION pi = {NULL};

	SetCurrentDirectory(cur_dir);

	/* Create a file with the log path for ClientDetours.dll to read */
	FILE *f = fopen("DetoursLogPath.txt", "w");
	if (f)
	{
		fprintf(f, log_path);
		fclose(f);
	}

	fprintf(stderr,"Launching ENB with ClientDetours.dll\n");

	if (!DetourCreateProcessWithDll(exe,
									cmd, NULL, NULL, TRUE,
									CREATE_DEFAULT_ERROR_MODE, NULL, NULL,
									&si, &pi, dll, NULL)) {

		printf("launcher: DetourCreateProcessWithDll failed: %d\n", GetLastError());
	}

	return 0;
}
