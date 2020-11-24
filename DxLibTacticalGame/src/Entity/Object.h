#pragma once
#include <climits>
#include <string>
#include "Figure.h"
#include "Shape.h"
#include "DxLib.h"
#include "Define.h"

namespace Entity
{
	class Object : public Figure
	{
	public:

		Object() : isMouseDown_(false), isMouseOver_(false) {};
		virtual ~Object() {};

		bool checkMouseEvent(int x, int y, int button, int* eventType, bool isOtherHit);

	protected:
		/**
		 * @fn
		 * マウスが左ボタンで押下された瞬間の処理
		 */
		virtual void onMouseLeftDown() {};
		/**
		 * @fn
		 * マウスの左ボタンが押し上げられた瞬間の処理
		 */
		virtual void onMouseLeftUp() {};
		/**
		 * @fn
		 * クリックされた瞬間の処理
		 */
		virtual void onMouseClick() {};
		/**
		 * @fn
		 * マウスにホバーされた瞬間の処理
		 */
		virtual void onMouseOver() {};
		/**
		 * @fn
		 * マウスがホバー状態から離れたときの処理
		 */
		virtual void onMouseOut() {};

		/**
		 * @fn
		 * マウスが右ボタンで押下された瞬間の処理
		 */
		virtual void onMouseRightDown() {};
		/**
		 * @fn
		 * マウスの右ボタンが押し上げられた瞬間の処理
		 */
		virtual void onMouseRightUp() {};

		/**
		 * @fn
		 * マウスがホイールボタンで押下された瞬間の処理
		 */
		virtual void onMouseWheelDown() {};
		/**
		 * @fn
		 * マウスのホイールボタンが押し上げられた瞬間の処理
		 */
		virtual void onMouseWheelUp() {};

		//! マウスに左ボタンで押下されているか（とりあえず左ボタンだけ）
		bool isMouseDown_;

		//! マウスにホバーされているか
		bool isMouseOver_;
	};
}