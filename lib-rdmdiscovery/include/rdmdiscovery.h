/**
 * @file rdmddiscovery.h
 *
 */
/* Copyright (C) 2017-2018 by Arjan van Vught mailto:info@orangepi-dmx.nl
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

#ifndef RDMDISCOVERY_H_
#define RDMDISCOVERY_H_

#include <stdint.h>

#include "rdm.h"

#include "rdmmessage.h"
#include "rdmtod.h"

class RDMDiscovery: public RDMTod {
public:
	RDMDiscovery(uint8_t nPort = 0);

	void SetUid(const uint8_t *);
	const uint8_t *GetUid(void);

	void Full(void);

private:
	bool FindDevices(uint64_t, uint64_t);
	bool QuickFind(const uint8_t *);

	bool IsValidDiscoveryResponse(const uint8_t *, uint8_t *);

	void PrintUid(uint64_t);
	void PrintUid(const uint8_t *);
	const uint8_t *ConvertUid(uint64_t);
	uint64_t ConvertUid(const uint8_t *);

private:
	uint8_t m_nPort;
	uint8_t m_Uid[RDM_UID_SIZE];
	RDMMessage m_UnMute;
	RDMMessage m_Mute;
	RDMMessage m_DiscUniqueBranch;
};

#endif /* RDMDISCOVERY_H_ */
