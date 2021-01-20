#include "ResourceManager.h"
#define THROWS_EX(ret, msg) { if (ret == -1) { throw msg; } }

namespace Utility {
	ResourceManager& ResourceManager::getInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	ResourceManager::~ResourceManager()
	{
		unloadFont("resource/font/rounded-mplus/rounded-mplus-1p-regular.ttf");
		unloadFont("resource/font/rounded-mplus/rounded-mplus-1p-black.ttf");

		for (auto&& mItr = image_.begin(); mItr != image_.end(); ++mItr)
		{
			vector<int*> vec = mItr->second;
			for (auto itr = vec.begin(); itr != vec.end(); ++itr)
			{
				delete* itr;
			}
		}
	}

	/**
	 * @fn
	 * リソースのロード
	 * @return 終了フラグ（0が正常）
	 */
	int ResourceManager::load()
	{
		int ret = 0;
		loadImages();
		loadFonts();
		loadColors();
		loadSounds();

		mapList_[MapId::STAGE1] = loadMapCsv("resource/map/stage1.csv");

		return ret;
	}
	/**
	 * @fn
	 * 指定されたオブジェクト取得
	 * @param (type) リソースタイプ
	 * @param (kind) リソースの種類
	 * @param (pos) リソースの位置
	 * @return 対象リソースのID
	 */
	int ResourceManager::getImage(int type, int kind, int pos) const
	{
		return image_.at(type).at(kind)[pos];
	}


	int ResourceManager::getHdlFont(int kind) const
	{
		return hdlFont_[kind];
	}
	int ResourceManager::getColor(int kind) const
	{
		return colorType_[kind];
	}
	int ResourceManager::getSound(int kind) const
	{
		return sounds_[kind];
	}

	int ResourceManager::getMapMass(int mapId, int w, int h) const
	{
		return mapList_[mapId][h][w];
	}

	void ResourceManager::playSound(int kind) {
		DxLib::PlaySoundMem(getInstance().sounds_[kind], DX_PLAYTYPE_BACK);
	}

	/**
	 * @fn
	 * マップチップ用の読み取り関数
	 * @param (resourcePath) リソースパス
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadMapchip(const char* resourcePath, int* handle) {
		int ret = DxLib::LoadDivGraph(resourcePath,
			8, 4, 2, CHIP_SIZE, CHIP_SIZE, handle);
		THROWS_EX(ret, resourcePath);
	}
	/**
	 * @fn
	 * キャラチップ用の読み取り関数
	 * @param (resourcePath) リソースパス
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadCharacterchip(const char* resourcePath, int* handle) {
		int ret = DxLib::LoadDivGraph(resourcePath, 16, 4, 4, CHIP_SIZE, CHIP_SIZE, handle);
		THROWS_EX(ret, resourcePath);
	}
	/**
	 * @fn
	 * 1枚絵の読み取り関数
	 * @param (resourcePath) リソースパス
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadImage(const char* resourcePath, int* handle) {
		*handle = DxLib::LoadGraph(resourcePath);
		THROWS_EX(*handle, resourcePath);
	}

	/**
	 * @fn
	 * マップチップ用の読み取り関数（リソースから）
	 * @param (resourceName) リソース名
	 * @param (resourceType) リソースタイプ
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadMapchip(const char* resourceName, const char* resourceType, int* handle) {
		int ret = DxLib::LoadDivGraphToResource(resourceName, resourceType,
			8, 4, 2, CHIP_SIZE, CHIP_SIZE, handle);
		THROWS_EX(ret, resourceName);
	}
	/**
	 * @fn
	 * キャラチップ用の読み取り関数（リソースから）
	 * @param (resourceName) リソース名
	 * @param (resourceType) リソースタイプ
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadCharacterchip(const char* resourceName, const char* resourceType, int* handle) {
		int ret = DxLib::LoadDivGraphToResource(resourceName, resourceType, 6, 3, 2, CHIP_SIZE, CHIP_SIZE, handle);
		THROWS_EX(ret, resourceName);
	}
	/**
	 * @fn
	 * 1枚絵の読み取り関数（リソースから）
	 * @param (resourceName) リソース名
	 * @param (resourceType) リソースタイプ
	 * @param (handle) ID格納変数
	 */
	void ResourceManager::loadImage(const char* resourceName, const char* resourceType, int* handle) {
		*handle = DxLib::LoadGraphToResource(resourceName, resourceType);
		THROWS_EX(*handle, resourceName);
	}

