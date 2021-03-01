#pragma once
#include <climits>
#include <string>
#include "Utility/ResourceManager.h"
#include "Entity/Object.h"
#include "Animation/Animation.h"
#include "DxLib.h"
#include "Define.h"
#include "Battle/BUI/BattleUIDefine.h"
#include "Utility/Timer.h"

using namespace std;

/**
 * @file Message.h
 * @brief メッセージ要素
 */

namespace Entity
{
	class Message : public Object
	{
	public:
		Message();
		~Message() {};

		void show(string text, bool isSound, int displayTimeMs = -1);
		void hide();

		void onMouseClick(int x, int y) override;

		void render() const override;

		bool animationUpdate() override;

	protected:
		bool createAnimation(int animationId);

	private:
		void getClosePos(int* x, int* y) const;

		constexpr static int MARGIN = 10; //! 余白
		constexpr static int PADDING = 12; //! 余白
		constexpr static int PADDING_TOP = 6; //! 余白(上側)
		const static int FONT_TYPE = FontType::NORMAL_S24;  //! フォントタイプ

		constexpr static int TEXT_PADDING_RIGHT = PADDING + 12; //! テキストのみの右側余白

		constexpr static int CLOSE_SIZE = 18; //! 閉じるボタンのサイズ
		constexpr static int CLOSE_MARGIN_BOTTOM = 6; //! 閉じるボタンの余白
		constexpr static int CLOSE_H = CLOSE_SIZE + CLOSE_MARGIN_BOTTOM; //! 閉じるボタンの余白含めた高さ

		constexpr static int ALPHA = 210; //! 不透明度
		constexpr static int ANIME_MS = 200; //! アニメーション時間

		Shape shapeClose_; //! 閉じるボタンの座標

		string text_; //! テキスト

		int displayTimeMs_; //! 表示時間

		Animation animation_; //! アニメーション

		// アニメーションの種類
		enum AnimationKind
		{
			SHOW, // 表示時
			DELAY_HIDE // 遅延非表示
		};
	};
}