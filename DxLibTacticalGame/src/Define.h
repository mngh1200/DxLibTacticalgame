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

//! �摜�`�b�v�̃T�C�Y
constexpr int CHIP_SIZE = 64;

//! �}�b�v�̃}�X�̌�
constexpr int MAP_MASS_W = 20;
constexpr int MAP_MASS_H = 10;

//! �}�b�v�\���ʒu
constexpr int MAP_Y = WIN_H - MAP_MASS_H * CHIP_SIZE;

//! �}�b�v��ʂ�UI�G���A
constexpr int BATTLE_UI_AREA_Y = 0;
constexpr int BATTLE_UI_AREA_H = MAP_Y - BATTLE_UI_AREA_Y;
