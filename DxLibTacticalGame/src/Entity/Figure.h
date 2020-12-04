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

namespace Entity
{
	class Figure
	{
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
			MASS,
			TEXT,
			OVERLAY
		};

		Figure() :
			type_(Type::FIGURE), layerId_(-1), objectId_(-1),
			isReserveDelete_(false), animationId_(-1) {};
		virtual ~Figure() {};

		virtual void init(int layerId, int objectId);

		virtual void render() const = 0;

		/**
		 * @fn
		 * １フレーム分のアニメーション実行
		 * @return trueのとき、アニメーション終了
		 */
		virtual bool animationUpdate() { return true; };

		bool setAnimationId(int animationId);

		bool isAnimation();

		void destroy();

		bool isDelete() const;

		int getLayerId() const;

		int getObjectId() const;

		int getType() const;

	protected:

		//! Objectの種類
		int type_;

		//! オブジェクトの形状(位置やサイズ)
		Shape shape_;

		//! アニメーションID（未指定のときは-1）
		int animationId_;

	private:

		//! 所属レイヤーID
		int layerId_;

		//! オブジェクトID
		int objectId_;

		//! 削除予約
		bool isReserveDelete_;
	};
}