#pragma once
#include <climits>
#include <string>
#include "Shape.h"
#include "DxLib.h"
#include "Define.h"

namespace Entity
{
	class ViewObject
	{
	public:
		/**
		 * @enum ObjectType
		 * オブジェクトの種類
		 */
		enum ObjectType {
			OBJECT,
			BACKGROUND,
			BUTTON,
			UNIT,
			MASS,
			TEXT
		};

		ViewObject() :
			objectType(ObjectType::OBJECT),	isTrans_(false), imagePath_(-1), layerId_(-1), objectId_(-1),
			isReserveDelete_(false), animationId(-1) {};
		virtual ~ViewObject() {};

		virtual void init(int layerId, int objectId, Shape shape);

		virtual void render() const;

		void destroy();

		bool isDelete() const;

		int getLayerId() const;

		int getObjectId() const;

		//! Objectの種類
		ObjectType objectType;

	protected:
		/**
		 * @fn
		 * １フレーム分のアニメーション実行
		 * @return trueのとき、アニメーション終了
		 */
		virtual bool animationUpdate() { return false; };

		void renderExtend() const;

		//! オブジェクトの形状(位置やサイズ)
		Shape shape_;

		//! 実施中のアニメーションID（-1のときは未実施）
		int animationId;

		//! 画像を透過させるか
		bool isTrans_;

		//! DxLibで読み込んだ画像のパス
		int imagePath_;

	private:

		//! 所属レイヤーID
		int layerId_;

		//! オブジェクトID
		int objectId_;

		//! 削除予約
		bool isReserveDelete_;
	};
}