#pragma once
#include <climits>
#include "Entity/Object.h"
#include "Utility/ResourceManager.h"
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
		Unit() : x_(0), y_(0), baseX_(0), baseY_(0), targetRealX_(0), targetRealY_(0), 
			kind_(0), imageId_(0), animation_{}, animationSub_{},
			hp_(0), hpm_(0), atk_(0), def_(0), mov_(0), len_(0), range_(1), viewHp_(0), prevHp_(0),
			alpha_(255), isEnemy_(false), state_(State::NORMAL) {};
		~Unit() {};

		// 状況の種類
		enum class State
		{
			NORMAL,
			MOVE,
			SELECTED,
			ACTED
		};

		void init(int x, int y, int kind, bool isEnemy = false);

		void render() const override;
		bool animationUpdate() override;

		void move(int x, int y);

		void back();

		bool damage(int damage);

		bool checkDead();

		bool select(bool isSelect);

		int atack(int targetRealX, int targetRealY);

		void setPos(int x, int y);

		string getLenText() const;

		int getMassX() const { return x_; }; // x座標を返す
		int getMassY() const { return y_; }; // y座標を返す
		int getBaseX() const { return baseX_; }; // 移動元x座標を返す
		int getBaseY() const { return baseY_; }; // 移動元y座標を返す
		virtual int getAtk() const { return atk_; } ; // 攻撃力を返す
		int getMove() const { return mov_; }; // 移動力を返す
		int getLen() const { return len_; }; // 射程を返す
		int getRange() const { return range_; }; // 攻撃範囲を返す

		bool isHorse() const { return kind_ == UnitKey::CAVALRY; } // 騎兵であればtrueを返す
		bool isEnemy() const { return isEnemy_; } // 敵ユニットであるかを返す

	protected:
		bool createAnimation(int animationId);
	
	private:
		constexpr static int HP_PADDING = 5; //! HPバーの余白
		constexpr static int HP_Y = 55; //! HPバーの相対位置y
		constexpr static int HP_H = 5;  //! HPバーの高さ

		const static vector<string> LEN_TEXT; //! 射程を示すテキスト

		constexpr static int ANIME_ATACK_MS = 400;					//! 攻撃アニメーションの時間
		constexpr static int ANIME_DAMAGE_MS = ANIME_ATACK_MS / 2;	//! ダメージアニメションの時間
		constexpr static int ANIME_DAMAGE_MOVE = 10;				//! ダメージアニメションの動作範囲
		constexpr static int ANIME_DAMAGE_REPEAT = 4;				//! ダメージアニメションのリピート回数

		void setKind(int kind);

		//! ユニットの種類
		int kind_;

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

		//! 攻撃先のx座標
		int targetRealX_;

		//! 攻撃先のy座標
		int targetRealY_;

		//! 各種ステータス
		int hp_;
		int hpm_;
		int atk_;
		int def_;
		int mov_;
		int len_;
		int range_;

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

		//! アニメーション用
		int viewHp_, prevHp_;

		//! アニメーション用 不透明度
		int alpha_;

		//! 敵かどうか
		bool isEnemy_;

		//! アニメーションクラス（複数必要だった場合のサブ）
		Animation animationSub_;
	};


}