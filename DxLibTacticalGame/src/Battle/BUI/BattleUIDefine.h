#pragma once
#include <string>
#include "Utility/ResourceManager.h"
#include "Define.h"
#include "Battle/BattleDefine.h"

/**
 * @file BattleUIDefine.h
 * @brief バトルUI用の共通定数と関数
 */

constexpr int BUI_PADDING = 5; //! バトルUIの余白

constexpr int BUI_FONT_SIZE = 18; //! 文字サイズ
constexpr int BUI_ZEN_W = BUI_FONT_SIZE + 2;			//! 全角文字の幅
constexpr int BUI_HAN_W = (BUI_FONT_SIZE + 2) * 2 / 3;  //! 半角文字の幅

constexpr int BUI_LINE_MARGIN = 3;  //! 一行の余白
constexpr int BUI_LINE_PADDING = 6; //! 一行の余白
constexpr int BUI_LINE_HEIGHT = BUI_FONT_SIZE + BUI_LINE_MARGIN + BUI_LINE_PADDING * 2; //! 一行当たりの高さ
constexpr int BUI_LINE_COUNT = 2;   //! 何行あるか

constexpr int BUI_FONT_TYPE = FontType::NORMAL_S18; //! バトルUI用のフォントタイプ

//! マップ画面のUIエリア
constexpr int BATTLE_UI_AREA_Y = MAP_MASS_H * CHIP_SIZE;
constexpr int BATTLE_UI_AREA_H = WIN_H - BATTLE_UI_AREA_Y;

namespace BUI
{
	int drawLabel(int x0, int y0, string text, int w, int backgroundColorType = ColorType::SUB_COLOR_LITE, int textColorType = ColorType::SUB_COLOR);
	int drawValue(int x0, int y0, string text, int w, int textColorType = ColorType::SUB_COLOR);
	int getHanW(const int textLen);
	int getZenW(const int textLen);
}



