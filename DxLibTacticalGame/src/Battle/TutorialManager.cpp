#include "TutorialManager.h"

namespace Battle {
	const vector<string> TutorialManager::MESSAGES = TutorialManager::initMessages();

	/**
	 * @fn
	 * チュートリアルIDに対応するメッセージを定義
	 */
	vector<string> TutorialManager::initMessages()
	{
		vector<string> messageList(TutorialId::TUTORIAL_ID_LEN);

		messageList.at(TutorialId::UNIT_SELECT) =		"ユニットをクリックすることで\nクリックしたユニットを選択することができます";
		messageList.at(TutorialId::TURN_END) =			"右下の「ターン終了」ボタン\nまたは、右クリックで表示されるシステムメニューから\n自分のターンを終了できます";
		messageList.at(TutorialId::MOVE) =				"青くなっているマスを選択すると\nユニットをクリックしたマスに移動させられます";
		messageList.at(TutorialId::MOVE_CONFIRM) =		"選択中のユニットをクリックすると、移動を確定させられます\n選択中のユニット以外をクリックすると、移動をキャンセルできます";
		messageList.at(TutorialId::FIGHT_PREDICT) =		"①敵ユニットに隣接するように移動\n②隣接した敵ユニットにマウスカーソルを合わせる\n上記手順で画面下に戦闘予測が表示されます";
		messageList.at(TutorialId::ATACK) =				"赤くなっているマスの敵ユニットをクリックすると攻撃できます";
		messageList.at(TutorialId::TERRAIN_EFFECT) =	"森マスにユニットを置くと、回避や防御が上昇します\n（地形効果は、マスの上にカーソルを置くと左下に表示されます）";
		messageList.at(TutorialId::FREE_SET_SELECT) =	"左下から設置したいユニットを選択してください";
		messageList.at(TutorialId::FREE_SET_SET) =		"青いマスをクリックすることで\n選択中のユニットを設置できます";
		messageList.at(TutorialId::FREE_SET_DEL) =		"設置したユニットをクリックすることで\n設置を取り消すことができます";
		messageList.at(TutorialId::FREE_SET_FIN) =		"ユニットの設置がすべて完了したら\n右下の「準備完了」ボタンをクリックしてください";
		messageList.at(TutorialId::COORDINATED) =		"攻撃後、他のユニットで側面から攻撃することで\n「連携」攻撃になり、ダメージが上昇します";
		messageList.at(TutorialId::PINCHING) =			"攻撃後、他のユニットで反対側から攻撃することで\n「挟撃」になり、ダメージが大幅に上昇します";
		messageList.at(TutorialId::ALL_KILL) =			"敵ユニットを全滅させることで勝利になります";
		messageList.at(TutorialId::DEFFENCE) =			"敵ユニットが自軍の砦に移動すると敗北になります";
		messageList.at(TutorialId::SUPPRESSION) =		"敵軍の砦に移動することで勝利になります";
		messageList.at(TutorialId::AMBUSH) =			"槍兵が騎兵（自分よりも射程の短いユニット）に攻撃された時\nスキル「迎撃」を発動します\n「迎撃」を発動すると、槍兵側が先制攻撃します";
		messageList.at(TutorialId::AMBUSH_CANCEL) =		"「連携」または「挟撃」をすると\n槍兵のスキル「迎撃」を無効化できます";
		messageList.at(TutorialId::RUSH) =				"騎兵は自分から攻撃した時にスキル「突撃」を発動します。\n「突撃」を発動すると、ダメージが上昇します。";
		messageList.at(TutorialId::RUSH_CANCEL) =		"「迎撃」を受けた場合は、「突撃」は発動しません";
		messageList.at(TutorialId::RUSH_NOT_PLAIN) =	"「草原」マス以外では「突撃」は発動しません";
		messageList.at(TutorialId::THROUGH) =			"銃兵はスキル「貫通」により、敵ユニットの防御力を無視して\nダメージを与えることができます";
		messageList.at(TutorialId::LOADING) =			"銃兵は攻撃した次のターンに攻撃不可になります";
		messageList.at(TutorialId::HIT_ATTENUATION) =	"銃兵は離れた敵を攻撃しようとした場合、命中率が減少します";

		return messageList;
	}

	/**
	 * @fn
	 * 初期処理
	 * @param (sategeId) ステージID
	 */
	void TutorialManager::init(int stageId, shared_ptr<Message> message)
	{
		isAble_ = setTutorialIdList(stageId);
		message_ = message;
	}

	/**
	 * @fn
	 * 特定イベント発生時に、セットされているチュートリアルIDがあればメッセージ表示
	 * @return メッセージが表示された場合 trueを返す
	 */
	bool TutorialManager::onEvent(int tutorialId)
	{
		if (!isAble_)
		{
			return false; // 無効状態
		}

		for (auto itr = tutorialIdList_.begin(); itr != tutorialIdList_.end(); ++itr)
		{
			if (*itr == tutorialId)
			{
				// チュートリアルメッセージ表示
				message_->show(MESSAGES[tutorialId], true);
				tutorialIdList_.erase(itr);
				return true;
			}
		}

		return false;
	}

