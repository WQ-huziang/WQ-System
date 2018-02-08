#include "spi.hpp"
#include <stdio.h>

int main() {
	printf("%s\n", CThostFtdcMdApi::GetApiVersion());
}