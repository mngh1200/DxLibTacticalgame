#pragma once
#include <climits>
#include <map>
#include "Entity/Battle/Map.h"
#include "Entity/Unit/Unit.h"

using namespace std;
using namespace Entity;

/**
 * @file CheckWin.h
 * @brief ���s�����`�F�b�N�p�̃N���X
 */

namespace Battle
{
	class CheckWin
	{
	public:
		CheckWin() : winner_(Winner::UNDECIDED) {};
		~CheckWin() {};

		void checkWin(shared_ptr<Map> map);

		int getWinner() const { return winner_; }; // ���҂�Ԃ�

	private:

		//! ����
		int winner_;

		// ���҂̒l
		enum Winner
		{
			UNDECIDED,
			PLAYER,
			ENEMY
		};
	};


}