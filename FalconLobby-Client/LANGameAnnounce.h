#pragma once

//******************************************************************************
//* Created this class to generate the LAN game announce packets which cause
//* F4AF to show them in the LAN Games multiplayer list.  This was never added
//* to FalconLobby or used, just some code added as a potential future feature.
//******************************************************************************

#include "Settings.h"
#include "Client.h"

#include <vector>

class LANGameAnnounce : public Settings
{
public:
	LANGameAnnounce();
	virtual ~LANGameAnnounce();

	void SetGames(std::vector<Client::Game>& games);
	
	void Announce();

private:
	std::vector<Client::Game> games;
	CSocket sock;
	BOOL gotSock;

   void CreatePacket(Client::Game& game, char*& data, int& len);
};
