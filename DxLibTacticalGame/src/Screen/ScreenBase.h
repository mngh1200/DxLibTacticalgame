#pragma once
#include <climits>
#include <memory>
#include "Entity/Object.h"

using namespace std;

namespace FrameWork
{
	class Game;
}

namespace Screen
{
	class ScreenBase
	{
	public:
		ScreenBase() : isInited_(false), nowScene_(-1) {};
		~ScreenBase() {};

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
		//! 現在のシーンを保持
		int nowScene_;

	private:
		//! 初期処理が完了しているか
		bool isInited_;
	};
}