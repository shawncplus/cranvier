#ifndef CRANVIER_PLAYER_H
#define CRANVIER_PLAYER_H

#include "session.hpp"

namespace Cranvier
{
	class Player
	{
		private:
			Cranvier::Session* _session;
		public:
			Player (Cranvier::Session* session) : _session( session ) { }
	};

}

#endif
