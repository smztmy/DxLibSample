
//�w�b�_�t�@�C���ǂݍ���
#include "game.h"		//�Q�[���S�̂̃w�b�_�t�@�C��
#include "keyboard.h"	//�L�[�{�[�h�̏���
#include "FPS.h"		//FPS�̏���

//�\���̂̒�`

//�L�����N�^�[�̍\����
struct CHARACTOR
{
	int handle = -1;		//�摜�̃n���h���i�Ǘ��ԍ��j
	char path[255];			//�摜�̏ꏊ

	int x;					//X�ʒu
	int y;					//Y�ʒu
	int width;				//��
	int height;				//����

	int speed = 1;			//�ړ����x

	RECT coll;				//�����蔻��̗̈�(�l�p)
	BOOL IsDraw = FALSE;	//�摜���`��ł���
};

//����̍\����
struct MOVIE
{
	int handle = -1;		//����̃n���h��
	char path[255];			//����̃p�X

	int x;					//X�ʒu
	int y;					//Y�ʒu
	int width;				//��
	int height;				//����

	int Volume = 255;		//�{�����[���i�ŏ��j0�`255�i�ő�j
};

//���y�̍\����
struct AUDIO
{
	int handle = -1;	//���y�̃n���h��
	char path[255];		//���y�̃p�X

	int Volume = -1;	//�{�����[���iMIN 0 �` 255 MAX�j
	int playType = -1;
};

//�O���[�o���ϐ�
//�V�[�����Ǘ�����ϐ�
GAME_SCENE GameScene;		//���݂̃Q�[���̃V�[��
GAME_SCENE OldGameScene;	//�O��̃Q�[���̃V�[��
GAME_SCENE NextGameScene;	//���̃Q�[���̃V�[��

//�v���C�w�i�̓���
MOVIE playMovie;

//�v���C���[
CHARACTOR player;

//�S�[��
CHARACTOR Goal;

//���y
AUDIO TitleBGM;
AUDIO PlayBGM;
AUDIO EndBGM;

//���ʉ�
AUDIO PlayerSE;

//��ʂ̐؂�ւ�
BOOL IsFadeOut = FALSE;		//�t�F�[�h�A�E�g
BOOL IsFadeIn = FALSE;		//�t�F�[�h�C��

//�V�[���؂�ւ�
int fadeTimeMill = 2000;		//�؂�ւ��~���b
int fadeTimeMax = fadeTimeMill / 1000 * 60;	//�~���b���t���[���b�ŕϊ�

//�t�F�[�h�A�E�g
int fadeOutCntInit = 0;				//�����l
int fadeOutCnt = fadeOutCntInit;	//�t�F�[�h�A�E�g�̃J�E���^
int fadeOutCntMax = fadeTimeMax;	//�t�F�[�h�A�E�g�̃J�E���^MAX

//�t�F�[�h�A�C��
int fadeInCntInit = fadeTimeMax;	//�����l
int fadeInCnt = fadeInCntInit;		//�t�F�[�h�A�E�g�̃J�E���^
int fadeInCntMax = fadeTimeMax;				//�t�F�[�h�A�E�g�̃J�E���^MAX

//�v���g�^�C�v�錾
VOID Title(VOID);		//�^�C�g�����
VOID TitleProc(VOID);	//�^�C�g����ʂ̏���
VOID TitleDraw(VOID);	//�^�C�g����ʂ̕`��

VOID Play(VOID);		//�v���C���
VOID PlayProc(VOID);	//�v���C��ʂ̏���
VOID PlayDraw(VOID);	//�v���C��ʂ̕`��

VOID End(VOID);			//�G���h���
VOID EndProc(VOID);		//�G���h��ʂ̏���
VOID EndDraw(VOID);		//�G���h��ʂ̕`��

VOID Change(VOID);		//�؂�ւ����
VOID ChangeProc(VOID);	//�؂�ւ���ʂ̏���
VOID ChangeDraw(VOID);	//�؂�ւ���ʂ̕`��

VOID ChangeScene(GAME_SCENE scene);	//�V�[���؂�ւ�

VOID CollUpdatePlayer(CHARACTOR* chara);	//�����蔻��̗̈���X�V
VOID CollUpdate(CHARACTOR* chara);			//�����蔻��

