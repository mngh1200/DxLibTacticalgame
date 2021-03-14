#include "RuleSetting.h"
#include "Screen/NetworkScreen.h"

namespace Network
{
	const int RuleSetting::LAYER_CONTENT = Screen::NetworkScreen::Layer::MODAL_CONTENT;
	const int RuleSetting::LAYER_FRAME = Screen::NetworkScreen::Layer::MODAL_FRAME;


	/**
	 * @fn
	 * �J�n
	 * @param (netHandle) �l�b�g���[�N�n���h��
	*/
	void RuleSetting::start(int netHandle)
	{
		netHandle_ = netHandle;

		// ����ʐ���
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		// �t���[��
		shared_ptr<Entity::ModalFrame> frame = make_shared<ModalFrame>();
		frame->setColor(ColorType::MAIN_COLOR);
		frame->setShape(MODAL_X, MODAL_Y, MODAL_W, MODAL_H);
		frame->setTitle("���[���ݒ�");
		objectsControl.addObject(LAYER_FRAME, frame);

		
		// ���j�b�g��
		objectsControl.addFigure(LAYER_CONTENT, make_shared<Text>("���j�b�g����I�����Ă�������", CONTENT_X, CONTENT_Y, FontType::NORMAL_S24, ColorType::SUB_COLOR_BIT_LITE));

		unitNum_ = make_shared<RadioButton>();
		vector<string> textList = {"6", "9", "12"};
		unitNum_->setItems(textList);
		unitNum_->setPos(CONTENT_X, CONTENT_Y + LINE_H + LINE_MARGIN);
		unitNum_->setItemSize(RADIO_W, RADIO_H);
		objectsControl.addObject(LAYER_CONTENT, unitNum_);


		// �}�b�v�I��
		objectsControl.addFigure(LAYER_CONTENT, make_shared<Text>("�}�b�v��I�����Ă�������", CONTENT_X, SELECT_MAP_Y, FontType::NORMAL_S24, ColorType::SUB_COLOR_BIT_LITE));

		mapSelect_ = make_shared<RadioButton>();
		textList = { "����", "�͔�" };
		mapSelect_->setItems(textList);
		mapSelect_->setPos(CONTENT_X, SELECT_MAP_Y + LINE_H + LINE_MARGIN);
		mapSelect_->setItemSize(RADIO_W, RADIO_H);
		objectsControl.addObject(LAYER_CONTENT, mapSelect_);

		// �󋵃��b�Z�[�W
		statusText_ = make_shared<Text>("�ڑ��� : �N���C�A���g�Ɛڑ��ς�", CONTENT_X, BUTTON_Y - (LINE_H + LINE_MARGIN), FontType::NORMAL_S24, ColorType::SUB_COLOR);
		objectsControl.addFigure(LAYER_CONTENT, statusText_);

		// �J�n�{�^��
		startButton_ = make_shared<TextButton>();
		startButton_->setShape(CONTENT_X, BUTTON_Y, BUTTON_W, BUTTON_H);
		startButton_->setText("�J�n", FontType::BLACK_S32);
		startButton_->setColor(ColorType::POSITIVE_LITE_COLOR, ColorType::POSITIVE_COLOR, Entity::TextButton::State::NORMAL);
		startButton_->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		startButton_->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_OVER);
		objectsControl.addObject(LAYER_CONTENT, startButton_);

		// �ؒf�{�^��
		closeButton_ = make_shared<TextButton>();
		closeButton_->setShape(CONTENT_X + BUTTON_W + BUTTON_MARGIN_X, BUTTON_Y, BUTTON_W, BUTTON_H);
		closeButton_->setText("�ؒf", FontType::BLACK_S32);
		closeButton_->setColor(ColorType::SUB_COLOR_LITE, ColorType::SUB_COLOR, TextButton::State::NORMAL);
		closeButton_->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, TextButton::State::MOUSE_DOWN);
		closeButton_->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, TextButton::State::MOUSE_OVER);
		objectsControl.addObject(LAYER_CONTENT, closeButton_);
	}

	/**
	 * @fn
	 * �I��
	*/
	void RuleSetting::end()
	{
		// �ؒf
		DxLib::CloseNetWork(netHandle_);

		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		// �S�v�f�폜
		objectsControl.removeObject(LAYER_FRAME);
		objectsControl.removeObject(LAYER_CONTENT);
		objectsControl.removeFigure(LAYER_CONTENT);
	}

	/**
	 * @fn
	 * �󋵎擾�y�эX�V����
	 * @param (hitObjWp) �ڐG�I�u�W�F�N�g�̎�Q��
	 * @param (x) �}�E�X��x���W
	 * @param (y) �}�E�X��y���W
	 * @param (button) �{�^���̎��
	 * @param (eventType) �C�x���g�̎�ށi�}�E�X�_�E�� or �}�E�X�A�b�v or �}�E�X�N���b�N�j
	 * @return �󋵂�Ԃ�
	*/
	int RuleSetting::checkAndUpdate(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{

		// �ؒf���ꂽ�ꍇ
		if (int lostHandle = GetLostNetWork() == netHandle_)
		{
			statusText_->setText("�ڑ��� : �N���C�A���g���ؒf");
			DxLib::CloseNetWork(netHandle_);
			isConnect_ = false;
			startButton_->setColor(ColorType::NEGATIVE_COLOR_LITE, ColorType::NEGATIVE_COLOR, TextButton::State::ALL);
			closeButton_->setText("����");
		}

		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		if (hitObjSp)
		{
			// �{�^���̃N���b�N�C�x���g
			if (hitObjSp->getType() == Entity::Figure::BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
			{
				int objId = hitObjSp->getObjectId();

				if (hitObjSp == closeButton_) // �L�����Z�� or �ؒf
				{
					end();
					Utility::ResourceManager::playSound(SoundKind::BACK);
					return Result::CANCEL;
				}
				else if (isConnect_ && hitObjSp == startButton_) // �J�n
				{
					Utility::ResourceManager::playSound(SoundKind::CLICK);
					DxLib::printfDx("�J�n");
					return Result::START_BATTLE;
				}
			}
		}

		return Result::CONTINUE;
	}

}