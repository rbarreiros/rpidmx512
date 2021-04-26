/**
 * @file factorydefaults.h
 *
 */
/* Copyright (C) 2020 by Arjan van Vught mailto:info@orangepi-dmx.nl
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

#ifndef FACTORYDEFAULTS_H_
#define FACTORYDEFAULTS_H_

#include "rdmfactorydefaults.h"

#include "remoteconfig.h"
#include "spiflashstore.h"
#include "storenetwork.h"

class FactoryDefaults: public RDMFactoryDefaults {
public:
	FactoryDefaults() {}
	~FactoryDefaults() {}

	void Set() {
		RemoteConfig::Get()->SetDisable(false);
		SpiFlashStore::Get()->ResetSetList(spiflashstore::Store::RDMDEVICE);
		StoreNetwork::Get()->SaveDhcp(true);
	}
};

#endif /* FACTORYDEFAULTS_H_ */