BOOL OnCollRect(RECT a, RECT b);			//��`�Ƌ�`�̓����蔻��

BOOL GameLoad(VOID);	//�Q�[���̃f�[�^��ǂݍ���
VOID GameInit(VOID);

// �v���O������ WinMain ����n�܂�܂�
//Windows�̃v���O���~���O���@ = (WinAPI)�œ����Ă���
//DxLib�́ADirectX�Ƃ����A�Q�[���v���O���~���O���ȒP�Ɏg����d�g��
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log.txt���o�͂��Ȃ�
	ChangeWindowMode(TRUE);								//�E�B���h�E���[�h�ɐݒ�
	SetMainWindowText("GAME_TITLE");					//�E�B���h�E�̃^�C�g��
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�E�B���h�E�̉𑜓x��ݒ�
	SetWindowSize(GAME_WIDTH, GAME_HEIGHT);				//�E�B���h�E�̑傫����ݒ�
	SetBackgroundColor(255, 255, 255);					//�f�t�H���g�̔w�i�̐F
	SetWindowIconID(GAME_ICON_ID);						//�A�C�R���t�@�C����ǂݍ���
	SetWindowStyleMode(GAME_WINDOW_BAR);				//�E�B���h�E�o�[�̏��
	SetWaitVSyncFlag(TRUE);								//�f�B�X�v���C�̐���������L���ɂ���
	SetAlwaysRunFlag(TRUE);								//�E�B���h�E�������ƃA�N�e�B�u�ɂ���

	// �c�w���C�u��������������
	if (DxLib_Init() == -1)		
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	//�_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);

	//�ŏ��̃V�[���́A�^�C�g����ʂ���
	GameScene = GAME_SCENE_TITLE;

	//�Q�[���S�̂̏�����

	//�Q�[���ǂݍ���
	if (!GameLoad())
	{
		//�f�[�^�̓ǂݍ��݂Ɏ��s�����Ƃ�
		DxLib_End();		//DxLib�I��
		return -1;			//�ُ�I��
	}

	//�Q�[���̏�����
	GameInit();
	
	//�������[�v
	while (1)
	{
		if (ProcessMessage() != 0) { break; }	//���b�Z�[�W���󂯎�葱����
		if (ClearDrawScreen() != 0) { break; }	//��ʂ���������

		//�L�[�{�[�h���͂̍X�V
		AllKeyUpdate();

		//FPS�l�̍X�V
		FPSUpdate();

		//ESC�L�[�ŋ����I��
		if (KeyClick(KEY_INPUT_ESCAPE) == TRUE) { break; }

		//�ȑO�̃V�[�����擾
		if (GameScene != GAME_SCENE_CHANGE)
		{
			OldGameScene = GameScene;
		}

		//�V�[�����Ƃɏ������s��
		switch (GameScene)
		{
		case GAME_SCENE_TITLE:
			Title();			//�^�C�g�����
			break;
		case GAME_SCENE_PLAY:
			Play();				//�v���C���
			break;
		case GAME_SCENE_END:
			End();				//�G���h���
			break;
		case GAME_SCENE_CHANGE:
			Change();			//�؂�ւ����
			break;
		default:
			break;
		}

		//�V�[����؂�ւ���
		if (OldGameScene != GameScene)
		{
			//���݂̃V�[�����؂�ւ���ʂłȂ��Ƃ�
			if (GameScene != GAME_SCENE_CHANGE)
			{
				NextGameScene = GameScene;		//���̃V�[����ۑ�
				GameScene = GAME_SCENE_CHANGE;	//��ʐ؂�ւ��V�[���ɕς���
			}
		}

		//FPS�l��`��
		FPSDraw();

		//FPS�l��҂�
		FPSWait();
		
		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��
	}

	//�I���Ƃ��̏���
	DeleteGraph(playMovie.handle);		//������������ォ��폜
	DeleteGraph(player.handle);			//�摜���������ォ��폜
	DeleteGraph(Goal.handle);			//�摜���������ォ��폜

	DeleteSoundMem(TitleBGM.handle);	//���y���������ォ��폜
	DeleteSoundMem(PlayBGM.handle);		//���y���������ォ��폜
	DeleteSoundMem(EndBGM.handle);		//���y���������ォ��폜

	DeleteSoundMem(PlayerSE.handle);		//���y���������ォ��폜

	// �c�w���C�u�����g�p�̏I������
	DxLib_End();

	return 0;				// �\�t�g�̏I�� 
}

	/// <summary>
