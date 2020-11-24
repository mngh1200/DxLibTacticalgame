#pragma once
#include <climits>
#include <string>
#include "Shape.h"
#include "DxLib.h"
#include "Define.h"

namespace Entity
{
	class Figure
	{
	public:
		/**
		 * @enum ObjectType
		 * オブジェクトの種類
		 */
		enum Type {
			FIGURE,
			BACKGROUND,
			BUTTON,
			UNIT,
			MASS,
			TEXT
		};

		Figure() :
			type_(FIGURE),	isTrans_(false), imagePath_(-1), layerId_(-1), objectId_(-1),
			isReserveDelete_(false), animationId_(-1) {};
		virtual ~Figure() {};

		virtual void init(int layerId, int objectId);

		virtual void render() const;

		/**
		 * @fn
		 * １フレーム分のアニメーション実行
		 * @return trueのとき、アニメーション終了
		 */
		virtual bool animationUpdate() { return false; };

		bool setAnimationId(int animationId);

		bool isAnimation();

		void destroy();

		bool isDelete() const;

		int getLayerId() const;

		int getObjectId() const;

		int getType() const;

	protected:

		void renderExtend() const;

		//! Objectの種類
		Type type_;

		//! オブジェクトの形状(位置やサイズ)
		Shape shape_;

		//! 画像を透過させるか
		bool isTrans_;

		//! DxLibで読み込んだ画像のパス
		int imagePath_;

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