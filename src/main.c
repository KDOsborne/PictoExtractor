#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "sqlite/sqlite3.h"

FILE 	*fp;
char	query[500];

static int callback_trial(void *data, int argc, char **argv, char **azColName)
{	
	for(int i = 0; i < argc; i++)
	{
		if(i == argc - 1)
			fprintf(fp,"%s\n",argv[i]);
		else
			fprintf(fp,"%s, ",argv[i]);
	}
	
	return 0;
}

void process_sql(char *database, char *savefile)
{
	sqlite3	*db;
	char	*zErrMsg = 0;
	int		rc;

	//Open the database
	rc = sqlite3_open(database, &db);
	if (rc)
	{
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
		return;
	}

	fp = fopen(savefile, "w");
	fprintf(fp, "Timestamp, Aligncode, Aligneventnumber\n");

	rc = sqlite3_exec(db, query, callback_trial, NULL, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		fprintf(stderr, "SQL error: %s\n%s\n", zErrMsg, query);
		sqlite3_free(zErrMsg);
	}

	fclose(fp);
	sqlite3_close(db);
}

int main(void)
{
	char filename[100], savefile[100];
	OPENFILENAME ofn, sfn;	
	
	//Select Database File
	memset(&ofn, 0, sizeof(ofn));
	memset(filename, 0, sizeof(filename));
	
	ofn.lStructSize      = sizeof(ofn);
	ofn.hwndOwner        = NULL;
	ofn.lpstrFilter      = "SQLITE File (*.sqlite)\0*.sqlite";
	ofn.nFilterIndex     = 1;
	ofn.lpstrFile        = filename;
	ofn.nMaxFile         = sizeof(filename);
	ofn.lpstrTitle       = "Select a File To Open";
	ofn.lpstrInitialDir  = ".";
	ofn.Flags            = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	if (!GetOpenFileName(&ofn))
	{
		fprintf(stderr, "OFN ERROR: 0x%lx\nPRESS ENTER TO EXIT", CommDlgExtendedError());
		getchar();
		return 0;
	}
	
	//Select Save File
	memset(&sfn, 0, sizeof(sfn));
	memset(savefile, 0, sizeof(savefile));
	
	sfn.lStructSize      = sizeof(sfn);
	sfn.hwndOwner        = NULL;
	sfn.lpstrFilter      = "Text File (*.txt)\0*.txt";
	sfn.nFilterIndex     = 1;
	sfn.lpstrFile        = savefile;
	sfn.nMaxFile         = sizeof(savefile);
	sfn.lpstrTitle       = "Save";
	sfn.lpstrInitialDir  = ".";
	sfn.Flags            = OFN_PATHMUSTEXIST;
	
	if (!GetSaveFileName(&sfn))
	{
		fprintf(stderr, "OFN ERROR: 0x%lx\nPRESS ENTER TO EXIT", CommDlgExtendedError());
		getchar();
		return 0;
	}
	
	if(strstr(savefile, ".txt") == NULL)
		strcat(savefile, ".txt");
	
	//Remove this and add an input statement if you want to do a custom query
	sprintf(query, "SELECT timestamp, aligncode, aligneventnumber FROM behavioralalignevents");
	
	printf("PROCESSING FILE: %s\n", filename);
	process_sql(filename, savefile);
	printf("FINISHED\nPRESS ENTER TO EXIT");
	
	getchar();
	
    return 0;
}
