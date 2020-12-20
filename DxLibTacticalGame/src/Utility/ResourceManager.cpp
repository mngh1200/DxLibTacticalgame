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
	}

	/**
	 * @fn
     * ���\�[�X�̃��[�h
     * @return �I���t���O�i0������j
     */
	int ResourceManager::load()
	{
		int ret = 0;
		loadImages();
		loadFonts();
		loadColors();
		loadSounds();
		
		return ret;
	}

	/**
	 * @fn
	 * �w�肳�ꂽ�I�u�W�F�N�g�擾(1���G�p)
	 * @param (type) ���\�[�X�^�C�v
	 * @param (kind) ���\�[�X�̎��
	 * @return �Ώۃ��\�[�X��ID
	 */
	int ResourceManager::getImage(int type, int kind) const
	{
		return image_.at(type).at(kind)[0];
	}
	/**
     * @fn
     * �w�肳�ꂽ�I�u�W�F�N�g�擾
     * @param (type) ���\�[�X�^�C�v
     * @param (kind) ���\�[�X�̎��
	 * @param (pos) ���\�[�X�̈ʒu
     * @return �Ώۃ��\�[�X��ID
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

	void ResourceManager::playSound(int kind) {
		DxLib::PlaySoundMem(getInstance().sounds_[kind], DX_PLAYTYPE_BACK);
	}

	/**
	 * @fn
	 * �}�b�v�`�b�v�p�̓ǂݎ��֐�
	 * @param (resourcePath) ���\�[�X�p�X
	 * @param (handle) ID�i�[�ϐ�
	 */
	void ResourceManager::loadMapchip(const char* resourcePath, int* handle) {
		int ret = DxLib::LoadDivGraph(resourcePath,
			8, 4, 2, CHIP_SIZE, CHIP_SIZE, handle);
		THROWS_EX(ret, resourcePath);
	}
	/**
	 * @fn
	 * �L�����`�b�v�p�̓ǂݎ��֐�
	 * @param (resourcePath) ���\�[�X�p�X
	 * @param (handle) ID�i�[�ϐ�
	 */
	void ResourceManager::loadCharacterchip(const char* resourcePath, int* handle) {
		int ret = DxLib::LoadDivGraph(resourcePath, 16, 4, 4, CHIP_SIZE, CHIP_SIZE, handle);
		THROWS_EX(ret, resourcePath);
	}
	/**
	 * @fn
	 * 1���G�̓ǂݎ��֐�
	 * @param (resourcePath) ���\�[�X�p�X
	 * @param (handle) ID�i�[�ϐ�
	 */
	void ResourceManager::loadImage(const char* resourcePath, int* handle) {
		*handle = DxLib::LoadGraph(resourcePath);
		THROWS_EX(*handle, resourcePath);
	}

	int ResourceManager::loadImages()
	{
		int ret = 0;
		image_.insert(std::make_pair(ImageType::IMAGE, vector<int*>()));
		image_.at(ImageType::IMAGE).push_back(new int[IMAGE_ID_LEN]);
		loadImage("resource/image/menu/background.jpg", image_.at(ImageType::IMAGE).at(0));


		image_.insert(std::make_pair(ImageType::MAP, vector<int*>()));
		image_.at(ImageType::MAP).push_back(new int[8]);
		loadMapchip("resource/image/map/mapchip.png", image_.at(ImageType::MAP).at(0));
		// �v���C���[�摜�̓ǂݍ���
		image_.insert(std::make_pair(ImageType::PLAYER, vector<int*>()));
		image_.at(ImageType::PLAYER).push_back(new int[16]);
		loadCharacterchip("resource/image/unit/player/lancer.png", image_.at(ImageType::PLAYER).at(UnitKey::LANCER));
		image_.insert(std::make_pair(ImageType::PLAYER, vector<int*>()));
		image_.at(ImageType::PLAYER).push_back(new int[16]);
		loadCharacterchip("resource/image/unit/player/cavalry.png", image_.at(ImageType::PLAYER).at(UnitKey::CAVALRY));
		image_.insert(std::make_pair(ImageType::PLAYER, vector<int*>()));
		image_.at(ImageType::PLAYER).push_back(new int[16]);
		loadCharacterchip("resource/image/unit/player/gunner.png", image_.at(ImageType::PLAYER).at(UnitKey::GUNNER));
		// �G�l�~�[�摜�̓ǂݍ���
		image_.insert(std::make_pair(ImageType::ENEMY, vector<int*>()));
		image_.at(ImageType::ENEMY).push_back(new int[16]);
		loadCharacterchip("resource/image/unit/enemy/lancer_enemy.png", image_.at(ImageType::ENEMY).at(UnitKey::LANCER));
		image_.insert(std::make_pair(ImageType::ENEMY, vector<int*>()));
		image_.at(ImageType::ENEMY).push_back(new int[16]);
		loadCharacterchip("resource/image/unit/enemy/cavalry_enemy.png", image_.at(ImageType::ENEMY).at(UnitKey::CAVALRY));
		image_.insert(std::make_pair(ImageType::ENEMY, vector<int*>()));
		image_.at(ImageType::ENEMY).push_back(new int[16]);
		loadCharacterchip("resource/image/unit/enemy/gunner_enemy.png", image_.at(ImageType::ENEMY).at(UnitKey::GUNNER));

		return ret;
	}

	int ResourceManager::loadFonts()
	{
		int ret = 0;

		loadFont("resource/font/rounded-mplus/rounded-mplus-1p-regular.ttf");
		loadFont("resource/font/rounded-mplus/rounded-mplus-1p-black.ttf");

		hdlFont_[FontType::NORMAL_S24] = DxLib::CreateFontToHandle("Rounded M+ 1p regular", 24, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::NORMAL_S32] = DxLib::CreateFontToHandle("Rounded M+ 1p regular", 32, 3, DX_FONTTYPE_ANTIALIASING_4X4);
		hdlFont_[FontType::BLACK_S48] = DxLib::CreateFontToHandle("Rounded M+ 1p black", 48, 3, DX_FONTTYPE_ANTIALIASING_4X4);

		return ret;
	}

	int ResourceManager::loadColors()
	{
		int ret = 0;
		// �F�擾
		colorType_[ColorType::MAIN_COLOR] = DxLib::GetColor(250, 244, 232);
		colorType_[ColorType::MAIN_COLOR_ON] = DxLib::GetColor(254, 213, 175);

		colorType_[ColorType::SUB_COLOR] = DxLib::GetColor(124, 104, 95);

		colorType_[ColorType::TITLE_TEXT] = DxLib::GetColor(211, 181, 155);

		colorType_[ColorType::PLAYER_COLOR] = DxLib::GetColor(35, 87, 137);
		colorType_[ColorType::ENEMY_COLOR] = DxLib::GetColor(209, 6, 6);

		colorType_[ColorType::POSITIVE_COLOR] = DxLib::GetColor(116, 142, 84);
		colorType_[ColorType::POSITIVE_LITE_COLOR] = DxLib::GetColor(185, 202, 165);

		colorType_[ColorType::NEGATIVE_COLOR] = DxLib::GetColor(139, 137, 130);

		colorType_[ColorType::ACCENT_COLOR] = DxLib::GetColor(204, 75, 194);

		return ret;
	}

	int ResourceManager::loadSounds()
	{
		int ret = 0;
		// ���ʉ��擾
		sounds_[SoundKind::CLICK] = DxLib::LoadSoundMem("resource/sound/click.ogg");
		DxLib::ChangeVolumeSoundMem(210, sounds_[SoundKind::CLICK]);

		sounds_[SoundKind::CHECK] = DxLib::LoadSoundMem("resource/sound/check.ogg");
		DxLib::ChangeVolumeSoundMem(220, sounds_[SoundKind::CHECK]);

		// sounds_[SoundKind::BACK] = DxLib::LoadSoundMem("resource/sound/back.wav");
		sounds_[SoundKind::BACK] = DxLib::LoadSoundMemByResource(MAKEINTRESOURCE ("BACK_WAVE"), MAKEINTRESOURCE ("WAVE"));
		DxLib::ChangeVolumeSoundMem(180, sounds_[SoundKind::BACK]);

		sounds_[SoundKind::BORN] = DxLib::LoadSoundMem("resource/sound/born.wav");
		DxLib::ChangeVolumeSoundMem(190, sounds_[SoundKind::BORN]);

		return ret;
	}

	void ResourceManager::loadFont(const LPCSTR fontFilePath)
	{
		if (AddFontResource(fontFilePath) > 0) {
			PostMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
		}
		else {
			// �t�H���g�Ǎ��G���[����
			MessageBox(NULL, "�t�H���g�Ǎ����s", "", MB_OK);
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

}