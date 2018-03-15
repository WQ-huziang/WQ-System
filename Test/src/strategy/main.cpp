#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "server/UdpServer.h"
#include "server/UdpClient.h"
#include "util/Iniline.h"
#include "adapter/MdDataStruct.h"
using namespace std;

// udp values
UdpServer *server;
UdpClient *client;
char_11 gBrokerID;
char_16 gUserID;


inline void GetCurrentPath(char fullpath[],char *filename)
{  
    realpath(filename, fullpath);
}

void init(char *filepath) {
    // get absolute path
    char fullpath[MAX_LEN];
    GetCurrentPath(fullpath, filepath);

    cout << "Configfile path: " << fullpath << endl;

    // get file values
    char *str;
    str = GetIniKeyString("Public", "BrokerID", fullpath);
    strcpy(gBrokerID, str);
    str = GetIniKeyString("Public", "InvesterID", fullpath);
    strcpy(gUserID, str);
    str = GetIniKeyString("StrategyInfo", "out", fullpath);
    client = new UdpClient(atoi(str));
    str = GetIniKeyString("StrategyInfo", "in", fullpath);
    server = new UdpServer(atoi(str));
}

void strategy(WZInputOrderField &order, const WZMarketDataField &mddata) {
    strcpy(order.BrokerID, gBrokerID);
    strcpy(order.UserID, gUserID);
    strcpy(order.InstrumentID, mddata.InstrumentID);
    order.LimitPrice = mddata.LowestPrice - 100;
    order.Direction = WZ_CHAR_Buy;
    order.Volume = 1;
    order.MinVolume = 1;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("Usage : ./strategy CONFIG_FILE\n"
               "\tCONFIG_FILE : initial file path (.ini)\n");
        return 0;
    }

    init(argv[1]);

    char buf[BUFSIZ + 1];
    WZMarketDataField mddata = {};
    WZInputOrderField order = {};
    if (server->recvUdpPackage(buf)) {
        WZMarketDataField::fromString(mddata, buf);
        cout << mddata.InstrumentID << endl;
        strategy(order, mddata);
        cout << order.InstrumentID << endl;
        cout << order.LimitPrice << endl;
        buf[0] = CHAR_OrderInsert;
        WZInputOrderField::toString(buf + TRAN_FLAG_LEN, order);
        if (client->sendUdpPackage(buf, sizeof(order) + TRAN_FLAG_LEN)) {
            printf("Strategy successfully!\n");
        }
    }

    delete client;
    delete server;

    return 0;
}