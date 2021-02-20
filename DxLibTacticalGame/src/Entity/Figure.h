#pragma once
#include <climits>
#include <string>
#include "Shape.h"
#include "DxLib.h"
#include "Define.h"

/**
 * @file Figure.h
 * @brief 画面内要素の抽象クラス（マウスイベントを検知しない要素はFigureクラスを直接継承）
 */

namespace FrameWork
{
	class Game;
}

namespace Entity
{
	class Figure
	{
		friend class ObjectsControl;
	public:
		/**
		 * @enum ObjectType
		 * オブジェクトの種類
		 */
		enum Type : int {
			FIGURE,
			BACKGROUND,
			BUTTON,
			UNIT,
			MAP,
			TEXT,
			CONTEXT_MENU,
			RESULT_SCENE,
			OVERLAY
		};

		Figure() :
			type_(Type::FIGURE), layerId_(-1), objectId_(-1),
			isReserveDelete_(false), animationId_(-1), isView_(true) {};
		virtual ~Figure() {};

		virtual void init(int layerId, int objectId);

		virtual void render() const = 0;

		/**
		 * @fn
		 * １フレーム分のアニメーション実行
		 * @return trueのとき、アニメーション終了
		 */
		virtual bool animationUpdate() { return true; };

		bool isAnimation() const;

		void destroy();

		bool isDelete() const;

		int getLayerId() const;

		int getObjectId() const;

		int getType() const;

		int getX() const { return shape_.x; }; // X座標取得
		int getY() const { return shape_.y; }; // Y座標取得

	protected:
		/**
		 * @fn
		 * アニメーション作成(ObjectsControl::addAnimationObjメソッド専用で呼び出す)
		 * @return アニメーション作成可能な場合trueを返す
		 */
		virtual bool createAnimation(int animationId) { return false; };

		void joinAnimationList(int animationId = 0);
		void changeAnimation(int animationId);

		//! Objectの種類
		int type_;

		//! オブジェクトの形状(位置やサイズ)
		Shape shape_;

		//! アニメーションID（未指定のときは-1）
		int animationId_;

		//! 見た目だけの要素の場合 true、マウスイベントを検知する要素の場合 false
		bool isView_;

	private:

		//! 所属レイヤーID
		int layerId_;

		//! オブジェクトID
		int objectId_;

		//! 削除予約
		bool isReserveDelete_;
	};
}