#pragma once

/**
 * @file NetworkDefine.h
 * @brief �l�b�g���[�N�֘A�̋��ʒ萔��ێ�
 */

//! �|�[�g�ԍ�
constexpr int PORT = 9850;

//! �f�[�^�̋�؂�
constexpr char DATA_SPLIT = ';';

//! �f�[�^���̒l�̋�؂�
constexpr char VALUE_SPLIT = ',';

namespace Network
{
	enum NetworkDataType
	{
		NONE,		//! �Ȃ�
		SIGNAL,		//! �P��̐��l��񑗎�M�p�i�^�[���I����z�u�������j
		RULE,		//! ���[���ݒ�
		CONT_LOG	//! ���샍�O
	};

	// �P����̎��
	enum SignalKind
	{
		NO_SIGNAL,		//! �Ȃ�
		SURRENDER_SIGNAL, //! �~�Q
		TURN_END,		//! �^�[���I��
		SET_END,		//! ���R�z�u����
		RETRY			//! �Đ�
	};

	// ���[���f�[�^
	struct RuleData
	{
		int unitNum = 6;	//! ���j�b�g��
		int mapId = 0;	//! �}�b�vID
		bool isClientFirst = false; //! �N���C�A���g������U�ł��邩
	};

	// �v���C���[���샍�O
	struct ContLog
	{
		int x = 0;				//! �Ώ�x���W
		int y = 0;				//! �Ώ�y���W
		int unitId = 0;			//! ���j�b�gID
		int actionKind = -1;	//! �s���̎�� (BattleDefine.h��ActionKind�Œ�`)
		int extraValue1 = 0;	//! ���̑��̕ϐ��l1�i�����󋵂⃆�j�b�g�z�u���̕���j
		int extraValue2 = 0;	//! ���̑��̕ϐ��l2�i�����󋵂⃆�j�b�g�z�u���̕���j
	};
}



