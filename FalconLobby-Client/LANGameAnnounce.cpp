#include "stdafx.h"
#include "LANGameAnnounce.h"

using namespace std;
/*

[ start LAN server, in lobby, user: Viper, Balkans ]

t=173.967648
0000   42 46 4f 31 00 00 00 00 01 00 00 00 0d 26 6c 01  BFO1.........&l.
0010   a8 c0 03 00 00 00 00 00 00 00 07 00 00 00 14 00  ................
0020   05 56 69 70 65 72 04 01 00 00 00 07 42 61 6c 6b  .Viper......Balk
0030   61 6e 73 00                                      ans.

[ exit and start LAN server again, in lobby, user: Viper, Balkans ]

0000   42 46 4f 31 00 00 00 00 01 00 00 00 0d 26 6c 01  BFO1.........&l.
0010   a8 c0 03 00 00 00 00 00 00 00 07 00 00 00 14 00  ................
0020   05 56 69 70 65 72 04 01 00 00 00 07 42 61 6c 6b  .Viper......Balk
0030   61 6e 73 00                                      ans.

[ in lobby, user: coder1024, Balkans ]

0000   42 46 4f 31 00 00 00 00 01 00 00 00 0d 2a 6c 01  BFO1.........*l.
0010   a8 c0 03 00 00 00 00 00 00 00 07 00 00 00 18 00  ................
0020   09 63 6f 64 65 72 31 30 32 34 04 01 00 00 00 07  .coder1024......
0030   42 61 6c 6b 61 6e 73 00                          Balkans.

[ in lobby, user: coder1024, Balkans 2005 ]

0000   42 46 4f 31 00 00 00 00 01 00 00 00 0d 2f 6c 01  BFO1........./l.
0010   a8 c0 03 00 00 00 00 00 00 00 07 00 00 00 1d 00  ................
0020   09 63 6f 64 65 72 31 30 32 34 04 01 00 00 00 0c  .coder1024......
0030   42 61 6c 6b 61 6e 73 20 32 30 30 35 00           Balkans 2005.

[ TE, user: coder1024, Balkans 2005 ]

0000   42 46 4f 31 00 00 00 00 01 00 00 00 0d 2f 6c 01  BFO1........./l.
0010   a8 c0 03 00 00 00 00 00 00 00 07 00 00 00 1d 00  ................
0020   09 63 6f 64 65 72 31 30 32 34 02 01 00 10 00 0c  .coder1024......
0030   42 61 6c 6b 61 6e 73 20 32 30 30 35 00           Balkans 2005.

[ Campaign, user: coder1024, Balkans 2005 ]

0000   42 46 4f 31 00 00 00 00 01 00 00 00 0d 2f 6c 01  BFO1........./l.
0010   a8 c0 03 00 00 00 00 00 00 00 07 00 00 00 1d 00  ................
0020   09 63 6f 64 65 72 31 30 32 34 03 01 00 10 00 0c  .coder1024......
0030   42 61 6c 6b 61 6e 73 20 32 30 30 35 00           Balkans 2005.

[ PACKET ]

	Offset Size Field
	====== ==== ==========================================
	     0    4 Packet ID 'B','F','O','1'
	    13    1 Size of the remaining data
	  	 32    1 Pilot Name Length
		 33      Pilot Name String (not NULL term)
		       5 04 01 00 00 00 (in lobby)
			      02 01 00 10 00 (in TE)
			  	   03 01 00 10 00 (in Campaign)
	          1 Theater Name Length
			      Theater Name String (not NULL term)
			    1 NULL
*/

LANGameAnnounce::LANGameAnnounce()
: gotSock(FALSE)
{
	gotSock = sock.Create(0,SOCK_DGRAM,NULL);
}

LANGameAnnounce::~LANGameAnnounce()
{
}

void LANGameAnnounce::SetGames(std::vector<Client::Game>& games)
{
	this->games = games;
}

void LANGameAnnounce::Announce()
{
	if (!gotSock)
		return;

   UINT destPort = getSettingsInt("Falcon","server_port1");

   if (games.size() == 0)
   {
      Client::Game game;
      game.game_name = "MyGameName";
      game.game_theater = "Balkans";
      games.push_back(game);
   }

   for (vector<Client::Game>::iterator i = games.begin(); i != games.end(); ++i)
   {
      char* packet = NULL;
      int packetLen = 0;
      CreatePacket(*i,packet,packetLen);

      sock.SendTo(packet,packetLen,destPort,_T("localhost"));

      delete [] packet;
   }
}

void LANGameAnnounce::CreatePacket(Client::Game& game, char*& data, int& len)
{
   len =
      32 +
      1 + game.game_name.size() +
      5 +
      1 + game.game_theater.size() +
      1;

   data = new char[len];
   memset(data,0,len);

   sprintf(&data[0],"BFO1");
   data[8] = 0x1;
   data[12] = 0xd;
   data[13] = len - 13;
   data[14] = 0x6c;
   data[15] = 0x1;
   data[16] = 0xa8;
   data[17] = 0xc0;
   data[18] = 0x3;
   data[26] = 0x7;
   data[30] = 0x1d;
   data[32] = game.game_name.size();
   strcpy(&data[33],game.game_name.c_str());
   int pos = 33;
   pos += game.game_name.size();
   data[pos++] = 0x3;
   data[pos++] = 0x1;
   data[pos++] = 0x0;
   data[pos++] = 0x10;
   data[pos++] = 0x0;
   data[pos++] = game.game_theater.size();
   strcpy(&data[pos],game.game_theater.c_str());
}

