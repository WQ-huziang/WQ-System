#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include "api/ctp/ThostFtdcUserApiStruct.h"
#include "api/ctp/ThostFtdcUserApiDataType.h"
#include "md/CustomCtpMd.h"
#include "util/Iniline.h"
using namespace std;

// Global values

// udp values
int clientID;
int serverID;

// Md values
CustomMd *customMdApi;                                             // my custom api
TThostFtdcBrokerIDType gCompanyID = "9999";                        // company ID
TThostFtdcInvestorIDType gInvesterID = "112586";                   // user ID
TThostFtdcPasswordType gInvesterPassword = "821361187";            // user password
char mdFrontAddr[MAX_LEN];                                         // Md front address
char dataDirPath[MAX_LEN];                                         // stored data dictionary path
char pInstrumentIDStr[MAX_LEN * ID_LEN];                           // stock ID string
char *pInstrumentID[MAX_LEN];                                      // stock ID string array
int instrumentNum;                                                 // stock ID's number

inline void GetCurrentPath(char fullpath[],char *filename)
{  
    realpath(filename, fullpath);
}

void init(char *filepath) {
	// get absolute path
	char fullpath[MAX_LEN];
	GetCurrentPath(fullpath, filepath);

	cout << fullpath << endl;

	// get file values
	char *str;
	str = GetIniKeyString("MdInfo", "FrontAddr", fullpath);
	strcpy(mdFrontAddr, str);
	str = GetIniKeyString("MdInfo", "DataDirPath", fullpath);
	GetCurrentPath(dataDirPath, str);
	strcat(dataDirPath, "/");    // api must need "/"
	str = GetIniKeyString("MdInfo", "InstrumentIDList", fullpath);
	strcpy(pInstrumentIDStr, str);
	str = GetIniKeyString("MdInfo", "out", fullpath);
	clientID = atoi(str);
	str = GetIniKeyString("MdInfo", "in", fullpath);
	serverID = atoi(str);

	cout << mdFrontAddr << endl;
	cout << dataDirPath << endl;
	customMdApi = (CustomMd*)CustomCtpMd::CreateCustomCtpMd(gInvesterID, gInvesterPassword, gCompanyID,
													 dataDirPath, mdFrontAddr, clientID, serverID);
	cout << "Initialize market api ..." << endl;
	instrumentNum = CustomCtpMd::ProcessInstrumentIDList(pInstrumentID, ID_LEN, pInstrumentIDStr);
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage : ./mdserver CONFIG_FILE\n"
			   "\tCONFIG_FILE : initial file path (.ini)\n");
		return 0;
	}

	init(argv[1]);

	// Initialize Md
	customMdApi->SubscribeInstrument(pInstrumentID, instrumentNum);

	customMdApi->Wait();

	return 0;
}