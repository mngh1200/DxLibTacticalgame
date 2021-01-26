#pragma once
#include <climits>
#include <string>
#include "FrameWork/Game.h"
#include "Entity/Object.h"
#include "Animation/Animation.h"
#include "DxLib.h"
#include "Utility/ResourceManager.h"
#include "Define.h"

/**
 * @file ResultScene.h
 * @brief 勝敗結果画面
 */

namespace Entity
{
	class ResultScene : public Object
	{
	public:
		ResultScene() : 
			isPlayerWin_(true),	alpha_(0), 
			textAlpha_(0), text(""), textX_(0) {};
		~ResultScene() {};

		static void makeResultScene(bool isPlayerWin);

		void init(bool isPlayerWin);

		void render() const override;

		bool animationUpdate() override;

	protected:
		bool createAnimation(int animationId);

	private:
		bool isPlayerWin_;

		Animation anim_; //! アニメーションクラス

		// アニメーション種類
		enum AnimKind
		{
			DISPLAY
		};

		int alpha_;		//! 不透明度
		int textAlpha_; //! テキストの不透明度

		string text; //! 勝利/敗北テキスト

		int textX_;  //! 勝利/敗北テキストのX座標
	};
}