	/**
	 * @fn
	 * プレイヤーターン開始時に、特定のチュートリアルメッセージを表示
	 */
	void TutorialManager::onPlayerTurnStart(shared_ptr<Map> map)
	{
		if (!isAble_)
		{
			return; // 無効状態
		}

		for (auto itr = tutorialIdList_.begin(); itr != tutorialIdList_.end(); ++itr)
		{
			int tutorialId = *itr;
			if (tutorialId == TutorialId::UNIT_SELECT ||	// ユニット選択
				tutorialId == TutorialId::FIGHT_PREDICT ||	// 戦闘予測
				tutorialId == TutorialId::PINCHING ||		// 挟撃
				tutorialId == TutorialId::AMBUSH ||			// 迎撃
				tutorialId == TutorialId::DEFFENCE ||		// 防衛
				tutorialId == TutorialId::RUSH ||			// 突撃
				tutorialId == TutorialId::RUSH_NOT_PLAIN ||	// 突撃(草原以外)
				tutorialId == TutorialId::LOADING ||		// 弾込め
				tutorialId == TutorialId::TERRAIN_EFFECT)	// 地形効果
			{
				onEvent(tutorialId);
				return;
			}
		}

		if (tutorialIdList_.size() == 1) // 残メッセージが１つの場合に表示
		{
			int tutorialId = tutorialIdList_.at(0);
			if (tutorialId == TutorialId::ALL_KILL ||		// 殲滅
				tutorialId == TutorialId::SUPPRESSION)	// 制圧
			{
				onEvent(tutorialId);
			}
		}
	}

	/**
	 * @fn
	 * 戦闘情報をもとにチュートリアルメッセージ表示
	 * @param (figth) 戦闘データ
	 * @param (phase) 戦闘フェイズ
	 */
	void TutorialManager::onFight(const Fight* fight, FightPhase phase)
	{

		for (auto itr = tutorialIdList_.begin(); itr != tutorialIdList_.end(); ++itr)
		{
			int tutorialId = *itr;
			if (phase == FightPhase::PREDICT)
			{
				if (tutorialId == TutorialId::RUSH_CANCEL) // 突撃無効化が残っている場合は表示しない
				{
					return;
				}
				else if (tutorialId == TutorialId::AMBUSH_CANCEL)
				{
					const FightData psvData = fight->getFightData(false);
					if (psvData.unit->getInfo().kind == UnitKey::LANCER)
					{
						onEvent(tutorialId);
						return;
					}
				}
				else if (tutorialId == TutorialId::THROUGH) // 貫通
				{
					onEvent(tutorialId);
					return;
				}
				else if (tutorialId == TutorialId::HIT_ATTENUATION) // 距離減衰
				{
					const FightData actData = fight->getFightData(true);
					if (actData.hitRate < 100) // 命中率が下がっている場合
					{
						onEvent(tutorialId);
						return;
					}
				}
			}
			else if (phase == FightPhase::START)
			{

			}
			else if (phase == FightPhase::END)
			{
				if (tutorialId == TutorialId::RUSH_CANCEL)
				{
					const FightData psvData = fight->getFightData(false);
					if (psvData.unit->getInfo().kind == UnitKey::LANCER)
					{
						onEvent(tutorialId);
					}
					return;
				}
			}
		}
	}

	/**
	 * @fn
	 * ステージIDをもとにチュートリアルIDリストをセット
	 * @param (sategeId) ステージID
	 */
	bool TutorialManager::setTutorialIdList(int stageId)
	{
		if (stageId == 0)
		{
			tutorialIdList_.push_back(TutorialId::UNIT_SELECT);
			tutorialIdList_.push_back(TutorialId::TURN_END);
			tutorialIdList_.push_back(TutorialId::MOVE);
			tutorialIdList_.push_back(TutorialId::MOVE_CONFIRM);
			tutorialIdList_.push_back(TutorialId::FIGHT_PREDICT);
			tutorialIdList_.push_back(TutorialId::ATACK);
			tutorialIdList_.push_back(TutorialId::ALL_KILL);
		}
		else if (stageId == 1)
		{
			tutorialIdList_.push_back(TutorialId::TERRAIN_EFFECT);
			tutorialIdList_.push_back(TutorialId::COORDINATED);
			tutorialIdList_.push_back(TutorialId::PINCHING);
			tutorialIdList_.push_back(TutorialId::SUPPRESSION);
		}
		else if (stageId == 2)
		{
			tutorialIdList_.push_back(TutorialId::DEFFENCE);
			tutorialIdList_.push_back(TutorialId::AMBUSH);
		}
		else if (stageId == 3)
		{
			tutorialIdList_.push_back(TutorialId::RUSH);
			tutorialIdList_.push_back(TutorialId::RUSH_CANCEL);
			tutorialIdList_.push_back(TutorialId::AMBUSH_CANCEL);
		}
		else if (stageId == 4)
		{
			tutorialIdList_.push_back(TutorialId::RUSH_NOT_PLAIN);
			tutorialIdList_.push_back(TutorialId::THROUGH);
			tutorialIdList_.push_back(TutorialId::LOADING);
			tutorialIdList_.push_back(TutorialId::HIT_ATTENUATION);
		}
		else if (stageId == 5)
		{
			tutorialIdList_.push_back(TutorialId::FREE_SET_SELECT);
			tutorialIdList_.push_back(TutorialId::FREE_SET_DEL);
			tutorialIdList_.push_back(TutorialId::FREE_SET_SET);
			tutorialIdList_.push_back(TutorialId::FREE_SET_FIN);
		}
		else
		{
			return false; // チュートリアルがセットされていないIDの場合は無効化
		}

		return true;
	}
}