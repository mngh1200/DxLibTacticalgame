#pragma once
#include <climits>
#include <memory>
#include "Entity/Object.h"


using namespace std;

/**
 * @file ScreenBase.h
 * @brief 各画面の進行を管理するクラスの基底クラス
 */

namespace FrameWork
{
	class Game;
}

namespace Entity
{
	class Overlay;
}

namespace
{
	constexpr int OVERLAY_ID = 99999;
	constexpr int START_OVERLAY_SCENE = -1;
	constexpr int END_OVERLAY_SCENE = -2;
}

namespace Screen
{
	class ScreenBase
	{
	public:
		ScreenBase() : isInited_(false), nowScene_(START_OVERLAY_SCENE) {};
		virtual ~ScreenBase() {};

		bool isInited() const;
		void inited();

		

		/**
		 * @fn
		 * 初期処理
		*/
		virtual void init() = 0;

		/**
		 * @fn
		 * イベント取得後のスクリーン更新処理
		*/
		virtual void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int x, int y, int button, int eventType) = 0;
		
		/**
		 * @fn
		 * アニメーション終了後のスクリーン更新処理
		*/
		virtual void updateByAnimation() = 0;

	protected:
		void createOverlay(bool isOpen);

		bool isOpenOverlayEnded();
		bool isCloseOverlayEnded();

		//! 現在のシーンを保持
		int nowScene_;

	private:
		//! 初期処理が完了しているか
		bool isInited_;
	};
}