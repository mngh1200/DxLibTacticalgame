#pragma once
#include <climits>
#include "Entity/Object.h"
#include "Utility/ResourceManager.h"
#include "Utility/FontManager.h"
#include "Animation/Animation.h"
#include "Entity/Battle/Map.h"

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
		Unit() : x_(0), y_(0), baseX_(0), baseY_(0), imageId_(0), animation_{},
			hp_(0), hpm_(0), atk_(0), def_(0), mov_(0),
			isEnemy_(false), state_(State::NORMAL) {};
		virtual ~Unit() {};

		//! 状況の種類
		enum class State
		{
			NORMAL,
			MOVE,
			SELECTED,
			ACTED
		};

		void init(int x, int y, bool isEnemy);

		void render() const override;
		bool animationUpdate() override;

		void move(int x, int y);

		void damage(int damage);

		void dead();

		bool select(bool isSelect);

		/**
		 * @fn
		 * 攻撃力を返す
		*/
		virtual int getAtk() const { return atk_; } ;

	protected:
		bool createAnimation(int animationId);

		void setPos(int x, int y);

		//! 画像
		int imageId_;

		//! 位置x
		int x_;

		//! 移動元x
		int baseX_;

		//! 位置y
		int y_;

		//! 移動元y
		int baseY_;

		//! 各種ステータス
		int hp_;
		int hpm_;
		int atk_;
		int def_;
		int mov_;



		//! 状況
		State state_;

		//! アニメーションクラス
		Animation animation_;

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
		bool isEnemy_;
	};


}