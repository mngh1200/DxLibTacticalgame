#include "TutorialManager.h"
#include "BattleManager.h"
#include "BUI/SelectUnitArea.h"

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
		messageList.at(TutorialId::MOVE) =				"青くなっているマスをクリックすると\nユニットをクリックしたマスに移動させられます";
		messageList.at(TutorialId::MOVE_CONFIRM) =		"選択中のユニットをクリックすると、移動を確定させられます\n選択中のユニット以外をクリックすると、移動をキャンセルできます";
		messageList.at(TutorialId::FIGHT_PREDICT) =		"①敵ユニットに隣接するように移動\n②隣接した敵ユニットにマウスカーソルを合わせる\n上記手順で画面下に戦闘予測が表示されます";
		messageList.at(TutorialId::ATACK) =				"赤くなっているマスの敵ユニットを\nクリックすると攻撃できます";
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
		messageList.at(TutorialId::SHOOTING) =			"銃兵の「射撃」攻撃の場合\n槍兵や騎兵のような「近接」攻撃と違い\n「連携」や「挟撃」は発動しません";
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
	 * 行動可能なユニットがいるか判定
	 * @return 行動可能なユニットが存在する場合 trueを返す
	 */
	bool isExistActableUnit(shared_ptr<Map> map)
	{
		for (auto itr = map->unitsBegin(); itr != map->unitsEnd(); ++itr)
		{
			shared_ptr<Unit> unit = (*itr).second;
			if (!unit->isEnemy() && !unit->isActed())
			{
				return true;
			}
		}
		return false;
	}

	/**
	 * @fn
	 * 適当な最初のユニットの座標取得(矢印座標用の調整含む)
	 */
	void getFirstUnitPos(shared_ptr<Map> map, bool isEnemy, int* x, int* y)
	{
		for (auto itr = map->unitsBegin(); itr != map->unitsEnd(); ++itr)
		{
			if ((*itr).second->isEnemy() == isEnemy)
			{
				*x = Map::getRealX((*itr).first.first) + CHIP_SIZE / 2;
				*y = Map::getRealY((*itr).first.second);
				break;
			}
		}
	}

	/**
	 * @fn
	 * 対象ユニットの座標を取得(矢印座標用の調整含む)
	 */
	void getUnitPos(shared_ptr<Unit> unit, int* x, int* y)
	{
		if (unit)
		{
			*x = unit->getX() + CHIP_SIZE / 2;
			*y = unit->getY();
		}
	}

	/**
	 * @fn
	 * 特定イベント発生時に、セットされているチュートリアルIDがあればメッセージ表示
	 * @return メッセージが表示された場合 trueを返す
	 */
	bool TutorialManager::onEvent(int tutorialId, BattleManager* bm, int x, int y)
	{
		if (!isAble_)
		{
			return false; // 無効状態
		}

		for (auto itr = tutorialIdList_.begin(); itr != tutorialIdList_.end(); ++itr)
		{
			if (*itr == tutorialId)
			{
				if (tutorialId == TutorialId::MOVE) // 移動
				{
					if (bm->map->getMass(MOVE_X, MOVE_Y)->state == Mass::State::MOVABLE)
					{
						x = Map::getRealX(MOVE_X) + CHIP_SIZE / 2;
						y = Map::getRealY(MOVE_Y);
					}
				}
				else if (tutorialId == TutorialId::MOVE_CONFIRM) // 移動確定
				{
					getUnitPos(bm->getSelectedUnit(), &x, &y);
				}
				else if (tutorialId == TutorialId::ATACK) // 戦闘
				{
					getFirstUnitPos(bm->map, true, &x, &y);
				}
				else if (tutorialId == TutorialId::FREE_SET_SELECT) // 自由選択
				{
					x = BUI_PADDING + SelectUnitArea::COUNT_WIDTH + SelectUnitArea::UNIT_MARGIN +  CHIP_SIZE / 2;
					y = WIN_H - BUI_H;
				}
				else if (tutorialId == TutorialId::FREE_SET_SET) // 自由設置
				{
					x = Map::getRealX(FREE_SET_X) + CHIP_SIZE / 2;
					y = Map::getRealY(FREE_SET_Y);
				}

				// チュートリアルメッセージ表示
				showTutorial(*itr, x, y);
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
	void TutorialManager::onPlayerTurnStart(BattleManager* bm)
	{
		if (!isAble_)
		{
			return; // 無効状態
		}

		int targetId = -1;
		int x = -1;
		int y = -1;

		for (auto itr = tutorialIdList_.begin(); itr != tutorialIdList_.end(); ++itr)
		{
			int tutorialId = *itr;
			if (tutorialId == TutorialId::AMBUSH ||			// 迎撃
				tutorialId == TutorialId::RUSH ||			// 突撃
				tutorialId == TutorialId::RUSH_NOT_PLAIN ||	// 突撃(草原以外)
				tutorialId == TutorialId::LOADING && tutorialIdList_.size() < 3)		// 弾込め
			{
				targetId = tutorialId;
				tutorialIdList_.erase(itr);
				break;
			}
			else if (tutorialId == TutorialId::UNIT_SELECT) // ユニット選択
			{
				getFirstUnitPos(bm->map, false, &x, &y);
				targetId = tutorialId;
				tutorialIdList_.erase(itr);
				break;
			}
			else if (tutorialId == TutorialId::FIGHT_PREDICT) // 戦闘予測
			{
				getFirstUnitPos(bm->map, true, &x, &y);

				targetId = tutorialId;
				tutorialIdList_.erase(itr);
				break;
			}
			else if (tutorialId == TutorialId::TERRAIN_EFFECT) // 地形効果
			{
				x = Map::getRealX(FOREST_X) + CHIP_SIZE / 2;
				y = Map::getRealY(FOREST_Y);
				targetId = tutorialId;
				tutorialIdList_.erase(itr);
				break;
			}
			else if (tutorialId == TutorialId::DEFFENCE)	// 防衛
			{
				x = Map::getRealX(DEFFENCE_X) + CHIP_SIZE / 2;
				y = Map::getRealY(DEFFENCE_Y);
				targetId = tutorialId;
				tutorialIdList_.erase(itr);
				break;
			}
		}

		if (targetId == -1 && tutorialIdList_.size() == 1) // 残メッセージが１つの場合に表示
		{
			int tutorialId = tutorialIdList_.at(0);
			if (tutorialId == TutorialId::ALL_KILL)		// 殲滅
			{
				targetId = tutorialId;
				tutorialIdList_.erase(tutorialIdList_.begin());
			}
			else if (tutorialId == TutorialId::SUPPRESSION)	// 制圧
			{
				x = Map::getRealX(SUPPRESSION_X) + CHIP_SIZE / 2;
				y = Map::getRealY(SUPPRESSION_Y);
				targetId = tutorialId;
				tutorialIdList_.erase(tutorialIdList_.begin());
			}
		}

		if (targetId != -1)
		{
			showTutorial(targetId, x, y);
		}

	}

	/**
	 * @fn
	 * 戦闘情報をもとにチュートリアルメッセージ表示
	 * @param (figth) 戦闘データ
	 * @param (phase) 戦闘フェイズ
	 */
	void TutorialManager::onFight(const Fight* fight, FightPhase phase, BattleManager* bm)
	{
		if (!isAble_)
		{
			return; // 無効状態
		}

		// 矢印削除判定
		if (phase == FightPhase::PREDICT)
		{
			if (prevTutorialId_ == TutorialId::TERRAIN_EFFECT)
			{
				clearArrowList();
			}
		}
		if (phase == FightPhase::START)
		{
			if (prevTutorialId_ == TutorialId::ATACK ||
				prevTutorialId_ == TutorialId::COORDINATED ||
				prevTutorialId_ == TutorialId::PINCHING)
			{
				clearArrowList();
			}
		}

		//! 矢印座標
		int x = -1;
		int y = -1;

		// 戦闘関連チュートリアル表示判定
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
						showTutorial(tutorialId);
						tutorialIdList_.erase(itr);
						return;
					}
				}
				else if (tutorialId == TutorialId::THROUGH || tutorialId == TutorialId::SHOOTING) // 貫通 or 射撃
				{
					showTutorial(tutorialId);
					tutorialIdList_.erase(itr);
					return;
				}
				else if (tutorialId == TutorialId::HIT_ATTENUATION && bm->isPlayerTurn()) // 距離減衰
				{
					const FightData actData = fight->getFightData(true);
					if (actData.hitRate < 100) // 命中率が下がっている場合
					{
						showTutorial(tutorialId);
						tutorialIdList_.erase(itr);
						return;
					}
				}
			}
			else if (phase == FightPhase::START)
			{

			}
			else if (phase == FightPhase::END)
			{
				if (tutorialId == TutorialId::RUSH_CANCEL) // 突撃キャンセル
				{
					const FightData psvData = fight->getFightData(false);
					if (psvData.unit->getInfo().kind == UnitKey::LANCER)
					{
						showTutorial(tutorialId);
						tutorialIdList_.erase(itr);
					}
					return;
				}
				else if (tutorialId == TutorialId::COORDINATED && bm->isPlayerTurn()) // 連携
				{
					vector<pair<int, int>> posList;

					const FightData psvData = fight->getFightData(false);
					const FightData actData = fight->getFightData(true);

					// 敵ユニット座標
					int psvMassX = psvData.unit->getMassX();
					int psvMassY = psvData.unit->getMassY();

					if (psvMassX == actData.unit->getMassX()) // 縦方向で攻撃した場合
					{
						posList.push_back(make_pair(Map::getRealX(psvMassX - 1) + CHIP_SIZE / 2, Map::getRealY(psvMassY)));
						posList.push_back(make_pair(Map::getRealX(psvMassX + 1) + CHIP_SIZE / 2, Map::getRealY(psvMassY)));
					}
					else // 横方向で攻撃した場合
					{
						posList.push_back(make_pair(Map::getRealX(psvMassX) + CHIP_SIZE / 2, Map::getRealY(psvMassY - 1)));
						posList.push_back(make_pair(Map::getRealX(psvMassX) + CHIP_SIZE / 2, Map::getRealY(psvMassY + 1)));
					}

					showTutorial(tutorialId, posList);
					tutorialIdList_.erase(itr);
					return;
				}
				else if (tutorialId == TutorialId::PINCHING && bm->isPlayerTurn()) // 挟撃
				{
					if (!isExistActableUnit(bm->map)) 
					{
						return; // 行動可能なユニットがいない場合はパス
					}

					const FightData psvData = fight->getFightData(false);
					const FightData actData = fight->getFightData(true);

					// 敵ユニット座標
					int psvMassX = psvData.unit->getMassX();
					int psvMassY = psvData.unit->getMassY();

					// ユニット攻撃位置の反対側の座標
					x = Map::getRealX(psvMassX + psvMassX - actData.unit->getMassX()) + CHIP_SIZE / 2;
					y = Map::getRealY(psvMassY + psvMassY - actData.unit->getMassY());

					showTutorial(tutorialId, x, y);
					tutorialIdList_.erase(itr);
					return;
				}
			}
		}
	}

	/**
	 * @fn
	 * チュートリアル表示
	 */
	void TutorialManager::showTutorial(int tutorialId, int x, int y)
	{
		vector<pair<int, int>> arrowPosList;
		if (x != -1 && y != -1)
		{
			arrowPosList.push_back(make_pair(x, y));
		}
		showTutorial(tutorialId, arrowPosList);
	}

	/**
	 * @fn
	 * チュートリアル表示
	 */
	void TutorialManager::showTutorial(int tutorialId, vector<pair<int, int>>& arrowPosList)
	{
		message_->show(MESSAGES[tutorialId], true, MESSAGE_MS);

		clearArrowList();

		// チュートリアル矢印追加
		for (auto itr = arrowPosList.begin(); itr != arrowPosList.end(); ++itr)
		{
			arrowList_.push_back(TutorialArrow::makeTutorialArrow((*itr).first, (*itr).second));
		}

		prevTutorialId_ = tutorialId;
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
			// tutorialIdList_.push_back(TutorialId::RUSH_NOT_PLAIN);
			tutorialIdList_.push_back(TutorialId::THROUGH);
			tutorialIdList_.push_back(TutorialId::SHOOTING);
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

	/**
	 * @fn
	 * チュートリアル用矢印を削除
	 */
	void TutorialManager::clearArrowList()
	{
		prevTutorialId_ = -1;

		// 既存のチュートリアル矢印削除
		for (auto itr = arrowList_.begin(); itr != arrowList_.end(); ++itr)
		{
			(*itr)->destroy();
		}

		arrowList_.clear();
	}
}