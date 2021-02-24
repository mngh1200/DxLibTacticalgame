#pragma once
#include <climits>
#include <map>
#include <string>
#include "Entity/UI/Message.h"
#include "Entity/Battle/Map.h"
#include "Entity/Effect/TutorialArrow.h"
#include "Fight.h"

using namespace std;
using namespace Entity;

namespace Battle
{
	class BattleManager;
}

/**
 * @file TutorialManager.h
 * @brief チュートリアルを管理するクラス
 */


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

		//! チュートリアルID
		enum TutorialId : int
		{
			UNIT_SELECT,	//! ユニット選択
			TURN_END,		//! ターンエンド
			MOVE,			//! 移動
			MOVE_CONFIRM,	//! 移動確定
			FIGHT_PREDICT,	//! 戦闘予測確認
			ATACK,			//! 攻撃
			TERRAIN_EFFECT,	//! 地形効果
			FREE_SET_SELECT,//! 自由配置（選択）
			FREE_SET_SET,	//! 自由配置（設置）
			FREE_SET_DEL,	//! 自由配置（削除）
			FREE_SET_FIN,	//! 自由配置（完了）
			COORDINATED,	//! 連携
			PINCHING,		//! 挟撃
			ALL_KILL,		//! 殲滅
			DEFFENCE,		//! 防衛
			SUPPRESSION,	//! 制圧
			AMBUSH,			//! 迎撃
			AMBUSH_CANCEL,	//! 連携、挟撃による迎撃キャンセル
			RUSH,			//! 突撃
			RUSH_CANCEL,	//! 迎撃による突撃キャンセル
			RUSH_NOT_PLAIN,	//! 地形による突撃キャンセル
			THROUGH,		//! 貫通
			SHOOTING,		//! 射撃の説明
			LOADING,		//! 弾込め
			HIT_ATTENUATION,//! 命中率減衰
			TUTORIAL_ID_LEN
		};

		enum class FightPhase
		{
			PREDICT,//! 戦闘予測
			START,	//! 戦闘開始
			END		//! 戦闘終了
		};

		void init(int stageId, shared_ptr<Message> message);
		bool onEvent(int tutorialId, BattleManager* bm);

		void onPlayerTurnStart(BattleManager* bm);

		void onFight(const Fight* fight, FightPhase phase, BattleManager* bm);

	private:
		void showTutorial(int tutorialId, int x = -1, int y = -1);
		void showTutorial(int tutorialId, vector<pair<int,int>>& arrowPosList);
		bool setTutorialIdList(int stageId);

		void clearArrowList();

		constexpr static int MESSAGE_MS = 20000; //! メッセージ表示時間

		constexpr static int MOVE_X = 8; //! 移動チュートリアル時の対象マス座標
		constexpr static int MOVE_Y = 5; //! 移動チュートリアル時の対象マス座標

		constexpr static int FOREST_X = 8; //! 地形チュートリアル時の対象マス座標
		constexpr static int FOREST_Y = 5; //! 地形チュートリアル時の対象マス座標

		constexpr static int SUPPRESSION_X = 8; //! 制圧座標X
		constexpr static int SUPPRESSION_Y = 1; //! 制圧座標Y

		constexpr static int DEFFENCE_X = 8; //! 防衛座標X
		constexpr static int DEFFENCE_Y = 8; //! 防衛座標Y

		static const vector<string> MESSAGES;
		static vector<string> initMessages();


		shared_ptr<Message> message_; //! メッセージオブジェクト

		vector<shared_ptr<TutorialArrow>> arrowList_; //! 表示中のチュートリアル矢印

		bool isAble_; //! 有効/無効

		vector<int> tutorialIdList_; //! チュートリアルのIDリスト

		int prevTutorialId_; //! 前回のチュートリアルID
	};


}