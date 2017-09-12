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

#include <ArtnetEthercard.h>

byte Ethernet::buffer[MAX_BUFFER_ARTNET];

ArtnetEthercard::ArtnetEthercard() {}


void ArtnetEthercard::begin()
{
  //ether.udpServerListenOnPort(callback, ART_NET_PORT);
}

void ArtnetEthercard::begin(byte mac[])
{
  ether.begin(sizeof Ethernet::buffer, mac);
  ether.dhcpSetup();
  //ether.udpServerListenOnPort(&ArtnetEthercard::udpCallback, ART_NET_PORT);
}

void ArtnetEthercard::begin(byte mac[], byte ip[])
{
  ether.begin(sizeof Ethernet::buffer, mac);
  ether.staticSetup(ip);
  //ether.udpServerListenOnPort(&ArtnetEthercard::udpCallback, ART_NET_PORT);
}

void ArtnetEthercard::begin(byte mac[], byte ip[], byte dns[], byte gateway[], byte subnet[])
{
  ether.begin(sizeof Ethernet::buffer, mac);
  ether.staticSetup(ip, gateway, dns, subnet);
  //ether.udpServerListenOnPort(&ArtnetEthercard::udpCallback, ART_NET_PORT);
}

void ArtnetEthercard::_udpCallback(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len) 
{
  if (len < 16 || len > MAX_BUFFER_ARTNET)
    lastPacketState = 0;
    return;

  // Check that packetID is "Art-Net" else ignore
  for (byte i = 0 ; i < 8 ; i++)
  {
    if ((uint8_t)data[i] != ART_NET_ID[i])
      lastPacketState = 0;
      return;
  }
  opcode = (uint8_t)data[8] | (uint8_t)data[9] << 8;
  
  if (opcode == ART_DMX)
  {
    sequence = (uint8_t)data[12];
    incomingUniverse = (uint8_t)data[14] | (uint8_t)data[15] << 8;
    dmxDataLength = (uint8_t)data[17] | (uint8_t)data[16] << 8;
    artnetPacket = data;
    if (artDmxCallback) (*artDmxCallback)(incomingUniverse, dmxDataLength, sequence, artnetPacket + ART_DMX_START);
    lastPacketState = ART_DMX;
    return;
  }
  if (opcode == ART_POLL)
  {
    lastPacketState = ART_POLL;
    return;
  }
  lastPacketState = 0;
  return;
}

uint16_t ArtnetEthercard::read()
{
  ether.packetLoop(ether.packetReceive());
  return lastPacketState;
  /*packetSize = Udp.parsePacket();

  if (packetSize <= MAX_BUFFER_ARTNET && packetSize > 0)
  {
      Udp.read(artnetPacket, MAX_BUFFER_ARTNET);

      // Check that packetID is "Art-Net" else ignore
      for (byte i = 0 ; i < 8 ; i++)
      {
        if (artnetPacket[i] != ART_NET_ID[i])
          return 0;
      }

      opcode = artnetPacket[8] | artnetPacket[9] << 8;

      if (opcode == ART_DMX)
      {
        sequence = artnetPacket[12];
        incomingUniverse = artnetPacket[14] | artnetPacket[15] << 8;
        dmxDataLength = artnetPacket[17] | artnetPacket[16] << 8;

        if (artDmxCallback) (*artDmxCallback)(incomingUniverse, dmxDataLength, sequence, artnetPacket + ART_DMX_START);
        return ART_DMX;
      }
      if (opcode == ART_POLL)
      {
        return ART_POLL;
      }
      return 0;
  }
  else
  {
    return 0;
  }*/
}
/*
void ArtnetEthercard::printPacketHeader()
{
  Serial.print("packet size = ");
  Serial.print(packetSize);
  Serial.print("\topcode = ");
  Serial.print(opcode, HEX);
  Serial.print("\tuniverse number = ");
  Serial.print(incomingUniverse);
  Serial.print("\tdata length = ");
  Serial.print(dmxDataLength);
  Serial.print("\tsequence n0. = ");
  Serial.println(sequence);
}

void ArtnetEthercard::printPacketContent()
{
  for (uint16_t i = ART_DMX_START ; i < dmxDataLength ; i++){
    Serial.print(artnetPacket[i], DEC);
    Serial.print("  ");
  }
  Serial.println('\n');
}
*/