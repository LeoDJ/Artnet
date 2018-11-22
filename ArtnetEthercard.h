/*The MIT License (MIT)

Copyright (c) 2014 Nathanaël Lécaudé
https://github.com/natcl/Artnet, http://forum.pjrc.com/threads/24688-Artnet-to-OctoWS2811

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef ARTNET_ETHERCARD_H
#define ARTNET_ETHERCARD_H

#include <Arduino.h>

#include <Ethercard.h>
#include <IPAddress.h>


// UDP specific
#define ART_NET_PORT 6454
// Opcodes
#define ART_POLL 0x2000
#define ART_DMX 0x5000
// Buffers
#define MAX_BUFFER_ARTNET 600
// Packet
#define ART_NET_ID "Art-Net\0"
#define ART_DMX_START 18

class ArtnetEthercard
{
public:
  ArtnetEthercard();
  void begin();
  void begin(byte mac[]);
  void begin(byte mac[], byte ip[], byte dns[], byte gateway[], byte subnet[]);
  void begin(byte mac[], byte ip[]);

  uint16_t read();
  void _udpCallback(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *artnetPacket, uint16_t len);
  //void printPacketHeader();
  //void printPacketContent();

  // Return a pointer to the start of the DMX data
  inline uint8_t* getDmxFrame(void)
  {
    return artnetPacket + ART_DMX_START;
  }

  inline uint16_t getOpcode(void)
  {
    return opcode;
  }

  inline uint8_t getSequence(void)
  {
    return sequence;
  }

  inline uint16_t getUniverse(void)
  {
    return incomingUniverse;
  }

  inline uint16_t getLength(void)
  {
    return dmxDataLength;
  }

  inline void setArtDmxCallback(void (*fptr)(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data))
  {
    artDmxCallback = fptr;
  }

private:
  uint16_t lastPacketState = 0;
  uint8_t *artnetPacket;
  uint16_t packetSize;
  uint16_t opcode;
  uint8_t sequence;
  uint16_t incomingUniverse;
  uint16_t dmxDataLength;
  void (*artDmxCallback)(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data);
};

#endif
