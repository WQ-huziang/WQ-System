#include <iostream>
#include "../ThostFtdcMdApi.h"
#include "../ThostFtdcTraderApi.h"
#include "../ThostFtdcUserApiDataType.h"
#include "../ThostFtdcUserApiStruct.h"
#define DATADIRPATH "../../data"

int main() {
    CThostFtdcMdApi* api = CThostFtdcMdApi::CreateFtdcMdApi(DATADIRPATH);
    api->Init();
    const char* ver = CThostFtdcMdApi::GetApiVersion();
    printf("Version: %s\n", ver);
    api->Release();
}