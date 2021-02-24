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
 * @brief �`���[�g���A�����Ǘ�����N���X
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

		//! �`���[�g���A��ID
		enum TutorialId : int
		{
			UNIT_SELECT,	//! ���j�b�g�I��
			TURN_END,		//! �^�[���G���h
			MOVE,			//! �ړ�
			MOVE_CONFIRM,	//! �ړ��m��
			FIGHT_PREDICT,	//! �퓬�\���m�F
			ATACK,			//! �U��
			TERRAIN_EFFECT,	//! �n�`����
			FREE_SET_SELECT,//! ���R�z�u�i�I���j
			FREE_SET_SET,	//! ���R�z�u�i�ݒu�j
			FREE_SET_DEL,	//! ���R�z�u�i�폜�j
			FREE_SET_FIN,	//! ���R�z�u�i�����j
			COORDINATED,	//! �A�g
			PINCHING,		//! ����
			ALL_KILL,		//! �r��
			DEFFENCE,		//! �h�q
			SUPPRESSION,	//! ����
			AMBUSH,			//! �}��
			AMBUSH_CANCEL,	//! �A�g�A�����ɂ��}���L�����Z��
			RUSH,			//! �ˌ�
			RUSH_CANCEL,	//! �}���ɂ��ˌ��L�����Z��
			RUSH_NOT_PLAIN,	//! �n�`�ɂ��ˌ��L�����Z��
			THROUGH,		//! �ђ�
			SHOOTING,		//! �ˌ��̐���
			LOADING,		//! �e����
			HIT_ATTENUATION,//! ����������
			TUTORIAL_ID_LEN
		};

		enum class FightPhase
		{
			PREDICT,//! �퓬�\��
			START,	//! �퓬�J�n
			END		//! �퓬�I��
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

		constexpr static int MESSAGE_MS = 20000; //! ���b�Z�[�W�\������

		constexpr static int MOVE_X = 8; //! �ړ��`���[�g���A�����̑Ώۃ}�X���W
		constexpr static int MOVE_Y = 5; //! �ړ��`���[�g���A�����̑Ώۃ}�X���W

		constexpr static int FOREST_X = 8; //! �n�`�`���[�g���A�����̑Ώۃ}�X���W
		constexpr static int FOREST_Y = 5; //! �n�`�`���[�g���A�����̑Ώۃ}�X���W

		constexpr static int SUPPRESSION_X = 8; //! �������WX
		constexpr static int SUPPRESSION_Y = 1; //! �������WY

		constexpr static int DEFFENCE_X = 8; //! �h�q���WX
		constexpr static int DEFFENCE_Y = 8; //! �h�q���WY

		static const vector<string> MESSAGES;
		static vector<string> initMessages();


		shared_ptr<Message> message_; //! ���b�Z�[�W�I�u�W�F�N�g

		vector<shared_ptr<TutorialArrow>> arrowList_; //! �\�����̃`���[�g���A�����

		bool isAble_; //! �L��/����

		vector<int> tutorialIdList_; //! �`���[�g���A����ID���X�g

		int prevTutorialId_; //! �O��̃`���[�g���A��ID
	};


}