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

		messageList.at(TutorialId::UNIT_SELECT) =		"ユニットをクリックして\nユニットを選択してください";
		messageList.at(TutorialId::STATUS_CHECK) =		"ユニットを選択すると\n画面したにユニットのステータスが表示されます";
		messageList.at(TutorialId::MOVE) =				"青くなっているマスを選択すると\nユニットが移動します";
		messageList.at(TutorialId::MOVE_CANCEL) =		"選択中のユニットと赤くなっているマス以外を\nクリックすると移動をキャンセルできます";
		messageList.at(TutorialId::FIGHT_PREDICT) =		"赤くなっているマスの上にマウスカーソルを置くと\n画面下に戦闘予測が表示されます";
		messageList.at(TutorialId::ATACK) =				"赤くなっているますをクリックすると敵ユニットに攻撃可能です";
		messageList.at(TutorialId::TERRAIN_EFFECT) =	"森になっているマスにユニットを置くと\n回避や防御が上昇します\n（地形効果は、マスの上にカーソルを置くと左下に表示されます。）";
		messageList.at(TutorialId::FREE_SET_SELECT) =	"左下から設置したいユニットを選択してください";
		messageList.at(TutorialId::FREE_SET_SET) =		"青いマスをクリックすることで\n選択中のユニットを設置できます";
		messageList.at(TutorialId::FREE_SET_DEL) =		"設置したユニットをクリックすることで\n設置を取り消すことができます";
		messageList.at(TutorialId::FREE_SET_FIN) =		"ユニットの設置がすべて完了したら、右下の「準備完了」ボタンをクリックしてください";
		messageList.at(TutorialId::COORDINATED) =		"攻撃後、他のユニットで側面から攻撃することで\n「連携」攻撃になり、ダメージが上昇します。";
		messageList.at(TutorialId::PINCHING) =			"攻撃後、他のユニットで反対側から攻撃することで\n「挟撃」になり、ダメージが大幅に上昇します。";
		messageList.at(TutorialId::ALL_KILL) =			"敵ユニットを全滅させることで勝利になります";
		messageList.at(TutorialId::DEFFENCE) =			"敵ユニットが自軍の砦に移動すると敗北になります";
		messageList.at(TutorialId::SUPPRESSION) =		"敵軍の砦に移動することで勝利になります";
		messageList.at(TutorialId::AMBUSH) =			"槍兵が騎兵（自分よりも射程の短いユニット）に攻撃された時、スキル「迎撃」を発動します。\n「迎撃」を発動すると、先制で攻撃します。";
		messageList.at(TutorialId::AMBUSH_CANCE) =		"槍兵は「連携」または「挟撃」を受けた場合は\nスキル「迎撃」を発動できません";
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
	 * 特定イベント発生時に、
	 */
	void TutorialManager::onEvent(int tutorialId)
	{
		if (!isAble_)
		{
			return; // 無効状態
		}

		for (auto itr = tutorialIdList_.begin(); itr != tutorialIdList_.end(); ++itr)
		{
			if (*itr == tutorialId)
			{
				// チュートリアルメッセージ表示
				message_->show(MESSAGES[tutorialId], true);
				tutorialIdList_.erase(itr);
				break;
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
			// tutorialIdList_.push_back(TutorialId::STATUS_CHECK);
			tutorialIdList_.push_back(TutorialId::MOVE);
			tutorialIdList_.push_back(TutorialId::MOVE_CANCEL);
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
			tutorialIdList_.push_back(TutorialId::AMBUSH_CANCE);
			tutorialIdList_.push_back(TutorialId::RUSH);
			tutorialIdList_.push_back(TutorialId::RUSH_CANCEL);
		}
		else if (stageId == 4)
		{
			tutorialIdList_.push_back(TutorialId::RUSH_NOT_PLAIN);
			tutorialIdList_.push_back(TutorialId::THROUGH);
			tutorialIdList_.push_back(TutorialId::LOADING);
			tutorialIdList_.push_back(TutorialId::HIT_ATTENUATION);
		}
		else
		{
			return false; // チュートリアルがセットされていないIDの場合は無効化
		}

		return true;
	}
}