#include "server/UdpServer.h"
#include "adapter/MdDataStruct.h"
#include "test.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])  
{  
    UdpServer *server = new UdpServer(12045);
    WZMarketDataField ori;
    char buf[BUFSIZ + 1];
    while (1) {
        if (server->recvUdpPackage(buf)) {
            cout << "STRING: " << buf << endl << endl;
            WZMarketDataField::fromString(ori, buf);

        }
    }
  
    return 0;  
}  