/// �Q�[���̃f�[�^��ǂݍ���
/// </summary>
/// <returns>�ǂݍ��߂���TRUE/�ǂݍ��߂Ȃ�������FALSE</returns>
	BOOL GameLoad()
	{
		//�v���C����̔w�i��ǂݍ���
		strcpyDx(playMovie.path, ".\\Movie\\PlayMovie.mp4");		//�p�X�̃R�s�[
		playMovie.handle = LoadGraph(playMovie.path);				//�摜�̓ǂݍ���

		//���悪�ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
		if (playMovie.handle == -1)
		{
			MessageBox(
				GetMainWindowHandle(),		//���C���̃E�B���h�E�n���h��
				playMovie.path,				//���b�Z�[�W�{��
				"����ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
				MB_OK						//�{�^��
			);

			return FALSE;		//�ǂݍ��ݎ��s

		}

		//�摜�̕��ƍ������擾
		GetGraphSize(playMovie.handle, &playMovie.width, &playMovie.height);

		//����̃{�����[��
		playMovie.Volume = 255;

		//�v���C���[�̉摜��ǂݍ���
		strcpyDx(player.path, ".\\Image\\Player.png");		//�p�X�̃R�s�[
		player.handle = LoadGraph(player.path);				//�摜�̓ǂݍ���

		//�摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
		if (player.handle == -1)
		{
			MessageBox(
				GetMainWindowHandle(),		//���C���̃E�B���h�E�n���h��
				player.path,				//���b�Z�[�W�{��
				"�摜�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
				MB_OK						//�{�^��
			);

			return FALSE;
		}

		//�摜�̕��ƍ������擾
		GetGraphSize(player.handle, &player.width, &player.height);

		//�S�[���̉摜��ǂݍ���
		strcpyDx(Goal.path, ".\\Image\\goal.png");		//�p�X�̃R�s�[
		Goal.handle = LoadGraph(Goal.path);				//�摜�̓ǂݍ���

		//�摜���ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
		if (Goal.handle == -1)
		{
			MessageBox(
				GetMainWindowHandle(),		//���C���̃E�B���h�E�n���h��
				Goal.path,				//���b�Z�[�W�{��
				"�摜�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
				MB_OK						//�{�^��
			);

			return FALSE;
		}

		//�摜�̕��ƍ������擾
		GetGraphSize(Goal.handle, &Goal.width, &Goal.height);

		//���y�̉摜��ǂݍ���
		strcpyDx(TitleBGM.path, ".\\Audio\\�����ԉ�.mp3");		//�p�X�̃R�s�[
		TitleBGM.handle = LoadSoundMem(TitleBGM.path);

		//���y���ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
		if (TitleBGM.handle == -1)
		{
			MessageBox(
				GetMainWindowHandle(),		//���C���̃E�B���h�E�n���h��
				TitleBGM.path,				//���b�Z�[�W�{��
				"���y�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
				MB_OK						//�{�^��
			);

			return FALSE;
		}

		TitleBGM.playType = DX_PLAYTYPE_LOOP;
		TitleBGM.Volume = 255;

		//���y�̉摜��ǂݍ���
		strcpyDx(PlayBGM.path, ".\\Audio\\Frenzy_Circus.mp3");		//�p�X�̃R�s�[
		PlayBGM.handle = LoadSoundMem(PlayBGM.path);

		//���y���ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
		if (PlayBGM.handle == -1)
		{
			MessageBox(
				GetMainWindowHandle(),		//���C���̃E�B���h�E�n���h��
				PlayBGM.path,				//���b�Z�[�W�{��
				"���y�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
				MB_OK						//�{�^��
			);

			return FALSE;
		}

		PlayBGM.playType = DX_PLAYTYPE_LOOP;
		PlayBGM.Volume = 255;

		//���y�̉摜��ǂݍ���
		strcpyDx(EndBGM.path, ".\\Audio\\�߂̌��_2.mp3");		//�p�X�̃R�s�[
		EndBGM.handle = LoadSoundMem(EndBGM.path);

		//���y���ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
		if (EndBGM.handle == -1)
		{
			MessageBox(
				GetMainWindowHandle(),		//���C���̃E�B���h�E�n���h��
				EndBGM.path,				//���b�Z�[�W�{��
				"���y�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
				MB_OK						//�{�^��
			);

			return FALSE;
		}

		PlayBGM.playType = DX_PLAYTYPE_LOOP;
		PlayBGM.Volume = 255;

		//���ʉ���ǂݍ���
		strcpyDx(PlayerSE.path, ".\\Audio\\�G�l���M�[�����߂�i���b�N�}�����j.mp3");		//�p�X�̃R�s�[
		PlayerSE.handle = LoadSoundMem(PlayerSE.path);

		//���y���ǂݍ��߂Ȃ������Ƃ��́A�G���[(-1)������
		if (PlayerSE.handle == -1)
		{
			MessageBox(
				GetMainWindowHandle(),		//���C���̃E�B���h�E�n���h��
				PlayerSE.path,				//���b�Z�[�W�{��
				"���y�ǂݍ��݃G���[�I",		//���b�Z�[�W�^�C�g��
				MB_OK						//�{�^��
			);

			return FALSE;
		}

		EndBGM.playType = DX_PLAYTYPE_BACK;
		EndBGM.Volume = 255;
		
		return TRUE;
	}

	/// <summary>
	/// �Q�[���f�[�^��������
	/// </summary>
	/// <param name=""></param>
	VOID GameInit(VOID)
	{
		//�v���C���[��������
		player.x = GAME_WIDTH / 2 - player.width / 2;		//������
		player.y = GAME_HEIGHT / 2 - player.height / 2;		//������
		player.speed = 500;
		player.IsDraw = TRUE;	//�`��ł���

			//�����蔻����X�V����
		CollUpdatePlayer(&player);		//�v���C���[�̓����蔻��̃A�h���X

		//�S�[����������
		Goal.x = GAME_WIDTH - Goal.width;		//������
		Goal.y = 0;		//������
		Goal.speed = 500;
		Goal.IsDraw = TRUE;	//�`��ł���

		//�����蔻����X�V����
		CollUpdate(&Goal);		//�v���C���[�̓����蔻��̃A�h���X
	}

