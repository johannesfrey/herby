/*
 * HerbyProtocolParser.h
 *
 *  Created on: Jan 17, 2015
 *      Author: johannes
 */

#ifndef HERBYPROTOCOLPARSER_H_
#define HERBYPROTOCOLPARSER_H_


#include "HerbyCommunicationProtocol.h"
#include <Wire.h>

class HerbyProtocolParser
{
	public:
		HerbyProtocolParser();
		void extractBuffer(stateData& stateData);
		void insert(stateData& stateData);
		bool fillReceiveBuffer(int howMany);
		bool send(byte address);

	private:
		bool validate();
		void clearBuffer();
		void addCommandToSendBuffer(int pos, uint8_t cmd, uint8_t dataLowByte, uint8_t dataHighByte);

		uint8_t _receiveBuffer[32];
		uint8_t _sendBuffer[32];
};

#endif /* HERBYPROTOCOLPARSER_H_ */
