#include "TutorialManager.h"
#include "BattleManager.h"
#include "BUI/SelectUnitArea.h"

namespace Battle {
	const vector<string> TutorialManager::MESSAGES = TutorialManager::initMessages();

	/**
	 * @fn
	 * �`���[�g���A��ID�ɑΉ����郁�b�Z�[�W���`
	 */
	vector<string> TutorialManager::initMessages()
	{
		vector<string> messageList(TutorialId::TUTORIAL_ID_LEN);

		messageList.at(TutorialId::UNIT_SELECT) =		"���j�b�g���N���b�N���邱�Ƃ�\n�N���b�N�������j�b�g��I�����邱�Ƃ��ł��܂�";
		messageList.at(TutorialId::TURN_END) =			"�E���́u�^�[���I���v�{�^��\n�܂��́A�E�N���b�N�ŕ\�������V�X�e�����j���[����\n�����̃^�[�����I���ł��܂�";
		messageList.at(TutorialId::MOVE) =				"���Ȃ��Ă���}�X���N���b�N�����\n���j�b�g���N���b�N�����}�X�Ɉړ��������܂�";
		messageList.at(TutorialId::MOVE_CONFIRM) =		"�I�𒆂̃��j�b�g���N���b�N����ƁA�ړ����m�肳�����܂�\n�I�𒆂̃��j�b�g�ȊO���N���b�N����ƁA�ړ����L�����Z���ł��܂�";
		messageList.at(TutorialId::FIGHT_PREDICT) =		"�@�G���j�b�g�ɗאڂ���悤�Ɉړ�\n�A�אڂ����G���j�b�g�Ƀ}�E�X�J�[�\�������킹��\n��L�菇�ŉ�ʉ��ɐ퓬�\�����\������܂�";
		messageList.at(TutorialId::ATACK) =				"�Ԃ��Ȃ��Ă���}�X�̓G���j�b�g��\n�N���b�N����ƍU���ł��܂�";
		messageList.at(TutorialId::TERRAIN_EFFECT) =	"�X�}�X�Ƀ��j�b�g��u���ƁA�����h�䂪�㏸���܂�\n�i�n�`���ʂ́A�}�X�̏�ɃJ�[�\����u���ƍ����ɕ\������܂��j";
		messageList.at(TutorialId::FREE_SET_SELECT) =	"��������ݒu���������j�b�g��I�����Ă�������";
		messageList.at(TutorialId::FREE_SET_SET) =		"���}�X���N���b�N���邱�Ƃ�\n�I�𒆂̃��j�b�g��ݒu�ł��܂�";
		messageList.at(TutorialId::FREE_SET_DEL) =		"�ݒu�������j�b�g���N���b�N���邱�Ƃ�\n�ݒu�����������Ƃ��ł��܂�";
		messageList.at(TutorialId::FREE_SET_FIN) =		"���j�b�g�̐ݒu�����ׂĊ���������\n�E���́u���������v�{�^�����N���b�N���Ă�������";
		messageList.at(TutorialId::COORDINATED) =		"�U����A���̃��j�b�g�ő��ʂ���U�����邱�Ƃ�\n�u�A�g�v�U���ɂȂ�A�_���[�W���㏸���܂�";
		messageList.at(TutorialId::PINCHING) =			"�U����A���̃��j�b�g�Ŕ��Α�����U�����邱�Ƃ�\n�u�����v�ɂȂ�A�_���[�W���啝�ɏ㏸���܂�";
		messageList.at(TutorialId::ALL_KILL) =			"�G���j�b�g��S�ł����邱�Ƃŏ����ɂȂ�܂�";
		messageList.at(TutorialId::DEFFENCE) =			"�G���j�b�g�����R�̍ԂɈړ�����Ɣs�k�ɂȂ�܂�";
		messageList.at(TutorialId::SUPPRESSION) =		"�G�R�̍ԂɈړ����邱�Ƃŏ����ɂȂ�܂�";
		messageList.at(TutorialId::AMBUSH) =			"�������R���i���������˒��̒Z�����j�b�g�j�ɍU�����ꂽ��\n�X�L���u�}���v�𔭓����܂�\n�u�}���v�𔭓�����ƁA���������搧�U�����܂�";
		messageList.at(TutorialId::AMBUSH_CANCEL) =		"�u�A�g�v�܂��́u�����v�������\n�����̃X�L���u�}���v�𖳌����ł��܂�";
		messageList.at(TutorialId::RUSH) =				"�R���͎�������U���������ɃX�L���u�ˌ��v�𔭓����܂��B\n�u�ˌ��v�𔭓�����ƁA�_���[�W���㏸���܂��B";
		messageList.at(TutorialId::RUSH_CANCEL) =		"�u�}���v���󂯂��ꍇ�́A�u�ˌ��v�͔������܂���";
		messageList.at(TutorialId::RUSH_NOT_PLAIN) =	"�u�����v�}�X�ȊO�ł́u�ˌ��v�͔������܂���";
		messageList.at(TutorialId::THROUGH) =			"�e���̓X�L���u�ђʁv�ɂ��A�G���j�b�g�̖h��͂𖳎�����\n�_���[�W��^���邱�Ƃ��ł��܂�";
		messageList.at(TutorialId::SHOOTING) =			"�e���́u�ˌ��v�U���̏ꍇ\n������R���̂悤�ȁu�ߐځv�U���ƈႢ\n�u�A�g�v��u�����v�͔������܂���";
		messageList.at(TutorialId::LOADING) =			"�e���͍U���������̃^�[���ɍU���s�ɂȂ�܂�";
		messageList.at(TutorialId::HIT_ATTENUATION) =	"�e���͗��ꂽ�G���U�����悤�Ƃ����ꍇ�A���������������܂�";

		return messageList;
	}

