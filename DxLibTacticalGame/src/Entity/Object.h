#pragma once
#include <climits>
#include <string>
#include "Figure.h"
#include "Shape.h"
#include "DxLib.h"
#include "Define.h"

namespace FrameWork
{
	class Game;
}

/**
 * @file Object.h
 * @brief マウスイベントを検知する画面内要素の抽象クラス
 */

namespace Entity
{
	class Object : public Figure
	{
	public:

		Object() : isMouseDown_(false), isMouseOver_(false), isFocus_(false){ isView_ = false; };
		virtual ~Object() {};

		bool checkMouseEvent(int x, int y, int button, int* eventType, bool isOtherHit);

		void onBlurBase();
		void onForceFocus();

	protected:
		/**
		 * @fn
		 * マウスが左ボタンで押下された瞬間の処理
		 */
		virtual void onMouseLeftDown(int x, int y) {};
		/**
		 * @fn
		 * マウスの左ボタンが押し上げられた瞬間の処理
		 */
		virtual void onMouseLeftUp(int x, int y) {};
		/**
		 * @fn
		 * クリックされた瞬間の処理
		 */
		virtual void onMouseClick(int x, int y) {};
		/**
		 * @fn
		 * マウスにホバーされた瞬間の処理
		 */
		virtual void onMouseOver(int x, int y) {};
		/**
		 * @fn
		 * マウスがホバー状態から離れたときの処理
		 */
		virtual void onMouseOut(int x, int y) {};

		/**
		 * @fn
		 * マウスが右ボタンで押下された瞬間の処理
		 */
		virtual void onMouseRightDown(int x, int y) {};
		/**
		 * @fn
		 * マウスの右ボタンが押し上げられた瞬間の処理
		 */
		virtual void onMouseRightUp(int x, int y) {};

		/**
		 * @fn
		 * マウスがホイールボタンで押下された瞬間の処理
		 */
		virtual void onMouseWheelDown(int x, int y) {};
		/**
		 * @fn
		 * マウスのホイールボタンが押し上げられた瞬間の処理
		 */
		virtual void onMouseWheelUp(int x, int y) {};
		/**
		 * @fn
		 * 要素をフォーカスした瞬間の処理
		 */
		virtual void onFocus() {};
		/**
		 * @fn
		 * 要素からフォーカスを外した瞬間の処理
		 */
		virtual void onBlur() {};

		//! マウスに左ボタンで押下されているか（とりあえず左ボタンだけ）
		bool isMouseDown_;

		//! マウスにホバーされているか
		bool isMouseOver_;

		//! 要素をフォーカス中であるか（最後にクリックした要素であるか）
		bool isFocus_;
	};
}