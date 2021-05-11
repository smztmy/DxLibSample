//�w�b�_�[�t�@�C���̓ǂݍ���
#include "DxLib.h"		//DxLib���g�����͕K�v

#include "keyboard.h"	//�L�[�{�[�h�̏���

//�}�N����`
#define GAME_TITLE  "�Q�[���^�C�g��"	//�Q�[���^�C�g��
#define GAME_WIDTH  1280				//�Q�[����ʂ̕�
#define GAME_HEIGHT 720					//�Q�[����ʂ̍���
#define GAME_COLOR  32					//�Q�[����ʂ̐F��

#define GAME_ICON_ID	333				//�Q�[����ICO��ID

#define GAME_WINDOW_BAR	0				//�E�B���h�E�o�[�̎��

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

	//�~�̒��S�_
	int X = GAME_WIDTH / 2;
	int Y = GAME_HEIGHT / 2;

	/*int x = GAME_WIDTH / 4;
	int y = GAME_HEIGHT / 4;*/
	//�~�̔��a
	int radius = 50;

	//�������[�v
	while (1)
	{
		
		if (ProcessMessage() != 0)	{ break; }	//���b�Z�[�W���󂯎�葱����
		if (ClearDrawScreen() != 0) { break; }	//��ʂ���������

		//�L�[�{�[�h���͂̍X�V
		AllKeyUpdate();

		//�L�[����
		if (KeyDown(KEY_INPUT_UP) == TRUE)
		{
			Y--;	//��Ɉړ�
		}
		if (KeyDown(KEY_INPUT_DOWN) == TRUE)
		{
			Y++;	//���Ɉړ�
		}
		if (KeyDown(KEY_INPUT_LEFT) == TRUE)
		{
			X--;	//���Ɉړ�
		}
		if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
		{
			X++;	//�E�Ɉړ�
		}

		/*if (KeyDown(KEY_INPUT_UP) == TRUE)
		{
			y--;	//��Ɉړ�
		}
		if (KeyDown(KEY_INPUT_DOWN) == TRUE)
		{
			y++;	//���Ɉړ�
		}
		if (KeyDown(KEY_INPUT_LEFT) == TRUE)
		{
			x--;	//���Ɉړ�
		}
		if (KeyDown(KEY_INPUT_RIGHT) == TRUE)
		{
			x++;	//�E�Ɉړ�
		}*/

		DrawCircle(X, Y, radius, GetColor(255, 255, 0), TRUE);

		/*DrawCircle(x, y, radius, GetColor(255, 255, 0), TRUE);*/
		
		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��
	}


	// �c�w���C�u�����g�p�̏I������
	DxLib_End();

	return 0;				// �\�t�g�̏I�� 
}