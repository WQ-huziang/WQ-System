#pragma once

#include "server/UdpServer.h"
#include "server/UdpClient.h"
#include "adapter/MdDataStruct.h"

class CustomMd {
private:
	UdpClient *client;
	UdpServer *server;

public:
	CustomMd(int clientID, int serverID);

	~CustomMd();

	// my spi, use by father
	void OnGetMarketData(const WZMarketDataField &mddata);

	// my api
	virtual void SubscribeInstrument(char **ppInstrumentID, int nCount) = 0;
	virtual void UnsubscribeInstrument(char **ppInstrumentID, int nCount) = 0;
	virtual void Wait() {
		while(1) {}
	}
};