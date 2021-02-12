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

		messageList.at(TutorialId::UNIT_SELECT) =		"���j�b�g���N���b�N����\n���j�b�g��I�����Ă�������";
		messageList.at(TutorialId::STATUS_CHECK) =		"���j�b�g��I�������\n��ʂ����Ƀ��j�b�g�̃X�e�[�^�X���\������܂�";
		messageList.at(TutorialId::MOVE) =				"���Ȃ��Ă���}�X��I�������\n���j�b�g���ړ����܂�";
		messageList.at(TutorialId::MOVE_CANCEL) =		"�I�𒆂̃��j�b�g�ƐԂ��Ȃ��Ă���}�X�ȊO��\n�N���b�N����ƈړ����L�����Z���ł��܂�";
		messageList.at(TutorialId::FIGHT_PREDICT) =		"�Ԃ��Ȃ��Ă���}�X�̏�Ƀ}�E�X�J�[�\����u����\n��ʉ��ɐ퓬�\�����\������܂�";
		messageList.at(TutorialId::ATACK) =				"�Ԃ��Ȃ��Ă���܂����N���b�N����ƓG���j�b�g�ɍU���\�ł�";
		messageList.at(TutorialId::TERRAIN_EFFECT) =	"�X�ɂȂ��Ă���}�X�Ƀ��j�b�g��u����\n�����h�䂪�㏸���܂�\n�i�n�`���ʂ́A�}�X�̏�ɃJ�[�\����u���ƍ����ɕ\������܂��B�j";
		messageList.at(TutorialId::FREE_SET_SELECT) =	"��������ݒu���������j�b�g��I�����Ă�������";
		messageList.at(TutorialId::FREE_SET_SET) =		"���}�X���N���b�N���邱�Ƃ�\n�I�𒆂̃��j�b�g��ݒu�ł��܂�";
		messageList.at(TutorialId::FREE_SET_DEL) =		"�ݒu�������j�b�g���N���b�N���邱�Ƃ�\n�ݒu�����������Ƃ��ł��܂�";
		messageList.at(TutorialId::FREE_SET_FIN) =		"���j�b�g�̐ݒu�����ׂĊ���������A�E���́u���������v�{�^�����N���b�N���Ă�������";
		messageList.at(TutorialId::COORDINATED) =		"�U����A���̃��j�b�g�ő��ʂ���U�����邱�Ƃ�\n�u�A�g�v�U���ɂȂ�A�_���[�W���㏸���܂��B";
		messageList.at(TutorialId::PINCHING) =			"�U����A���̃��j�b�g�Ŕ��Α�����U�����邱�Ƃ�\n�u�����v�ɂȂ�A�_���[�W���啝�ɏ㏸���܂��B";
		messageList.at(TutorialId::ALL_KILL) =			"�G���j�b�g��S�ł����邱�Ƃŏ����ɂȂ�܂�";
		messageList.at(TutorialId::DEFFENCE) =			"�G���j�b�g�����R�̍ԂɈړ�����Ɣs�k�ɂȂ�܂�";
		messageList.at(TutorialId::SUPPRESSION) =		"�G�R�̍ԂɈړ����邱�Ƃŏ����ɂȂ�܂�";
		messageList.at(TutorialId::AMBUSH) =			"�������R���i���������˒��̒Z�����j�b�g�j�ɍU�����ꂽ���A�X�L���u�}���v�𔭓����܂��B\n�u�}���v�𔭓�����ƁA�搧�ōU�����܂��B";
		messageList.at(TutorialId::AMBUSH_CANCE) =		"�����́u�A�g�v�܂��́u�����v���󂯂��ꍇ��\n�X�L���u�}���v�𔭓��ł��܂���";
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
	 * ����C�x���g�������ɁA
	 */
	void TutorialManager::onEvent(int tutorialId)
	{
		if (!isAble_)
		{
			return; // �������
		}

		for (auto itr = tutorialIdList_.begin(); itr != tutorialIdList_.end(); ++itr)
		{
			if (*itr == tutorialId)
			{
				// �`���[�g���A�����b�Z�[�W�\��
				message_->show(MESSAGES[tutorialId], true);
				tutorialIdList_.erase(itr);
				break;
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
			return false; // �`���[�g���A�����Z�b�g����Ă��Ȃ�ID�̏ꍇ�͖�����
		}

		return true;
	}
}