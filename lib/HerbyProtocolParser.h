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
		void extractBuffer(stateData& stateData);	// extract Data from receiveBuffer and write to stateData struct.
		void insert(stateData& stateData);			// extract Data from stateData struct and build sendBuffer.
		bool fillReceiveBuffer(int howMany);		// copy received I2C Buffer into local receiveBuffer.
		bool send(byte address);					// send sendBuffer to address. Wraps Arduino WireLibrary.

	private:
		bool validate();							// check if received I2C Buffer is in valid HerbyProtocol format
		void clearBuffer();							// utility to clear buffers
		void addCommandToSendBuffer(int pos, uint8_t cmd, uint8_t dataLowByte, uint8_t dataHighByte);	// utility used in insert()

		uint8_t _receiveBuffer[32];					// receiveBuffer
		uint8_t _sendBuffer[32];					// sendBuffer
};

#endif /* HERBYPROTOCOLPARSER_H_ */
