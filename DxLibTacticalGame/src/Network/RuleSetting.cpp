#include "RuleSetting.h"
#include "Screen/NetworkScreen.h"

namespace Network
{
	const int RuleSetting::LAYER_CONTENT = Screen::NetworkScreen::Layer::MODAL_CONTENT;
	const int RuleSetting::LAYER_FRAME = Screen::NetworkScreen::Layer::MODAL_FRAME;


	/**
	 * @fn
	 * 開始
	 * @param (netHandle) ネットワークハンドル
	*/
	void RuleSetting::start(int netHandle)
	{
		netHandle_ = netHandle;

		// 小画面生成
		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		// フレーム
		shared_ptr<Entity::ModalFrame> frame = make_shared<ModalFrame>();
		frame->setColor(ColorType::MAIN_COLOR);
		frame->setShape(MODAL_X, MODAL_Y, MODAL_W, MODAL_H);
		frame->setTitle("ルール設定");
		objectsControl.addObject(LAYER_FRAME, frame);

		
		// ユニット数
		objectsControl.addFigure(LAYER_CONTENT, make_shared<Text>("ユニット数を選択してください", CONTENT_X, CONTENT_Y, FontType::NORMAL_S24, ColorType::SUB_COLOR_BIT_LITE));

		unitNum_ = make_shared<RadioButton>();
		vector<string> textList = {"6", "9", "12"};
		unitNum_->setItems(textList);
		unitNum_->setPos(CONTENT_X, CONTENT_Y + LINE_H + LINE_MARGIN);
		unitNum_->setItemSize(RADIO_W, RADIO_H);
		objectsControl.addObject(LAYER_CONTENT, unitNum_);


		// マップ選択
		objectsControl.addFigure(LAYER_CONTENT, make_shared<Text>("マップを選択してください", CONTENT_X, SELECT_MAP_Y, FontType::NORMAL_S24, ColorType::SUB_COLOR_BIT_LITE));

		mapSelect_ = make_shared<RadioButton>();
		textList = { "平原", "河畔" };
		mapSelect_->setItems(textList);
		mapSelect_->setPos(CONTENT_X, SELECT_MAP_Y + LINE_H + LINE_MARGIN);
		mapSelect_->setItemSize(RADIO_W, RADIO_H);
		objectsControl.addObject(LAYER_CONTENT, mapSelect_);

		// 状況メッセージ
		statusText_ = make_shared<Text>("接続状況 : クライアントと接続済み", CONTENT_X, BUTTON_Y - (LINE_H + LINE_MARGIN), FontType::NORMAL_S24, ColorType::SUB_COLOR);
		objectsControl.addFigure(LAYER_CONTENT, statusText_);

		// 開始ボタン
		startButton_ = make_shared<TextButton>();
		startButton_->setShape(CONTENT_X, BUTTON_Y, BUTTON_W, BUTTON_H);
		startButton_->setText("開始", FontType::BLACK_S32);
		startButton_->setColor(ColorType::POSITIVE_LITE_COLOR, ColorType::POSITIVE_COLOR, Entity::TextButton::State::NORMAL);
		startButton_->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_DOWN);
		startButton_->setColor(ColorType::POSITIVE_COLOR, ColorType::POSITIVE_LITE_COLOR, Entity::TextButton::State::MOUSE_OVER);
		objectsControl.addObject(LAYER_CONTENT, startButton_);

		// 切断ボタン
		closeButton_ = make_shared<TextButton>();
		closeButton_->setShape(CONTENT_X + BUTTON_W + BUTTON_MARGIN_X, BUTTON_Y, BUTTON_W, BUTTON_H);
		closeButton_->setText("切断", FontType::BLACK_S32);
		closeButton_->setColor(ColorType::SUB_COLOR_LITE, ColorType::SUB_COLOR, TextButton::State::NORMAL);
		closeButton_->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, TextButton::State::MOUSE_DOWN);
		closeButton_->setColor(ColorType::SUB_COLOR, ColorType::MAIN_COLOR, TextButton::State::MOUSE_OVER);
		objectsControl.addObject(LAYER_CONTENT, closeButton_);
	}

	/**
	 * @fn
	 * 終了
	*/
	void RuleSetting::end()
	{
		// 切断
		DxLib::CloseNetWork(netHandle_);

		FrameWork::Game& game = FrameWork::Game::getInstance();
		Entity::ObjectsControl& objectsControl = game.objectsControl;

		// 全要素削除
		objectsControl.removeObject(LAYER_FRAME);
		objectsControl.removeObject(LAYER_CONTENT);
		objectsControl.removeFigure(LAYER_CONTENT);
	}

	/**
	 * @fn
	 * 状況取得及び更新処理
	 * @param (hitObjWp) 接触オブジェクトの弱参照
	 * @param (x) マウスのx座標
	 * @param (y) マウスのy座標
	 * @param (button) ボタンの種類
	 * @param (eventType) イベントの種類（マウスダウン or マウスアップ or マウスクリック）
	 * @return 状況を返す
	*/
	int RuleSetting::checkAndUpdate(weak_ptr<Entity::Object> hitObjWp, int x, int y, int button, int eventType)
	{

		// 切断された場合
		if (int lostHandle = GetLostNetWork() == netHandle_)
		{
			statusText_->setText("接続状況 : クライアントが切断");
			DxLib::CloseNetWork(netHandle_);
			isConnect_ = false;
			startButton_->setColor(ColorType::NEGATIVE_COLOR_LITE, ColorType::NEGATIVE_COLOR, TextButton::State::ALL);
			closeButton_->setText("閉じる");
		}

		shared_ptr<Entity::Object> hitObjSp = hitObjWp.lock();

		if (hitObjSp)
		{
			// ボタンのクリックイベント
			if (hitObjSp->getType() == Entity::Figure::BUTTON && eventType == MOUSE_INPUT_LOG_CLICK)
			{
				int objId = hitObjSp->getObjectId();

				if (hitObjSp == closeButton_) // キャンセル or 切断
				{
					end();
					Utility::ResourceManager::playSound(SoundKind::BACK);
					return Result::CANCEL;
				}
				else if (isConnect_ && hitObjSp == startButton_) // 開始
				{
					Utility::ResourceManager::playSound(SoundKind::CLICK);
					DxLib::printfDx("開始");
					return Result::START_BATTLE;
				}
			}
		}

		return Result::CONTINUE;
	}

}