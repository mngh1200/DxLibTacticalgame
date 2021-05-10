#pragma once
#include <climits>
#include <map>
#include "Entity/Battle/Map.h"
#include "Entity/Unit/Unit.h"
#include "Entity/UI/Message.h"

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

		void loadData(vector<int>& data);

		void setLimitTurn(int turnNum, bool isPlayerWinOverLimit);

		void setBaseDefense(bool isPlayer);

		void showRemainingTurnMessage(shared_ptr<Message> message, int turnNum);

		void checkWin(shared_ptr<Map> map);
		void checkWin(int turnNum);

		void getWinConditionsText(string* text, int* lineCount) const;
		void getLoseConditionsText(string* text, int* lineCount) const;

		int getWinner() const { return winner_; }; // ���҂�Ԃ�

		int getLimitTurn() const { return limitTurn_; } // �^�[��������Ԃ�
		bool isPlayerWinOverLimit() const { return isPlayerWinOverLimit_; } // �^�[���I�[�o�[���Ƀv���C���[�����ƂȂ邩��Ԃ�
		bool isPlayerBaseDefense() const { return isPlayerBaseDefense_; } // �v���C���[���̖h�q���_�����邩��Ԃ�
		bool isEnemyBaseDefense() const { return isEnemyBaseDefense_; } // �G���̖h�q���_�����邩

	private:
		int checkWipeOut(shared_ptr<Map> map);
		
		constexpr static int MESSAGE_MS = 3000; //! ���b�Z�[�W�̕\�����ԁims�j

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

		enum Rule
		{
			NORMAL,  //! �h�q�n�_�Ȃ�
			DEFENSE, //! �v���C���[�h�q�n�_�L��
			ATTACK,  //! �G���̖h�q�n�_����
			CONFRICT //! �����̖h�q�n�_����
		};
	};


}