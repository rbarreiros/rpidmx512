/**
 * @file main.cpp
 *
 */
/* Copyright (C) 2019-2020 by Arjan van Vught mailto:info@orangepi-dmx.nl
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <stdint.h>

#include "hardware.h"
#include "networkh3emac.h"
#include "ledblink.h"

#include "h3/showsystime.h"

#include "ntpclient.h"

#include "display.h"

#include "networkconst.h"

#include "mdns.h"
#include "mdnsservices.h"

#include "oscserver.h"
#include "oscserverparms.h"
#include "storeoscserver.h"
#include "oscservermsgconst.h"

// Monitor Output
#include "dmxmonitor.h"

#include "firmwareversion.h"
#include "software_version.h"

#include "displayhandler.h"

#include "spiflashinstall.h"
#include "spiflashstore.h"

#include "remoteconfig.h"
#include "remoteconfigparams.h"
#include "storeremoteconfig.h"

extern "C" {

void notmain(void) {
	Hardware hw;
	NetworkH3emac nw;
	LedBlink lb;
	Display display(DisplayType::SSD1306);
	FirmwareVersion fw(SOFTWARE_VERSION, __DATE__, __TIME__);
	ShowSystime showSystime;

	SpiFlashInstall spiFlashInstall;
	SpiFlashStore spiFlashStore;

	console_clear();

	fw.Print();

	console_puts("OSC ");
	console_set_fg_color(CONSOLE_GREEN);
	console_puts("Real-time DMX Monitor");
	console_set_fg_color(CONSOLE_WHITE);
	console_set_top_row(2);

	hw.SetLed(hardware::LedStatus::ON);
	lb.SetLedBlinkDisplay(new DisplayHandler);

	display.TextStatus(NetworkConst::MSG_NETWORK_INIT, Display7SegmentMessage::INFO_NETWORK_INIT, CONSOLE_YELLOW);

	nw.SetNetworkStore(StoreNetwork::Get());
	nw.Init(StoreNetwork::Get());
	nw.Print();

	NtpClient ntpClient;
	ntpClient.Start();
	ntpClient.Print();

	if (ntpClient.GetStatus() != NtpClientStatus::FAILED) {
		printf("Set RTC from System Clock\n");
		HwClock::Get()->SysToHc();
	}

	display.TextStatus(OscServerMsgConst::PARAMS, Display7SegmentMessage::INFO_BRIDGE_PARMAMS, CONSOLE_YELLOW);

	StoreOscServer storeOscServer;
	OSCServerParams params(&storeOscServer);
	OscServer server;

	if (params.Load()) {
		params.Dump();
		params.Set(&server);
	}

	MDNS mDns;
	mDns.Start();
	mDns.AddServiceRecord(nullptr, MDNS_SERVICE_OSC, server.GetPortIncoming(), "type=monitor");
	mDns.Print();

	DMXMonitor monitor;
	// There is support for HEX output only
	server.SetOutput(&monitor);
	monitor.Cls();
	console_set_top_row(20);

	server.Print();

	for (unsigned i = 1; i < 7 ; i++) {
		display.ClearLine(i);
	}

	uint8_t nHwTextLength;

	display.Printf(1, "OSC Monitor");
	display.Write(2, hw.GetBoardName(nHwTextLength));
	display.Printf(3, "IP: " IPSTR " %c", IP2STR(Network::Get()->GetIp()), nw.IsDhcpKnown() ? (nw.IsDhcpUsed() ? 'D' : 'S') : ' ');
	display.Printf(4, "In: %d", server.GetPortIncoming());
	display.Printf(5, "Out: %d", server.GetPortOutgoing());

	RemoteConfig remoteConfig(remoteconfig::Node::OSC, remoteconfig::Output::MONITOR, 1);

	StoreRemoteConfig storeRemoteConfig;
	RemoteConfigParams remoteConfigParams(&storeRemoteConfig);

	if(remoteConfigParams.Load()) {
		remoteConfigParams.Set(&remoteConfig);
		remoteConfigParams.Dump();
	}

	while (spiFlashStore.Flash())
		;

	display.TextStatus(OscServerMsgConst::START, Display7SegmentMessage::INFO_BRIDGE_START, CONSOLE_YELLOW);

	server.Start();

	display.TextStatus(OscServerMsgConst::STARTED, Display7SegmentMessage::INFO_BRIDGE_STARTED, CONSOLE_GREEN);

	hw.WatchdogInit();

	for (;;) {
		hw.WatchdogFeed();
		nw.Run();
		server.Run();
		remoteConfig.Run();
		spiFlashStore.Flash();
		ntpClient.Run();
		lb.Run();
		showSystime.Run();
		display.Run();
	}
}

}
