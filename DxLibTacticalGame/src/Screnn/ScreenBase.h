#pragma once
#include <climits>
#include <memory>
#include "Entity/Object.h"

using namespace std;

namespace Screen
{
	class ScreenBase
	{
	public:
		ScreenBase() : isInited_(false) {};
		~ScreenBase() {};

		/**
		 * @fn
		 * 初期処理
		*/
		virtual void init() = 0;

		/**
		 * @fn
		 * イベント取得後のスクリーン更新処理
		*/
		virtual void updateByEvents(weak_ptr < Entity::Object> hitObjWp, int button, int eventType, int x, int y) = 0;
		
		/**
		 * @fn
		 * アニメーション終了後のスクリーン更新処理
		*/
		virtual void updateByAnimation() = 0;
	private:
		//! 初期処理が完了しているか
		bool isInited_;
	};
}