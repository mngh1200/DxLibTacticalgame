#pragma once

/**
 * @file Define.h
 * @brief ���ʒ萔��ێ�
 */

//! �E�B���h�E�𑜓x(��)
constexpr int WIN_W = 1280;

//! �E�B���h�E�𑜓x�i�����j
constexpr int WIN_H = 720;

//! �}�E�X�̃N���b�N���̃C�x���g�^�C�v
constexpr int MOUSE_INPUT_LOG_CLICK = 2;

//! ���ߓx�̍ő�l
constexpr int MAX_ALPHA = 255;

//! �X�e�[�W��
constexpr int MAX_STAGE = 10;

namespace
{
	enum StageRank : int {
		LOCK,  // ���J���X�e�[�W
		NEW,   // �V�K�X�e�[�W
		NONE,  // �����N�Ȃ�
		D,
		C,
		B,
		A,
		S,
		RANK_LEN
	};
}