/// <summary>
/// �V�[����؂�ւ���֐�
/// </summary>
/// <param name="scene">�V�[��</param>
VOID ChangeScene(GAME_SCENE scene)
{
	GameScene = scene;	//�V�[����؂�ւ�
	IsFadeIn = FALSE;	//�t�F�[�h�C�����Ȃ�
	IsFadeOut = TRUE;	//�t�F�[�h�A�E�g����

	return;
}

/// <summary>
/// �^�C�g�����
/// </summary>
VOID Title(VOID)
{
	TitleProc();	//����
	TitleDraw();	//�`��

	return;
}

/// <summary>
/// �^�C�g����ʂ̏���
/// </summary>
VOID TitleProc(VOID)
{

	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//BGM���~�߂�
		StopSoundMem(TitleBGM.handle);

		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�Q�[���̏�����
		GameInit();

		//�v���C��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_PLAY);

		return;
	}

	//BGM������Ă��Ȃ��Ƃ�
	if (CheckSoundMem(TitleBGM.handle) == 0)
	{
		//BGM�𗬂�
		PlaySoundMem(TitleBGM.handle,TitleBGM.playType);
	}

	return;
}

/// <summary>
/// �^�C�g����ʂ̕`��
/// </summary>
VOID TitleDraw(VOID)
{

	DrawString(0, 0, "�^�C�g�����", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �v���C���
/// </summary>
VOID Play(VOID)
{
	PlayProc();	//����
	PlayDraw();	//�`��

	return;
}

/// <summary>
/// �v���C��ʂ̏���
/// </summary>
VOID PlayProc(VOID)
{
	/*
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);
	}
	*/

	//BGM������Ă��Ȃ��Ƃ�
	if (CheckSoundMem(PlayBGM.handle) == 0)
	{
		//BGM�𗬂�
		PlaySoundMem(PlayBGM.handle, PlayBGM.playType);
	}

	//�v���C���[�̑���
	if (KeyDown(KEY_INPUT_UP) == TRUE)
	{
		player.y -= player.speed * fps.DaltaTime;

		//�������̌��ʉ���ǉ�
		if (CheckSoundMem(PlayerSE.handle) == 0)
		{
			PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
		}
	}

	if (KeyDown(KEY_INPUT_DOWN) == TRUE)
	{
		player.y += player.speed * fps.DaltaTime;

		//�������̌��ʉ���ǉ�
		if (CheckSoundMem(PlayerSE.handle) == 0)
		{
			PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
		}

	}

	if (KeyDown(KEY_INPUT_LEFT) == TRUE)
	{
		player.x -= player.speed * fps.DaltaTime;

		//�������̌��ʉ���ǉ�
		if (CheckSoundMem(PlayerSE.handle) == 0)
		{
			PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
		}
	}

	if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
	{
		player.x += player.speed * fps.DaltaTime;

		//�������̌��ʉ���ǉ�
		if (CheckSoundMem(PlayerSE.handle) == 0)
		{
			PlaySoundMem(PlayerSE.handle, PlayerSE.playType);
		}
	}

	//�����蔻����X�V����
	CollUpdatePlayer(&player);

	//�S�[���̓����蔻����X�V����
	CollUpdate(&Goal);

	//�v���C���[���S�[���ɓ���������
	if (OnCollRect(player.coll, Goal.coll) == TRUE)
	{
		//BGM���~�߂�
		StopSoundMem(PlayBGM.handle);

		//�G���h��ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_END);
		
		//�����������I��
		return;
	}

	return;
}

/// <summary>
/// �v���C��ʂ̕`��
/// </summary>
VOID PlayDraw(VOID)
{
	//�w�i�����`��

	//�����A���悪�Đ�����Ă��Ȃ��Ƃ�
	if (GetMovieStateToGraph(playMovie.handle) == 0)
	{
		//�Đ�����
		SeekMovieToGraph(playMovie.handle, 0);		//�V�[�N�o�[���ŏ��ɖ߂�
		PlayMovieToGraph(playMovie.handle);			//������Đ�
	}

	//�����`��(��ʂɍ��킹�ĉ摜�������L�΂�)
	DrawExtendGraph(0, 0, GAME_WIDTH, GAME_HEIGHT, playMovie.handle, TRUE);

	//�v���C���[��`��
	if (player.IsDraw == TRUE)
	{
		//�摜��`��
		DrawGraph(player.x, player.y, player.handle, TRUE);

		//�f�o�b�O�̎��́A�����蔻��̗̈��`��
		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(player.coll.left, player.coll.top, player.coll.right, player.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}

	//�S�[����`��
	if (Goal.IsDraw == TRUE)
	{
		//�摜��`��
		DrawGraph(Goal.x, Goal.y, Goal.handle, TRUE);

		//�f�o�b�O�̎��́A�����蔻��̗̈��`��
		if (GAME_DEBUG == TRUE)
		{
			//�l�p��`��
			DrawBox(Goal.coll.left, Goal.coll.top, Goal.coll.right, Goal.coll.bottom,
				GetColor(255, 0, 0), FALSE);
		}
	}

	DrawString(0, 0, "�v���C���", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// �G���h���
/// </summary>
VOID End(VOID)
{
	EndProc();	//����
	EndDraw();	//�`��

	return;
}

/// <summary>
/// �G���h��ʂ̏���
/// </summary>
VOID EndProc(VOID)
{
	if (KeyClick(KEY_INPUT_RETURN) == TRUE)
	{
		//�V�[���؂�ւ�
		//���̃V�[���̏������������ōs���Ɗy

		//BGM���~�߂�
		StopSoundMem(EndBGM.handle);

		//�^�C�g����ʂɐ؂�ւ�
		ChangeScene(GAME_SCENE_TITLE);

		return;
	}

	//BGM������Ă��Ȃ��Ƃ�
	if (CheckSoundMem(EndBGM.handle) == 0)
	{
		//BGM�𗬂�
		PlaySoundMem(EndBGM.handle, EndBGM.playType);
	}

	return;
}

/// <summary>
/// �G���h��ʂ̕`��
/// </summary>
VOID EndDraw(VOID)
{
	DrawString(0, 0, "�G���h���", GetColor(0, 0, 0));
	return;
}

/// <summary>
/// ��ʂ̐؂�ւ�
/// </summary>
VOID Change(VOID)
{
	ChangeProc();
	ChangeDraw();

	return;
}

/// <summary>
/// ��ʂ̐؂�ւ��̏���
/// </summary>
VOID ChangeProc(VOID)
{

	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		if (fadeInCnt > fadeInCntMax)
		{
			fadeInCnt--;	//�J�E���^�����炷
		}
		else
		{
			//�t�F�[�h�C���������I�����

			fadeInCnt = fadeInCntInit;	//�J�E���^��������
			IsFadeIn = FALSE;			//�t�F�[�h�C�������I��
		}
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		if (fadeOutCnt < fadeOutCntMax)
		{
			fadeOutCnt++;	//�J�E���^�����炷
		}
		else
		{
			//�t�F�[�h�A�E�g�������I�����

			fadeOutCnt = fadeOutCntInit;	//�J�E���^��������
			IsFadeOut = FALSE;				//�t�F�[�h�A�E�g�����I��
		}
	}

	//�؂�ւ���ʏI��
	if (IsFadeIn == FALSE && IsFadeOut == FALSE)
	{
		//�t�F�[�h�C�����Ă��Ȃ��A�t�F�[�h�A�E�g�����Ă��Ȃ��Ƃ�
		GameScene = NextGameScene;		//���̃V�[���ɐ؂�ւ�
		OldGameScene = GameScene;		//�ȑO�̃Q�[���V�[���X�V
	}


	return;
}

/// <summary>
/// ��ʂ̐؂�ւ��̕`��
/// </summary>
VOID ChangeDraw(VOID)
{
	//�ȑO�̃V�[����`��
	switch (OldGameScene)
	{
	case GAME_SCENE_TITLE:
		TitleDraw();		//�^�C�g����ʂ̕`��
		break;
	case GAME_SCENE_PLAY:
		PlayDraw();			//�v���C��ʂ̕`��
		break;
	case GAME_SCENE_END:
		EndDraw();			//�G���h��ʂ̕`��
		break;
	default:
		break;
	}

	//�t�F�[�h�C��
	if (IsFadeIn == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeInCnt / fadeInCntMax) * 255);
	}

	//�t�F�[�h�A�E�g
	if (IsFadeOut == TRUE)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, ((float)fadeOutCnt / fadeOutCntMax) * 255);
	}

	//�l�p��`��
	DrawBox(0, 0, GAME_WIDTH, GAME_HEIGHT, GetColor(0, 0, 0), TRUE);

	//�������I��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "�؂�ւ����", GetColor(0, 0, 0));

	return;
}

