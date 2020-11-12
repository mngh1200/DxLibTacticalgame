#pragma once
#include <climits>
#include "DxLib.h"
#include "FrameWork/Controller.h"
#include "Shape.h"

namespace Entity
{
	class Object
	{
	public:
		Object() : isMouseDown_(false), isMouseOver_(false) {};
		Object(Shape* shape);
		~Object();

		bool isHit(int x, int y, int mouseStatus);
	private:
		/**
		 * @fn
		 * マウスが左ボタンで押下された瞬間の処理
		 */
		void onMouseLeftDown() {};
		/**
		 * @fn
		 * マウスの左ボタンが押し上げられた瞬間の処理
		 */
		void onMouseLeftUp() {};
		/**
		 * @fn
		 * マウスにホバーされた瞬間の処理
		 */
		void onMouseOver() {};
		/**
		 * @fn
		 * マウスがホバー状態から離れたときの処理
		 */
		void onMouseOut() {};

		/**
		 * @fn
		 * マウスが右ボタンで押下された瞬間の処理
		 */
		void onMouseRightDown() {};
		/**
		 * @fn
		 * マウスの右ボタンが押し上げられた瞬間の処理
		 */
		void onMouseRightUp() {};

		/**
		 * @fn
		 * マウスがホイールボタンで押下された瞬間の処理
		 */
		void onMouseWheelDown() {};
		/**
		 * @fn
		 * マウスのホイールボタンが押し上げられた瞬間の処理
		 */
		void onMouseWheelUp() {};

		//! オブジェクトの形状(位置やサイズ)
		Shape* shape_;

		//! マウスに左ボタンで押下されているか（とりあえず左ボタンだけ）
		bool isMouseDown_;

		//! マウスにホバーされているか
		bool isMouseOver_;
	};
}