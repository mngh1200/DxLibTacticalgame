#pragma once
#include <climits>
#include "Shape.h"
#include "DxLib.h"

namespace Entity
{
	class Object
	{
	public:
		Object() : isMouseDown_(false), isMouseOver_(false), objectType_(Object::ObjectType::OBJECT) {};
		Object(Shape* shape);
		~Object() {};

		bool checkMouseEvent(int x, int y, int button, int eventType, bool isOtherHit);

	protected:
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

		/**
		 * @enum ObjectType
		 * オブジェクトの種類
		 */
		enum ObjectType {
			OBJECT,
			BACKGROUND,
			UNIT,
			MASS
		};

		ObjectType objectType_;
	};
}