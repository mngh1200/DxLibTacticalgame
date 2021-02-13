#pragma once
#include <climits>
#include <map>
#include <string>
#include "Entity/UI/Message.h"
#include "Entity/Battle/Map.h"

using namespace std;
using namespace Entity;

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
			AMBUSH_CANCE,	//! �A�g�A�����ɂ��}���L�����Z��
			RUSH,			//! �ˌ�
			RUSH_CANCEL,	//! �}���ɂ��ˌ��L�����Z��
			RUSH_NOT_PLAIN,	//! �n�`�ɂ��ˌ��L�����Z��
			THROUGH,		//! �ђ�
			LOADING,		//! �e����
			HIT_ATTENUATION,//! ����������
			TUTORIAL_ID_LEN
		};

		void init(int stageId, shared_ptr<Message> message);
		bool onEvent(int tutorialId);

		void onPlayerTurnStart(shared_ptr<Map> map);

	private:
		bool setTutorialIdList(int stageId);

		static const vector<string> MESSAGES;
		static vector<string> initMessages();


		shared_ptr<Message> message_; //! ���b�Z�[�W�I�u�W�F�N�g

		bool isAble_; //! �L��/����

		vector<int> tutorialIdList_; //! �`���[�g���A����ID���X�g
	};


}