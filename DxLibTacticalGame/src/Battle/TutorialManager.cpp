#include "TutorialManager.h"

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
		messageList.at(TutorialId::MOVE) =				"���Ȃ��Ă���}�X��I�������\n���j�b�g���N���b�N�����}�X�Ɉړ��������܂�";
		messageList.at(TutorialId::MOVE_CONFIRM) =		"�I�𒆂̃��j�b�g���N���b�N����ƁA�ړ����m�肳�����܂�\n�I�𒆂̃��j�b�g�ȊO���N���b�N����ƁA�ړ����L�����Z���ł��܂�";
		messageList.at(TutorialId::FIGHT_PREDICT) =		"�@�G���j�b�g�ɗאڂ���悤�Ɉړ�\n�A�אڂ����G���j�b�g�Ƀ}�E�X�J�[�\�������킹��\n��L�菇�ŉ�ʉ��ɐ퓬�\�����\������܂�";
		messageList.at(TutorialId::ATACK) =				"�Ԃ��Ȃ��Ă���}�X�̓G���j�b�g���N���b�N����ƍU���ł��܂�";
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
	 * ����C�x���g�������ɁA�Z�b�g����Ă���`���[�g���A��ID������΃��b�Z�[�W�\��
	 * @return ���b�Z�[�W���\�����ꂽ�ꍇ true��Ԃ�
	 */
	bool TutorialManager::onEvent(int tutorialId)
	{
		if (!isAble_)
		{
			return false; // �������
		}

		for (auto itr = tutorialIdList_.begin(); itr != tutorialIdList_.end(); ++itr)
		{
			if (*itr == tutorialId)
			{
				// �`���[�g���A�����b�Z�[�W�\��
				message_->show(MESSAGES[tutorialId], true);
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
	void TutorialManager::onPlayerTurnStart(shared_ptr<Map> map)
	{
		if (!isAble_)
		{
			return; // �������
		}

		for (auto itr = tutorialIdList_.begin(); itr != tutorialIdList_.end(); ++itr)
		{
			int tutorialId = *itr;
			if (tutorialId == TutorialId::UNIT_SELECT ||	// ���j�b�g�I��
				tutorialId == TutorialId::FIGHT_PREDICT ||	// �퓬�\��
				tutorialId == TutorialId::PINCHING ||		// ����
				tutorialId == TutorialId::AMBUSH ||			// �}��
				tutorialId == TutorialId::DEFFENCE ||		// �h�q
				tutorialId == TutorialId::RUSH ||			// �ˌ�
				tutorialId == TutorialId::RUSH_NOT_PLAIN ||	// �ˌ�(�����ȊO)
				tutorialId == TutorialId::LOADING ||		// �e����
				tutorialId == TutorialId::TERRAIN_EFFECT)	// �n�`����
			{
				onEvent(tutorialId);
				return;
			}
		}

		if (tutorialIdList_.size() == 1) // �c���b�Z�[�W���P�̏ꍇ�ɕ\��
		{
			int tutorialId = tutorialIdList_.at(0);
			if (tutorialId == TutorialId::ALL_KILL ||		// �r��
				tutorialId == TutorialId::SUPPRESSION)	// ����
			{
				onEvent(tutorialId);
			}
		}
	}

	/**
	 * @fn
	 * �퓬�������ƂɃ`���[�g���A�����b�Z�[�W�\��
	 * @param (figth) �퓬�f�[�^
	 * @param (phase) �퓬�t�F�C�Y
	 */
	void TutorialManager::onFight(const Fight* fight, FightPhase phase)
	{

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
						onEvent(tutorialId);
						return;
					}
				}
				else if (tutorialId == TutorialId::THROUGH) // �ђ�
				{
					onEvent(tutorialId);
					return;
				}
				else if (tutorialId == TutorialId::HIT_ATTENUATION) // ��������
				{
					const FightData actData = fight->getFightData(true);
					if (actData.hitRate < 100) // ���������������Ă���ꍇ
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
			return false; // �`���[�g���A�����Z�b�g����Ă��Ȃ�ID�̏ꍇ�͖�����
		}

		return true;
	}
}