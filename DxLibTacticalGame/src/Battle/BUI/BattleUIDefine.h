#pragma once
#include <string>
#include "Utility/ResourceManager.h"
#include "Define.h"
#include "Battle/BattleDefine.h"

/**
 * @file BattleUIDefine.h
 * @brief �o�g��UI�p�̋��ʒ萔�Ɗ֐�
 */

constexpr int BUI_PADDING = 5; //! �o�g��UI�̗]��

constexpr int BUI_FONT_SIZE = 18; //! �����T�C�Y
constexpr int BUI_ZEN_W = BUI_FONT_SIZE + 2;			//! �S�p�����̕�
constexpr int BUI_HAN_W = (BUI_FONT_SIZE + 2) * 2 / 3;  //! ���p�����̕�

constexpr int BUI_LINE_MARGIN = 3;  //! ��s�̗]��
constexpr int BUI_LINE_PADDING = 6; //! ��s�̗]��
constexpr int BUI_LINE_HEIGHT = BUI_FONT_SIZE + BUI_LINE_MARGIN + BUI_LINE_PADDING * 2; //! ��s������̍���
constexpr int BUI_LINE_COUNT = 2;   //! ���s���邩

constexpr int BUI_FONT_TYPE = FontType::NORMAL_S18; //! �o�g��UI�p�̃t�H���g�^�C�v

//! �}�b�v��ʂ�UI�G���A
constexpr int BATTLE_UI_AREA_Y = MAP_MASS_H * CHIP_SIZE;
constexpr int BATTLE_UI_AREA_H = WIN_H - BATTLE_UI_AREA_Y;

namespace BUI
{
	int drawLabel(int x0, int y0, string text, int w, int backgroundColorType = ColorType::SUB_COLOR_LITE, int textColorType = ColorType::SUB_COLOR);
	int drawValue(int x0, int y0, string text, int w, int textColorType = ColorType::SUB_COLOR);
	int getHanW(const int textLen);
	int getZenW(const int textLen);
}



