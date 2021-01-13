#pragma once
#include <climits>
#include "Entity/Object.h"
#include "Utility/ResourceManager.h"
#include "Animation/Animation.h"
#include "Entity/Battle/Map.h"
#include "Entity/Effect/DamageEffect.h"

using namespace std;

/**
 * @file Unit.h
 * @brief ユニットのインターフェースクラス
 */

namespace Entity
{
	struct UnitInfo
	{
		//! 名前
		string name = "";

		//! ユニット種類
		int kind = UnitKey::LANCER;

		//! 各種ステータス
		int hp = 0;
		int hpm = 0;
		int atk = 0;
		int def = 0;
		int mov = 0;
		int len = 0;
		int range = 1;
	};

	class Unit : public Object
	{
	public:
		Unit() : x_(0), y_(0), baseX_(0), baseY_(0), targetRealX_(0), targetRealY_(0), 
			imageId_(0), isEnemy_(false), state_(State::NORMAL), isActed_(false),
			animation_{}, animationSub_{}, viewHp_(0), prevHp_(0), alpha_(255)  {};
		~Unit() {};

		//! アニメーションスピードの倍率
		static float animatinTimeRate;

		// 状況の種類
		enum class State
		{
			NORMAL,
			MOVE,
			SELECTED
		};

		void init(int x, int y, int kind, bool isEnemy = false);

		void render() const override;
		bool animationUpdate() override;

		void move(int x, int y);

		void setPos(int x, int y);

		void back();

		bool damage(int damage);

		void avoid();

		bool checkDead();

		void atack(int targetRealX, int targetRealY);

		bool select(bool isSelect);

		void turnEnd();

		void endAction();

		string getLenText() const;

		int getMassX() const { return x_; }; // x座標を返す
		int getMassY() const { return y_; }; // y座標を返す
		int getBaseX() const { return baseX_; }; // 移動元x座標を返す
		int getBaseY() const { return baseY_; }; // 移動元y座標を返す
		
		
		UnitInfo getInfo() const { return info_; } // ユニット名やステータスの情報を返す

		bool isHorse() const { return info_.kind == UnitKey::CAVALRY; } // 騎兵であればtrueを返す
		bool isEnemy() const { return isEnemy_; } // 敵ユニットであるかを返す
		bool isActed() const { return isActed_; } // 行動終了済みであるか

	protected:
		bool createAnimation(int animationId);
	
	private:
		constexpr static int HP_PADDING = 5; //! HPバーの余白
		constexpr static int HP_Y = 55; //! HPバーの相対位置y
		constexpr static int HP_H = 5;  //! HPバーの高さ

		const static vector<string> LEN_TEXT; //! 射程を示すテキスト

		constexpr static int ANIME_ATACK_MS = 400;	  //! 攻撃アニメーションの時間
		constexpr static int ANIME_DAMAGE_MOVE = 10;  //! ダメージアニメションの動作範囲
		constexpr static int ANIME_DAMAGE_REPEAT = 4; //! ダメージアニメションのリピート回数

		void setKind(int kind);

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

		// ユニットの情報
		UnitInfo info_;

		//! 状況
		State state_;

		//! 行動終了済みであるか
		bool isActed_;

		//! 敵かどうか
		bool isEnemy_;

		//! アニメーションクラス
		Animation animation_;

		// アニメーションの種類
		enum AnimationKind
		{
			MOVE,
			DAMAGE,
			AVOID,
			ATACK,
			FIRE,
			DESTROY
		};

		//! アニメーションクラス（複数必要だった場合のサブ）
		Animation animationSub_;

		//! アニメーション用変数
		int viewHp_, prevHp_, alpha_;

	public:
		constexpr static int ANIME_DAMAGE_MS = 400;	//! ダメージアニメションの時間
	};


}