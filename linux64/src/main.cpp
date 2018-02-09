#include "spi.hpp"
#include <stdio.h>

int main() {
	CThostFtdcMdApi* api = CThostFtdcMdApi::CreateFtdcMdApi();
	api->init()
	printf("%s\n", CThostFtdcMdApi::GetApiVersion());
}