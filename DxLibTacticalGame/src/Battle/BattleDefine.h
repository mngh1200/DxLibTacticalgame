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

namespace
{
	// ����Ǘ�
	enum UnitKey {
		LANCER,
		CAVALRY,
		GUNNER,
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
}