	int ResourceManager::loadImages()
	{
		int ret = 0;
		image_.insert(std::make_pair(ImageType::IMAGE, vector<int*>()));

		for (int i = 0; i < ImageId::IMAGE_ID_LEN; i++)
		{
			image_.at(ImageType::IMAGE).push_back(new int[1]);
		}
		loadImage(MAKEINTRESOURCE(BACKGROUND_IMAGE), MAKEINTRESOURCE(IMAGE_FILE), image_.at(ImageType::IMAGE).at(ImageId::BACKGROUND_MENU));

		loadImage("resource/image/map/state-atack.png", image_.at(ImageType::IMAGE).at(ImageId::MASS_ATACK));
		loadImage("resource/image/map/state-movable.png", image_.at(ImageType::IMAGE).at(ImageId::MASS_MOVE));

		image_.insert(std::make_pair(ImageType::MAP, vector<int*>()));
		image_.at(ImageType::MAP).push_back(new int[8]);
		//loadMapchip("resource/image/map/mapchip.png", image_.at(ImageType::MAP).at(0));
		loadMapchip(MAKEINTRESOURCE(MAP_IMAGE), MAKEINTRESOURCE(IMAGE_FILE), image_.at(ImageType::MAP).at(0));

		// プレイヤー画像の読み込み
		image_.insert(std::make_pair(ImageType::UNIT, vector<int*>()));
		image_.at(ImageType::UNIT).push_back(new int[6]);
		loadCharacterchip(MAKEINTRESOURCE(UNIT_IMAGE), MAKEINTRESOURCE(IMAGE_FILE), image_.at(ImageType::UNIT).at(UnitImageKind::NORMAL));
		image_.insert(std::make_pair(ImageType::UNIT, vector<int*>()));
		image_.at(ImageType::UNIT).push_back(new int[6]);
		loadCharacterchip(MAKEINTRESOURCE(UNIT_ACTED_IMAGE), MAKEINTRESOURCE(IMAGE_FILE), image_.at(ImageType::UNIT).at(UnitImageKind::ACTED));

		// ダメージエフェクト
		image_.insert(std::make_pair(ImageType::EFFECT, vector<int*>()));
		image_.at(ImageType::EFFECT).push_back(new int[10]);
		ret = DxLib::LoadDivGraph("resource/image/effect/damage.png", 10, 10, 1, 26, 32, image_.at(ImageType::EFFECT).at(EffectId::DAMAGE));
		
		image_.at(ImageType::EFFECT).push_back(new int[1]);
		loadImage("resource/image/effect/miss.png", image_.at(ImageType::EFFECT).at(EffectId::MISS));

		loadFlag = true;
		return ret;
	}

	int ResourceManager::loadFonts()
	{
		int ret = 0;

		loadFont("resource/font/rounded-mplus/rounded-mplus-1p-regular.ttf");
		loadFont("resource/font/rounded-mplus/rounded-mplus-1p-black.ttf");

		hdlFont_[FontType::NORMAL_S18] = DxLib::CreateFontToHandle("Rounded M+ 1p regular", 18, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::NORMAL_S24] = DxLib::CreateFontToHandle("Rounded M+ 1p regular", 24, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::NORMAL_S32] = DxLib::CreateFontToHandle("Rounded M+ 1p regular", 32, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::BLACK_S48] = DxLib::CreateFontToHandle("Rounded M+ 1p black", 48, 3, DX_FONTTYPE_ANTIALIASING_4X4);

		// フォントの行間
		DxLib::SetFontLineSpaceToHandle(24, hdlFont_[FontType::NORMAL_S18]);

		return ret;
	}

