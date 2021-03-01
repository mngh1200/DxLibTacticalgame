#pragma once
#include <climits>
#include <memory>
#include <string>
#include "Utility/ResourceManager.h"

using namespace std;

namespace Entity
{
	class Map;
}

/**
 * @file Mass.h
 * @brief マスデータ
 */

namespace Entity
{
	class Mass
	{
	public:
		Mass() : 
			kindId_(Kind::OUT_OF_MAP),
			state(State::NORMAL),
			passingMov(-1),
			imageId_(-1),
			ableUnitSet(false),
			isAttacked(false),
			angle_(0.0) {};
		Mass(int kindId);
		~Mass() {};

		// マス種類
		enum Kind
		{
			PLAIN,		//! 草原
			FOREST,		//! 森
			RIVER,		//! 川
			MOUNTAIN,	//! 山
			FORT_PLAYER,//! 砦（自軍）
			FORT_ENEMY, //! 砦（敵軍）
			LEN,
			OUT_OF_MAP	//! 範囲外
		};

		void init(int adjacent);

		int getCost() const;
		int getAgl() const;
		int getDef() const;

		int getKind() const;
		int getImageId() const;
		double getAngle() const { return angle_; }; // 画像の回転量を返す

		string getText(int line) const;

		bool isMovable() const { return state == MOVABLE; }

		//! 通過時のmov値
		int passingMov;

		//! マスの状況
		int state;

		//! ユニット配置可能であるか
		bool ableUnitSet;

		// マスの状況
		enum State
		{
			NORMAL,
			MOVABLE,	//! 移動可能
			ATK_ABLE	//! 攻撃可能
		};

		//! 敵の攻撃範囲内であるか
		bool isAttacked;

	private:
		void setImageIdFromKind(int kindId, int adjacent);

		static const vector<int> COST;		//! 移動コスト
		static const vector<int> AGL_UP;	//! 回避UP
		static const vector<int> DEF_UP;    //! 防御UP
		static const vector<string> TEXT1;  //! 地形効果説明文

		//! 地形種類
		int kindId_;

		//! 画像
		int imageId_;

		//! 画像の回転
		double angle_;
	};
}