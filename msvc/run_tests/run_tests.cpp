// run_tests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <sys/stat.h>

static int TestDirectory(char *dir, char *picoc);
static int TestFile(char *fileName, char *picoc);
static int ValidateResults(char *expect, char *output);
static char *LoadFile(char *fileName);
static char *LoadFile(char *fileName);

int main(int argc, char *argv[])
{
	int errorCount;
	char testDirFullPath[MAX_PATH], picocFullPath[MAX_PATH];

	if (argc != 3)
	{
		puts("Usage: run_tests <dir> <picoc executable>\r\n\r\n"
			 "Every .c file in <dir> (and its subdirectories) will be run through PicoC,\r\n"
			 "whose executable path is specified in the second argument. The output will\r\n"
			 "be compared with the corresponding .expect file.");
		exit(-1);
	}

	errorCount = TestDirectory(_fullpath(testDirFullPath, argv[1], sizeof(testDirFullPath)), 
		_fullpath(picocFullPath, argv[2], sizeof(picocFullPath)));

	if (errorCount == 0)
		puts("All tests passed");

	puts("Hit the Enter key to exit...");
	getchar();

	return errorCount;
}

int TestDirectory(char *dir, char *picoc)
{
	char pattern[MAX_PATH];
	WIN32_FIND_DATA findData;
	HANDLE h;
	int errorCount = 0;
	int dirLen = strlen(dir);

	// Make sure the dir has a trailing slash, then add "*.*" to it.
	strcpy(pattern, dir);
	if (pattern[dirLen - 1] != '\\' && pattern[dirLen - 1] != '/')
	{
		strcpy(pattern + dirLen, "\\*.*");
		++dirLen;
	}
	else
	{
		strcpy(pattern + dirLen, "*.*");
	}

	// Go through all the files in the directory
	h = FindFirstFile(pattern, &findData);
	if (h != INVALID_HANDLE_VALUE)
	{
		do
		{
			// If we find a subdirectory, recurse into it
			if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
			{
				if (findData.cFileName[0] != '.')
				{
					strcpy(pattern + dirLen, findData.cFileName);
					errorCount += TestDirectory(pattern, picoc);
				}
			}
			// Else (not a subdirectory)
			else
			{
				// If it's a .c file
				int fileNameLen = strlen(findData.cFileName);
				if (_stricmp(findData.cFileName + fileNameLen - 2, ".c") == 0)
				{
					// Run it through PicoC and check the results
					strcpy(pattern + dirLen, findData.cFileName);
					errorCount += TestFile(pattern, picoc);
				}
			}

		} while (FindNextFile(h, &findData));
		FindClose(h);
	}

	return errorCount;
}

int TestFile(char *fileName, char *picoc)
{
	char commandLine[MAX_PATH * 3 + 50];
	char outputFileName[MAX_PATH + 2];
	STARTUPINFO startupInfo = {sizeof(startupInfo)};
	PROCESS_INFORMATION processInfo = {0};
	SECURITY_ATTRIBUTES sa = {sizeof(sa), NULL, TRUE};
	int errorCount = 0;

	printf("Testing %s\r\n", fileName);

	// Set up the output streams for PicoC
	strcpy(outputFileName, fileName);
	strcpy(strrchr(outputFileName, '.'), ".output");
	startupInfo.dwFlags = STARTF_USESTDHANDLES;
	startupInfo.hStdOutput = startupInfo.hStdError = CreateFile(outputFileName, GENERIC_WRITE, FILE_SHARE_WRITE, &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	// Execute the command
	sprintf(commandLine, "\"%s\" \"%s\"", picoc, fileName);
	if (strstr(fileName, "args"))
		strcat(commandLine, " - arg1 arg2 arg3 arg4");
	if (CreateProcess(NULL, commandLine, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &startupInfo, &processInfo))
	{
		DWORD exitCode;

		WaitForSingleObject(processInfo.hProcess, INFINITE);
		GetExitCodeProcess(processInfo.hProcess, &exitCode);
		CloseHandle(processInfo.hProcess);
		CloseHandle(processInfo.hThread);
		CloseHandle(startupInfo.hStdOutput);

		if (exitCode == 0)
		{
			char expectedFileName[MAX_PATH];
			strcpy(expectedFileName, fileName);
			strcpy(strrchr(expectedFileName, '.'), ".expect");
			errorCount = ValidateResults(expectedFileName, outputFileName);
		}
		else
		{
			printf("PicoC exited with code %d\r\n", exitCode);
			errorCount = 1;
		}
	}
	else
	{
		CloseHandle(startupInfo.hStdOutput);
		printf("Unable to run %s\r\n", picoc);
		errorCount = 1;
	}

	return errorCount;
}

int ValidateResults(char *expect, char *output)
{
	int errorCount = 0;

	char *expectedContents = LoadFile(expect);
	char *outputContents = LoadFile(output);

	if (strcmp(expectedContents, outputContents) != 0)
	{
		puts("Generated:");
		puts(outputContents);
		puts("Expected:");
		puts(expectedContents);
		errorCount = 1;
	}

	free(expectedContents);
	free(outputContents);

	return errorCount;
}

char *LoadFile(char *fileName)
{
	FILE *file = fopen(fileName, "rt");
	struct stat fileStat;
	size_t read;
	char *contents;

	stat(fileName, &fileStat);
	contents = (char *) malloc(fileStat.st_size + 1);
	read = fread(contents, 1, fileStat.st_size, file);
	contents[read] = '\0';
	fclose(file);
	return contents;
}