	/**
	 * @fn
	 * ��������
	 * @param (sategeId) �X�e�[�WID
	 */
	void TutorialManager::init(int stageId, shared_ptr<Message> message)
	{
		isAble_ = setTutorialIdList(stageId);
		message_ = message;
	}

	/**
	 * @fn
	 * �s���\�ȃ��j�b�g�����邩����
	 * @return �s���\�ȃ��j�b�g�����݂���ꍇ true��Ԃ�
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
	 * �K���ȍŏ��̃��j�b�g�̍��W�擾(�����W�p�̒����܂�)
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
	 * �Ώۃ��j�b�g�̍��W���擾(�����W�p�̒����܂�)
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
	 * ����C�x���g�������ɁA�Z�b�g����Ă���`���[�g���A��ID������΃��b�Z�[�W�\��
	 * @return ���b�Z�[�W���\�����ꂽ�ꍇ true��Ԃ�
	 */
	bool TutorialManager::onEvent(int tutorialId, BattleManager* bm, int x, int y)
	{
		if (!isAble_)
		{
			return false; // �������
		}

		for (auto itr = tutorialIdList_.begin(); itr != tutorialIdList_.end(); ++itr)
		{
			if (*itr == tutorialId)
			{
				if (tutorialId == TutorialId::MOVE) // �ړ�
				{
					if (bm->map->getMass(MOVE_X, MOVE_Y)->state == Mass::State::MOVABLE)
					{
						x = Map::getRealX(MOVE_X) + CHIP_SIZE / 2;
						y = Map::getRealY(MOVE_Y);
					}
				}
				else if (tutorialId == TutorialId::MOVE_CONFIRM) // �ړ��m��
				{
					getUnitPos(bm->getSelectedUnit(), &x, &y);
				}
				else if (tutorialId == TutorialId::ATACK) // �퓬
				{
					getFirstUnitPos(bm->map, true, &x, &y);
				}
				else if (tutorialId == TutorialId::FREE_SET_SELECT) // ���R�I��
				{
					x = BUI_PADDING + SelectUnitArea::COUNT_WIDTH + SelectUnitArea::UNIT_MARGIN +  CHIP_SIZE / 2;
					y = WIN_H - BUI_H;
				}
				else if (tutorialId == TutorialId::FREE_SET_SET) // ���R�ݒu
				{
					x = Map::getRealX(FREE_SET_X) + CHIP_SIZE / 2;
					y = Map::getRealY(FREE_SET_Y);
				}

				// �`���[�g���A�����b�Z�[�W�\��
				showTutorial(*itr, x, y);
				tutorialIdList_.erase(itr);
				return true;
			}
		}

		return false;
	}

