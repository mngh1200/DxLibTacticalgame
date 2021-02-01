#pragma once
#include <climits>
#include <string>
#include "Utility/ResourceManager.h"
#include "Entity/Figure.h"
#include "Animation/Animation.h"
#include "DxLib.h"
#include "Define.h"

using namespace std;

/**
 * @file Tooltip.h
 * @brief ツールチップ要素
 */

namespace Entity
{
	class Tooltip : public Figure
	{
	public:
		Tooltip() : text_(""), alpha_(0) {};
		~Tooltip() {};

		void show(int x, int y, string text);
		void hide();

		void render() const override;

		bool animationUpdate() override;

	protected:
		bool createAnimation(int animationId);

	private:
		constexpr static int PADDING = 10;
		const static int FONT_TYPE = FontType::NORMAL_S18;

		constexpr static int ANIME_MS = 300; //! アニメーション時間

		int alpha_; //! 不透明度

		string text_; //! テキスト

		Animation animation_; //! アニメーション

		// アニメーションの種類
		enum AnimationKind
		{
			SHOW,
			HIDE
		};
	};
}