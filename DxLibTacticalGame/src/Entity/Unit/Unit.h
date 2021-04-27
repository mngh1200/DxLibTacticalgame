#pragma once
#include <climits>
#include "Entity/Object.h"
#include "UnitInfo.h"
#include "Utility/ResourceManager.h"
#include "Animation/Animation.h"
#include "Entity/Battle/Map.h"
#include "Entity/Effect/DamageEffect.h"
#include "Battle/BUI/EnemyTurnCont.h"

using namespace std;

/**
 * @file Unit.h
 * @brief ユニットクラス
 */

namespace Entity
{
	class Unit : public Object
	{
	public:
		Unit() : x_(0), y_(0), baseX_(0), baseY_(0), targetRealX_(0), targetRealY_(0), 
			imageId_(0), isEnemy_(false), state_(State::NORMAL), isActed_(false), closeAttackedLogs_(Direction::NONE_DIRECTION),
			animation_{}, animationSub_{}, viewHp_(0), prevHp_(0), alpha_(255), predictHp_(-1)  {};
		virtual ~Unit() {};

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

		bool damage(int damage, int direction, bool isCloseAtack);

		void forceDecreaseHp(int decrease);

		void avoid();

		bool checkDead();

		void atack(int targetRealX, int targetRealY);

		bool select(bool isSelect);

		void turnEnd(bool isOwnEnd);

		void endAction();

		void getExtraStatusList(vector<pair<string, string>>& list) const;

		void setPredictDamage(int predictDamage);
		void clearPredictDamage();

		int getMassX() const { return x_; }; // x座標を返す
		int getMassY() const { return y_; }; // y座標を返す
		int getBaseX() const { return baseX_; }; // 移動元x座標を返す
		int getBaseY() const { return baseY_; }; // 移動元y座標を返す
		
		
		const UnitInfo& getInfo() const { return info_; } // ユニット名やステータスの情報を返す

		virtual bool isAtackable() const { return true; }; // 攻撃可能であるか
		int getCloseAttackLogs() const { return closeAttackedLogs_; } // 近接攻撃された履歴を返す
		bool isHorse() const { return info_.kind == UnitKey::CAVALRY; } // 騎兵であればtrueを返す
		bool isEnemy() const { return isEnemy_; } // 敵ユニットであるかを返す
		bool isActed() const { return isActed_; } // 行動終了済みであるか
		bool isStricken() const { return info_.hpm > info_.hp; } // ダメージを受けたことがあるか

	protected:
		constexpr static int HP_PADDING = 5; //! HPバーの余白
		constexpr static int HP_Y = 55; //! HPバーの相対位置y
		constexpr static int HP_W = CHIP_SIZE - HP_PADDING * 2; //! HPバーの幅
		constexpr static int HP_H = 5;  //! HPバーの高さ

		constexpr static int ANIME_ATACK_MS = 400;	  //! 攻撃アニメーションの時間
		constexpr static int ANIME_DAMAGE_MOVE = 10;  //! ダメージアニメションの動作範囲
		constexpr static int ANIME_DAMAGE_REPEAT = 4; //! ダメージアニメションのリピート回数

		bool createAnimation(int animationId) override;
		virtual void createAtackAnimation();
		virtual bool updateAtackAnimation();

		virtual void renderExtra() const {}; // 下位クラスで追加する描画処理
		virtual void turnEndExtra(bool isOwnEnd) {};      // 下位クラス用のターンエンド処理
		virtual void getExtraStatusListExtra(vector<pair<string, string>>& list) const {}; // 

		//! マス座標
		int x_, y_;

		//! 攻撃先の正味の座標
		int targetRealX_, targetRealY_;

		//! アニメーションクラス
		Animation animation_;
	
	private:
		void setKind(int kind);
		void setImage(int state);

		//! 画像
		int imageId_;

		//! 移動元マス座標
		int baseX_, baseY_;

		// ユニットの情報
		UnitInfo info_;

		//! 状況
		State state_;

		//! 行動終了済みであるか
		bool isActed_;

		//! 敵かどうか
		bool isEnemy_;

		//! このターン中に近接攻撃を受けた方向のログ
		int closeAttackedLogs_;

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
		int viewHp_, prevHp_, alpha_, predictHp_;

	public:
		constexpr static int ANIME_DAMAGE_MS = 400;	//! ダメージアニメションの時間
	};


}