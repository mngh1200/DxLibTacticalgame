#pragma once
#include <climits>
#include <map>
#include <string>
#include "Entity/UI/Message.h"

using namespace std;
using namespace Entity;

/**
 * @file TutorialManager.h
 * @brief チュートリアルを管理するクラス
 */

namespace
{
	//! チュートリアルID
	enum TutorialId
	{
		UNIT_SELECT,	//! ユニット選択
		STATUS_CHECK,	//! ステータス確認
		MOVE,			//! 移動
		MOVE_CANCEL,	//! 移動キャンセル
		FIGHT_PREDICT,	//! 戦闘予測確認
		ATACK,			//! 攻撃
		TERRAIN_EFFECT,	//! 地形効果
		FREE_SET,		//! 自由配置
		COORDINATED,	//! 連携
		PINCHING,		//! 挟撃
		ALL_KILL,		//! 殲滅
		DEFFENCE,		//! 防衛
		SUPPRESSION,	//! 制圧
		AMBUSH,			//! 迎撃
		AMBUSH_CANCE,	//! 連携、挟撃による迎撃キャンセル
		RUSH,			//! 突撃
		RUSH_CANCEL,	//! 迎撃による突撃キャンセル
		RUSH_NOT_PLAIN,	//! 地形による突撃キャンセル
		THROUGH,		//! 貫通
		LOADING,		//! 弾込め
		HIT_ATTENUATION,//! 命中率減衰
		TUTORIAL_ID_LEN
	};
}

namespace Battle
{
	class TutorialManager
	{
	public:
		TutorialManager() : 
			isAble_(false),
			tutorialIdList_{}
		{};
		~TutorialManager() {};

		void init(int stageId);
		void onEvent(int tutorialId);

	private:
		bool setTutorialIdList(int stageId);

		static const vector<string> MESSAGES;
		static vector<string> initMessages();


		shared_ptr<Message> message_; //! メッセージオブジェクト

		bool isAble_; //! 有効/無効

		vector<int> tutorialIdList_; //! チュートリアルのIDリスト
	};


}