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
		CheckWin() : 
			winner_(Winner::UNDECIDED),
			limitTurn_(0),
			isPlayerWinOverLimit_(true),
			isPlayerBaseDefense_(false),
			isEnemyBaseDefense_(false)
		{};
		~CheckWin() {};

		// ���҂̒l
		enum Winner
		{
			UNDECIDED,
			PLAYER,
			ENEMY
		};

		void setLimitTurn(int turnNum, bool isPlayerWinOverLimit);

		void setBaseDefense(bool isPlayer);

		void checkWin(shared_ptr<Map> map);
		void checkWin(int turnNum);

		int getWinner() const { return winner_; }; // ���҂�Ԃ�

	private:
		int checkWipeOut(shared_ptr<Map> map);

		//! ����
		int winner_;

		//! �^�[������
		int limitTurn_;

		//! �^�[���������؂ꂽ�^�C�~���O�Ńv���C���[�̏����ɂ��邩
		bool isPlayerWinOverLimit_;

		//! �v���C���[���̖h�q���_�����邩
		bool isPlayerBaseDefense_;

		//! �G���̖h�q���_�����邩
		bool isEnemyBaseDefense_;
	};


}