	/**
	 * @fn
	 * �v���C���[�^�[���J�n���ɁA����̃`���[�g���A�����b�Z�[�W��\��
	 */
	void TutorialManager::onPlayerTurnStart(BattleManager* bm)
	{
		if (!isAble_)
		{
			return; // �������
		}

		int targetId = -1;
		int x = -1;
		int y = -1;

		for (auto itr = tutorialIdList_.begin(); itr != tutorialIdList_.end(); ++itr)
		{
			int tutorialId = *itr;
			if (tutorialId == TutorialId::AMBUSH ||			// �}��
				tutorialId == TutorialId::RUSH ||			// �ˌ�
				tutorialId == TutorialId::RUSH_NOT_PLAIN ||	// �ˌ�(�����ȊO)
				tutorialId == TutorialId::LOADING && tutorialIdList_.size() < 3)		// �e����
			{
				targetId = tutorialId;
				tutorialIdList_.erase(itr);
				break;
			}
			else if (tutorialId == TutorialId::UNIT_SELECT) // ���j�b�g�I��
			{
				getFirstUnitPos(bm->map, false, &x, &y);
				targetId = tutorialId;
				tutorialIdList_.erase(itr);
				break;
			}
			else if (tutorialId == TutorialId::FIGHT_PREDICT) // �퓬�\��
			{
				getFirstUnitPos(bm->map, true, &x, &y);

				targetId = tutorialId;
				tutorialIdList_.erase(itr);
				break;
			}
			else if (tutorialId == TutorialId::TERRAIN_EFFECT) // �n�`����
			{
				x = Map::getRealX(FOREST_X) + CHIP_SIZE / 2;
				y = Map::getRealY(FOREST_Y);
				targetId = tutorialId;
				tutorialIdList_.erase(itr);
				break;
			}
			else if (tutorialId == TutorialId::DEFFENCE)	// �h�q
			{
				x = Map::getRealX(DEFFENCE_X) + CHIP_SIZE / 2;
				y = Map::getRealY(DEFFENCE_Y);
				targetId = tutorialId;
				tutorialIdList_.erase(itr);
				break;
			}
		}

		if (targetId == -1 && tutorialIdList_.size() == 1) // �c���b�Z�[�W���P�̏ꍇ�ɕ\��
		{
			int tutorialId = tutorialIdList_.at(0);
			if (tutorialId == TutorialId::ALL_KILL)		// �r��
			{
				targetId = tutorialId;
				tutorialIdList_.erase(tutorialIdList_.begin());
			}
			else if (tutorialId == TutorialId::SUPPRESSION)	// ����
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
	 * �퓬�������ƂɃ`���[�g���A�����b�Z�[�W�\��
	 * @param (figth) �퓬�f�[�^
	 * @param (phase) �퓬�t�F�C�Y
	 */
	void TutorialManager::onFight(const Fight* fight, FightPhase phase, BattleManager* bm)
	{
		if (!isAble_)
		{
			return; // �������
		}

		// ���폜����
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

		//! �����W
		int x = -1;
		int y = -1;

		// �퓬�֘A�`���[�g���A���\������
		for (auto itr = tutorialIdList_.begin(); itr != tutorialIdList_.end(); ++itr)
		{
			int tutorialId = *itr;
			if (phase == FightPhase::PREDICT)
			{
				if (tutorialId == TutorialId::RUSH_CANCEL) // �ˌ����������c���Ă���ꍇ�͕\�����Ȃ�
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
				else if (tutorialId == TutorialId::THROUGH || tutorialId == TutorialId::SHOOTING) // �ђ� or �ˌ�
				{
					showTutorial(tutorialId);
					tutorialIdList_.erase(itr);
					return;
				}
				else if (tutorialId == TutorialId::HIT_ATTENUATION && bm->isPlayerTurn()) // ��������
				{
					const FightData actData = fight->getFightData(true);
					if (actData.hitRate < 100) // ���������������Ă���ꍇ
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
				if (tutorialId == TutorialId::RUSH_CANCEL) // �ˌ��L�����Z��
				{
					const FightData psvData = fight->getFightData(false);
					if (psvData.unit->getInfo().kind == UnitKey::LANCER)
					{
						showTutorial(tutorialId);
						tutorialIdList_.erase(itr);
					}
					return;
				}
				else if (tutorialId == TutorialId::COORDINATED && bm->isPlayerTurn()) // �A�g
				{
					vector<pair<int, int>> posList;

					const FightData psvData = fight->getFightData(false);
					const FightData actData = fight->getFightData(true);

					// �G���j�b�g���W
					int psvMassX = psvData.unit->getMassX();
					int psvMassY = psvData.unit->getMassY();

					if (psvMassX == actData.unit->getMassX()) // �c�����ōU�������ꍇ
					{
						posList.push_back(make_pair(Map::getRealX(psvMassX - 1) + CHIP_SIZE / 2, Map::getRealY(psvMassY)));
						posList.push_back(make_pair(Map::getRealX(psvMassX + 1) + CHIP_SIZE / 2, Map::getRealY(psvMassY)));
					}
					else // �������ōU�������ꍇ
					{
						posList.push_back(make_pair(Map::getRealX(psvMassX) + CHIP_SIZE / 2, Map::getRealY(psvMassY - 1)));
						posList.push_back(make_pair(Map::getRealX(psvMassX) + CHIP_SIZE / 2, Map::getRealY(psvMassY + 1)));
					}

					showTutorial(tutorialId, posList);
					tutorialIdList_.erase(itr);
					return;
				}
				else if (tutorialId == TutorialId::PINCHING && bm->isPlayerTurn()) // ����
				{
					if (!isExistActableUnit(bm->map)) 
					{
						return; // �s���\�ȃ��j�b�g�����Ȃ��ꍇ�̓p�X
					}

					const FightData psvData = fight->getFightData(false);
					const FightData actData = fight->getFightData(true);

					// �G���j�b�g���W
					int psvMassX = psvData.unit->getMassX();
					int psvMassY = psvData.unit->getMassY();

					// ���j�b�g�U���ʒu�̔��Α��̍��W
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
	 * �`���[�g���A���\��
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
	 * �`���[�g���A���\��
	 */
	void TutorialManager::showTutorial(int tutorialId, vector<pair<int, int>>& arrowPosList)
	{
		message_->show(MESSAGES[tutorialId], true, MESSAGE_MS);

		clearArrowList();

		// �`���[�g���A�����ǉ�
		for (auto itr = arrowPosList.begin(); itr != arrowPosList.end(); ++itr)
		{
			arrowList_.push_back(TutorialArrow::makeTutorialArrow((*itr).first, (*itr).second));
		}

		prevTutorialId_ = tutorialId;
	}

	/**
	 * @fn
	 * �X�e�[�WID�����ƂɃ`���[�g���A��ID���X�g���Z�b�g
	 * @param (sategeId) �X�e�[�WID
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
			return false; // �`���[�g���A�����Z�b�g����Ă��Ȃ�ID�̏ꍇ�͖�����
		}

		return true;
	}

	/**
	 * @fn
	 * �`���[�g���A���p�����폜
	 */
	void TutorialManager::clearArrowList()
	{
		prevTutorialId_ = -1;

		// �����̃`���[�g���A�����폜
		for (auto itr = arrowList_.begin(); itr != arrowList_.end(); ++itr)
		{
			(*itr)->destroy();
		}

		arrowList_.clear();
	}
}