#pragma once
/**
 * @file BattleDefine.h
 * @brief �o�g���֘A�̒萔
 */

 //! �摜�`�b�v�̃T�C�Y
constexpr int CHIP_SIZE = 64;

//! �}�b�v�̃}�X�̌�
constexpr int MAP_MASS_W = 20;
constexpr int MAP_MASS_H = 10;

 //! �}�b�v�\���ʒu
constexpr int MAP_Y = 0;

//! �_���[�W�G�t�F�N�g�̉摜����
constexpr int DMAGE_EFFECT_LEN = 10; 
constexpr int DMAGE_EFFECT_W = 26;
constexpr int DMAGE_EFFECT_H = 32;

//! �e�����ɂ�����x�̕ω���
constexpr int dx[4] = { 0, -1, 0, 1 };

//! �e�����ɂ�����x�̕ω���
constexpr int dy[4] = { -1, 0, 1, 0 };

namespace
{
	// ����Ǘ�
	enum UnitKey {
		FREE = -1,	//! ���R�ݒu
		LANCER,		//! ����
		CAVALRY,	//! �R��
		GUNNER,		//! �e��
		UNIT_KIND_LEN
	};

	// ���� (���������̔�����\�ɂ��邽�߂Q�i���Œ�`)
	enum Direction {
		NONE_DIRECTION = 0b0000,
		TOP = 0b0001,
		RIGHT = 0b0010,
		BOTTOM = 0b0100,
		LEFT = 0b1000
	};

	// ����̎��
	enum ActionKind
	{
		NO_ACTION,
		MOVE_ACT,
		WAIT_ACT,
		ATACK_ACT,
		SET_ACT
	};
}