	int ResourceManager::loadColors()
	{
		int ret = 0;
		// 色取得
		colorType_[ColorType::WHITE] = DxLib::GetColor(255, 255, 255);

		colorType_[ColorType::MAIN_COLOR] = DxLib::GetColor(250, 244, 232);
		colorType_[ColorType::MAIN_COLOR_ON] = DxLib::GetColor(254, 213, 175);
		colorType_[ColorType::MAIN_COLOR_ON_DARK] = 0xFB8B24;

		colorType_[ColorType::SUB_COLOR] = DxLib::GetColor(124, 104, 95);
		colorType_[ColorType::SUB_COLOR_LITE] = DxLib::GetColor(224, 215, 204);
		colorType_[ColorType::SUB_COLOR_DARK] = DxLib::GetColor(76, 64, 59);

		colorType_[ColorType::TITLE_TEXT] = DxLib::GetColor(211, 181, 155);

		colorType_[ColorType::PLAYER_COLOR] = DxLib::GetColor(35, 87, 137);
		colorType_[ColorType::PLAYER_COLOR_LITE] = DxLib::GetColor(186, 197, 204);

		colorType_[ColorType::ENEMY_COLOR] = DxLib::GetColor(209, 6, 6);
		colorType_[ColorType::ENEMY_COLOR_LITE] = DxLib::GetColor(233, 183, 177);

		colorType_[ColorType::POSITIVE_COLOR] = DxLib::GetColor(116, 142, 84);
		colorType_[ColorType::POSITIVE_LITE_COLOR] = DxLib::GetColor(185, 202, 165);

		colorType_[ColorType::NEGATIVE_COLOR] = DxLib::GetColor(139, 137, 130);

		colorType_[ColorType::ACCENT_COLOR] = DxLib::GetColor(204, 75, 194);

		return ret;
	}

	int ResourceManager::loadSounds()
	{
		int ret = 0;
		// 効果音取得
		//sounds_[SoundKind::CLICK] = DxLib::LoadSoundMem("resource/sound/click.ogg");
		sounds_[SoundKind::CLICK] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(CLICK_OGG), MAKEINTRESOURCE(OGG));
		DxLib::ChangeVolumeSoundMem(210, sounds_[SoundKind::CLICK]);

		//sounds_[SoundKind::CHECK] = DxLib::LoadSoundMem("resource/sound/check.ogg");
		sounds_[SoundKind::CHECK] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(CHECK_OGG), MAKEINTRESOURCE(OGG));
		DxLib::ChangeVolumeSoundMem(220, sounds_[SoundKind::CHECK]);

		// sounds_[SoundKind::BACK] = DxLib::LoadSoundMem("resource/sound/back.wav");
		sounds_[SoundKind::BACK] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(BACK_WAVE), MAKEINTRESOURCE(WAVE));
		DxLib::ChangeVolumeSoundMem(180, sounds_[SoundKind::BACK]);

		//sounds_[SoundKind::BORN] = DxLib::LoadSoundMem("resource/sound/born.wav");
		sounds_[SoundKind::BORN] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE(BORN_WAVE), MAKEINTRESOURCE(WAVE));
		DxLib::ChangeVolumeSoundMem(190, sounds_[SoundKind::BORN]);

		return ret;
	}

	void ResourceManager::loadFont(const LPCSTR fontFilePath)
	{
		if (AddFontResource(fontFilePath) > 0) {
			PostMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
		}
		else {
			// フォント読込エラー処理
			MessageBox(NULL, "フォント読込失敗", "", MB_OK);
		}
	}

	void ResourceManager::unloadFont(const LPCSTR fontFilePath)
	{
		if (RemoveFontResource(fontFilePath)) {
			PostMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
		}
		else {
			MessageBox(NULL, "remove failure", "", MB_OK);
		}
	}

	std::array < std::array <int, MAP_MASS_W>, MAP_MASS_H > ResourceManager::loadMapCsv(const LPCSTR csvFilePath)
	{
		std::array < std::array <int, MAP_MASS_W>, MAP_MASS_H > ret = {};
		std::string str_buf;
		std::string str_conma_buf;
		for (int i = 0; i < MAP_MASS_H; i++) {
			for (int j = 0; j < MAP_MASS_W; j++) {
				ret[i][j] = -1;
			}

		}


		// 読み込むcsvファイルを開く(std::ifstreamのコンストラクタで開く)
		std::ifstream ifs_csv_file(csvFilePath);
		int lineCount = 0;
		// getline関数で1行ずつ読み込む(読み込んだ内容はstr_bufに格納)
		while (getline(ifs_csv_file, str_buf) && lineCount < MAP_MASS_H) {
			// 「,」区切りごとにデータを読み込むためにistringstream型にする
			std::istringstream i_stream(str_buf);
			int colCount = 0;
			// 「,」区切りごとにデータを読み込む
			while (getline(i_stream, str_conma_buf, ',') && colCount < MAP_MASS_W) {
				// csvファイルに書き込む
				ret[lineCount][colCount] = stoi(str_conma_buf);
				colCount++;
			}
			lineCount++;
		}


		return ret;
	}

	boolean ResourceManager::isLoaded() const {
		return loadFlag;
	}

}