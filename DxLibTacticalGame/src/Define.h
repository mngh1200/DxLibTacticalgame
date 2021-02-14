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
constexpr int MOUSE_INPUT_LOG_USED = 3; // ���łɑ��̃C�x���g�Ŏg�p�����ꍇ

//! ���ߓx�̍ő�l
constexpr int MAX_ALPHA = 255;

//! �X�e�[�W��
constexpr int MAX_STAGE = 10;

//! �`���[�g���A���X�e�[�W��
constexpr int TUTORIAL_STAGE_NUM = 6;

namespace
{
	enum StageRank : int {
		LOCK,  //! ���J���X�e�[�W
		NEW,   //! �V�K�X�e�[�W
		NONE,  //! �����N�Ȃ�
		CLEAR, //! �N���A�ς�
		NULL_STAGE,  //! ���݂��Ȃ��X�e�[�W
		RANK_LEN
	};
}



