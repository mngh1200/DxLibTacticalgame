#pragma once
#include <climits>
#include <string>
#include "Utility/ResourceManager.h"
#include "Entity/Figure.h"
#include "Battle/Fight.h"
#include "Entity/Unit/Unit.h"
#include "DxLib.h"
#include "Define.h"

using namespace std;
using namespace Battle;

/**
 * @file HitDisplay.h
 * @brief 命中率表示
 */

namespace Entity
{
	class HitDisplay : public Figure
	{
	public:
		HitDisplay() : isShow_(false), hitRate_(100) {};
		~HitDisplay() {};
		
		void show(Fight* fight);
		void hide();

		void render() const override;

	private:
		constexpr static int MARGIN = 10;	//! 左側余白
		constexpr static int PADDING = 5; //! 内側余白

		constexpr static int H = 28;		//! 高さ
		constexpr static int LABEL_W = 35;	//! ラベル幅
		constexpr static int VALUE_W = 62;	//! 値表示部分の幅

		//! 表示しているか
		bool isShow_;

		//! 確率
		int hitRate_;
	};
}