#pragma once
/* ----- user Base Class ----- */

#include <cstring>
#include <cstdio>
#include "api/ctp/ThostFtdcUserApiDataType.h"

class CustomUser 
{
protected:
	TThostFtdcBrokerIDType sBrokerID;         // broker ID
	TThostFtdcInvestorIDType sInvesterID;     // user ID
	TThostFtdcPasswordType sInvesterPassword; // user password

	char dataDirPath[100];                    // data store directory's path
	FILE *fout;
	FILE *ferr;

public:
	CustomUser(
		TThostFtdcBrokerIDType sBrokerID,
		TThostFtdcInvestorIDType sInvesterID,
		TThostFtdcPasswordType sInvesterPassword,
		char *dataDirPath)
	{
		strcpy(this->sBrokerID, sBrokerID);
		strcpy(this->sInvesterID, sInvesterID);
		strcpy(this->sInvesterPassword, sInvesterPassword);
		strcpy(this->dataDirPath, dataDirPath);
		fout = stdout;
		ferr = stderr;
	}

	// use relative path
	void setFout(const char *path) {
		fout = fopen(strcat(dataDirPath, path), "w");
	}
	void setFerr(const char *path) {
		ferr = fopen(strcat(dataDirPath, path), "w");
	}

	~CustomUser() {
		if (fout != stdout) {
			fclose(fout);
		}
		if (ferr != stderr) {
			fclose(ferr);
		}
	}
};