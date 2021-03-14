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
		RULE,		//! ���[���ݒ�
		CONT_LOG	//! ���샍�O
	};

	// ���[���f�[�^
	struct RuleData
	{
		int unitNum = 6;	//! ���j�b�g��
		int mapId = 0;	//! �}�b�vID
	};

	// �o�g���f�[�^
	struct ContLog
	{
		int x = 0;			//! �Ώ�x���W
		int y = 0;			//! �Ώ�y���W
		int unitId = 0;		//! ���j�b�gID
		int actionKind = -1;	//! �s���̎��
		int hitValue = 0;	//! �����̗����l
	};
}



