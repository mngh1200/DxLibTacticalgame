#pragma once
#include <climits>
#include <string>
#include "Shape.h"
#include "DxLib.h"
#include "Define.h"

namespace Entity
{
	class Object
	{
	public:
		Object() : 
			isMouseDown_(false), isMouseOver_(false), objectType(OBJECT),
			isTrans_(false), imagePath_(-1), layerId_(-1), objectId_(-1) {};
		virtual ~Object() {};

		virtual void init(int layerId, int objectId, Shape shape);

		bool checkMouseEvent(int x, int y, int button, int* eventType, bool isOtherHit);

		virtual void render() const;

		/**
		 * @enum ObjectType
		 * オブジェクトの種類
		 */
		enum ObjectType {
			OBJECT,
			BACKGROUND,
			BUTTON,
			UNIT,
			MASS
		};

		//! Objectの種類
		ObjectType objectType;

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
		 * クリックされた瞬間の処理
		 */
		void onMouseClick() {};
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

		void renderExtend() const;

		//! オブジェクトの形状(位置やサイズ)
		Shape shape_;

		//! マウスに左ボタンで押下されているか（とりあえず左ボタンだけ）
		bool isMouseDown_;

		//! マウスにホバーされているか
		bool isMouseOver_;

		//! 画像を透過させるか
		bool isTrans_;

		//! DxLibで読み込んだ画像のパス
		int imagePath_;

		//! 所属レイヤーID
		int layerId_;

		//! オブジェクトID
		int objectId_;
	};
}