/// <summary>
/// �����蔻��̗̈攻��(�v���C���[)
/// </summary>
/// <param name="chara">�����蔻��̗̈�</param>
VOID CollUpdatePlayer(CHARACTOR* chara)
{
	chara->coll.left = chara->x + 84;
	chara->coll.top = chara->y + 4;
	chara->coll.right = chara->x + chara->width - 85;
	chara->coll.bottom = chara->y + chara->height - 6;

	return;
}

/// <summary>
/// �����蔻��̗̈攻��
/// </summary>
/// <param name="chara">�����蔻��̗̈�</param>
VOID CollUpdate(CHARACTOR* chara)
{
	chara->coll.left = chara->x;
	chara->coll.top = chara->y;
	chara->coll.right = chara->x + chara->width;
	chara->coll.bottom = chara->y + chara->height;

	return;
}

/// <summary>
/// ��`�Ƌ�`�̓����蔻��
/// </summary>
/// <param name="a">��`A</param>
/// <param name="b">��`B</param>
/// <returns>����������TURE�^������Ȃ�������FALSE</returns>
BOOL OnCollRect(RECT a, RECT b)
{
	if(
		a.left < b.right &&
		a.right > b.left &&
		a.top < b.bottom &&
		a.bottom > b.top 
		)
	{
		//�������Ă���Ƃ�
		return TRUE;
	}
	else
	{
		//�������Ă��Ȃ��Ƃ�
		return FALSE;
	}
}