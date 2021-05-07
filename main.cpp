//�w�b�_�[�t�@�C���̓ǂݍ���
#include "DxLib.h"		//DxLib���g�����͕K�v

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

	//�l�p�̈ʒu�����߂�
	int X = GAME_WIDTH / 2;			//���̒��S
	int Y = GAME_HEIGHT / 2;		//�c�̒��S

	//�l�p�̑傫�������߂�
	int width = 32;			//��
	int height = 32;		//����

	//�~�̔��a�����߂�
	int radius = 25;

	//���x
	int Speed = 1;
	int XSpeed = Speed;
	int YSpeed = Speed;

	//�_�u���o�b�t�@�����O�L����
	SetDrawScreen(DX_SCREEN_BACK);

	//�������[�v
	while (1)
	{
		if (CheckHitKeyAll() != 0)	//0�̎��A�����L�[�������ꂽ
		{
			break;	//�������[�v���甲����
		}

		//���b�Z�[�W���󂯎�葱����
		if (ProcessMessage() != 0)	//-1�̂Ƃ��A�G���[��E�B���h�E���������
		{
			break;	//�������[�v�𔲂���
		}

		//��ʂ���������
		if (ClearDrawScreen() != 0) { break; }

		//�l�p��`��
		//drawbox(
		//	x, y, x + width, y + height,
		//	getcolor(255, 0, 0),	//�F���擾
		//	true					//�h��Ԃ�
		//);

		//�~��`��
		DrawCircle(
			X, Y, radius,
			GetColor(0, 0, 0),
			FALSE, 5
		);

		DrawCircle(
			X, Y, radius,
			GetColor(255, 0, 0),
			TRUE
		);

		X += XSpeed;		//�l�p�̈ʒu���E�ɂ��炷

		//�@�l�p���΂߉E��
		Y += YSpeed;

		//�A�l�p����ʂ̒[�ɗ�����A�ړ���������𔽓]
		if (X - radius < 0 || X + radius > GAME_WIDTH)		//��ʂ�������o���Ƃ�
		{
			//+1 �Ȃ�A-1����/ -1�Ȃ�΁A+1�ɂ���
			XSpeed = -XSpeed;	//�ړ���������𔽓]

			//�ǂɓ��������瑬���Ȃ�
			if (XSpeed > 0) { XSpeed += 2; }
			else if (XSpeed < 0) { XSpeed -= 2; }
		}

		if (Y - radius < 0 || Y + radius > GAME_HEIGHT)		//��ʂ�������o���Ƃ�
		{
			//+1 �Ȃ�A-1����/ -1�Ȃ�΁A+1�ɂ���
			YSpeed = -YSpeed;	//�ړ���������𔽓]

			//�ǂɓ��������瑬���Ȃ�
			if (YSpeed > 0) { YSpeed += 2; }
			else if (YSpeed < 0) { YSpeed -= 2; }
		}
		
		ScreenFlip();	//�_�u���o�b�t�@�����O������ʂ�`��
	}


	// �c�w���C�u�����g�p�̏I������
	DxLib_End();

	return 0;				// �\�t�g�̏I�� 
}