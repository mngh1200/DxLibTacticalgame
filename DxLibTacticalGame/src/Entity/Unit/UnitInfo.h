#pragma once
#include <climits>
#include "Ability.h"
#include "Define.h"
#include "Battle/BattleDefine.h"

using namespace std;

/**
 * @file UnitInfo.h
 * @brief ���j�b�g�̃X�e�[�^�X�Ǘ��N���X
 */

namespace Entity
{
	struct UnitInfo
	{
		//! ���O
		string name = "";

		//! ���j�b�g���
		int kind = UnitKey::LANCER;

		/* �e��X�e�[�^�X */
		int hp = 0;  //! ����HP
		int hpm = 0; //! �ő�HP
		int atk = 0; //! �U��
		int def = 0; //! �h��
		int mov = 0; //! �ړ�
		int len = 0; //! �˒�
		int range = 1; //! �}�b�v��̍U���͈�

		//! �ߐڍU�������邩
		bool isCloseAtack = true;

		//! �X�L��
		Ability ability = Ability::Kind::NONE;

		//! �˒��������e�L�X�g
		const static vector<string> LEN_TEXT; 

		string getLenText() const;
	};
}