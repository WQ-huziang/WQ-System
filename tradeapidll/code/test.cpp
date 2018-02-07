#include <iostream>
#include "../ThostFtdcMdApi.h"
#define DATADIRPATH "../../data"

int main() {
    CThostFtdcMdApi* api = CThostFtdcMdApi::CreateFtdcMdApi(DATADIRPATH);
    api->Init();
    const char* ver = CThostFtdcMdApi::GetApiVersion();
    printf("Version: %d\n", ver);
    api->Release();
}