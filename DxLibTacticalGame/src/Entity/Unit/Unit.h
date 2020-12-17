#pragma once
#include <climits>
#include "Entity/Object.h"
#include "Utility/ResourceManager.h"
#include "Animation/Animation.h"

using namespace std;

/**
 * @file Unit.h
 * @brief ユニットのインターフェースクラス
 */

namespace Entity
{
	class Unit : public Object
	{
	public:
		Unit();
		virtual ~Unit() {};

		void render() const override;
		bool animationUpdate() override;

		void move(int x, int y);

		void damage(int damage);

		void dead();

		/**
		 * @fn
		 * 攻撃力を返す
		*/
		virtual int getAtk() const { return atk_; } ;

		void select(bool isSelect);

	protected:
		bool createAnimation(int animationId);

		//! 画像
		int imageId_;

		//! 位置x
		int x_;

		//! 位置y
		int y_;

		//! アニメーションクラス
		Animation animation_;

		//! 各種ステータス
		int hp_;
		int hpm_;
		int atk_;
		int def_;
		int mov_;

		// アニメーションの種類
		enum AnimationKind
		{
			MOVE,
			DAMAGE,
			ATACK,
			FIRE,
			DESTROY
		};
	private:
		bool isSelected_;
	};


}