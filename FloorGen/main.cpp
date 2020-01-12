#include <Windows.h>
#include <d3dx9.h>
#include <time.h>
#include "common.h"
#include "mydirect3d.h"
#include "texture.h"
#include "meshLoader.h"
#include "input.h"
#include "sprite.h"
#include "sceneManagement.h"
#include "Title.h"
#include "result.h"
#include "camera.h"
#include "floorgenerator.h"
#include "enemy.h"
#include "crosshair.h"


//���C�u�����t�@�C���̃����N�iexe�t�@�C���Ɋ܂߂�j
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "dinput8.lib")

#define CLASS_NAME      "GameWindow"
#define WINDOW_CAPTION  "�Ƌ�V���[�^�["

// window
static HWND g_hWnd;

/*-----------------------------------------------------------------------
   �v���g�^�C�v�錾
------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc(HWND g_hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void InitRenderState();

static bool InitLibrary();
static void FinalizeLibrary();

static bool InitTitle();
static void UpdateTitle();
static void DrawTitle();
static void FinalizeTitle();

static bool InitGame();
static void UpdateGame();
static void DrawGame();
static void FinalizeGame();

static bool InitResult();
static void UpdateResult();
static void DrawResult();
static void FinalizeResult();


/*-----------------------------------------------------------------------
   ���C��
------------------------------------------------------------------------*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	srand(time(NULL));

	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

	RegisterClass(&wc);

	//�E�B���h�E�X�^�C���i�����ځj�̐ݒ�
	DWORD window_style;
	 window_style = WS_OVERLAPPEDWINDOW;

	//��{��`���W
	RECT window_rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//�w�肵���N���C�A���g�̈�ɍ��킹�ċ�`�𒲐�
	AdjustWindowRect(&window_rect, window_style, FALSE);

	//�E�B���h�E�̋�`���W���畝�ƍ������v�Z
	int window_width = window_rect.right - window_rect.left;
	int window_height = window_rect.bottom - window_rect.top;

	//�f�B�X�v���C�̉𑜓x�̎擾
	int desktop_width = GetSystemMetrics(SM_CXSCREEN);
	int desktop_height = GetSystemMetrics(SM_CYSCREEN);

	//��ʂ̒��ɃE�B���h�E��\������悤���W�v�Z
	//��������ʂ��傫���E�B���h�E�͍���ɍ��킹�ĕ\��
	int window_x = max((desktop_width - window_width) / 2, 0);
	int window_y = max((desktop_height - window_height) / 2, 0);

	g_hWnd = CreateWindow(
		CLASS_NAME,
		WINDOW_CAPTION,
		window_style,
		window_x,
		window_y,
		window_width,
		window_height,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	//�E�B���h�E���쐬�ł��Ȃ�������I��
	if (g_hWnd == NULL)
	{
		return 0;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	// init
	if (!InitLibrary())
	{
		//���������s
		return -1;
	}

	Keyboard_Initialize(hInstance, g_hWnd);

	ShowCursor(false);

	MSG msg = {};
	bool init_title = false;
	bool init_result = false;
	bool init_game = false;

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// update and draw every frame
			Sleep(7);
			Keyboard_Update();

			switch (GetScene())
			{
			case SCENE_TITLESCREEN:
				if (!init_title)
				{
					FinalizeResult();
					InitTitle();
					init_result = false;
					init_title = true;
				}

				UpdateTitle();
				DrawTitle();
				break;

			case SCENE_GAMESCREEN:
				if (!init_game)
				{
					FinalizeTitle();
					InitGame();
					init_title = false;
					init_game = true;
				}

				UpdateGame();
				DrawGame();
				break;

			case SCENE_RESULTSCREEN:
				if (!init_result)
				{
					FinalizeGame();
					InitResult();
					init_game = false;
					init_result = true;
				}

				UpdateResult();
				DrawResult();
				break;

			default:
				break;
			}
		}
	}

	//�Q�[���̏I������
	FinalizeTitle();
	FinalizeGame();
	FinalizeLibrary();
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND g_hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			std::exit(0);
		}
		break;

	case WM_CLOSE:
		if (MessageBox(g_hWnd, "�{���ɏI�����Ă�낵���ł����H", "�m�F", MB_OKCANCEL | MB_DEFBUTTON2) == IDOK) {
			DestroyWindow(g_hWnd);
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(g_hWnd, uMsg, wParam, lParam);
}


bool InitLibrary()
{
	//Direct3D�C���^�[�t�F�C�X�쐬
	if (!MyDirect3D_Initialize(g_hWnd))
	{
		return false;
	}

	InitRenderState();

	Texture_Load();
	LoadMesh();
	InitSprite();
	InitScene();

	return true;
}

void FinalizeLibrary()
{
	Keyboard_Finalize();
	UninitSprite();
	UninitMesh();
	Texture_Release();

	MyDirect3D_Finalize();
}


bool InitTitle()
{
	InitTitleScreen();

	return true;
}

void FinalizeTitle()
{
	FinalizeTitleScreen();
}

void UpdateTitle()
{
	UpdateTitleScreen();
}

void DrawTitle()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//�o�b�N�o�b�t�@�[�̃N���A ���F��230�C0�C255�C255
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0F, 0);

	// draw 3d meshes
	pDevice->BeginScene();

	// draw sprites
	SpriteStart();

	DrawTitleScreen();

	// end draw
	SpriteEnd();
	pDevice->EndScene();

	// draw to screen
	pDevice->Present(NULL, NULL, NULL, NULL);
}


bool InitGame()
{
	InitCamera();
	InitPlane();
	InitEnemy();
	InitCrosshair();

	return true;
}

void FinalizeGame()
{
	UninitCrosshair();
	UninitEnemy();
	UninitPlane();
	UninitCamera();
}

void UpdateGame()
{
	UpdateEnemy();
	UpdateCrosshair();
	UpdateCamera();
}

void DrawGame()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//�o�b�N�o�b�t�@�[�̃N���A ���F��230�C0�C255�C255
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0F, 0);

	// draw 3d meshes
	pDevice->BeginScene();

	DrawPlane();
	DrawEnemy();

	// draw sprites
	SpriteStart();

	DrawCrosshair();

	// end draw
	SpriteEnd();
	pDevice->EndScene();

	// draw to screen
	pDevice->Present(NULL, NULL, NULL, NULL);
}


bool InitResult()
{
	InitResultScreen();

	return true;
}

void FinalizeResult()
{
	FinalizeResultScreen();
}

void UpdateResult()
{
	UpdateResultScreen();
}

void DrawResult()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	//�o�b�N�o�b�t�@�[�̃N���A ���F��230�C0�C255�C255
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0F, 0);

	// draw 3d meshes
	pDevice->BeginScene();

	// draw sprites
	SpriteStart();

	DrawResultScreen();

	// end draw
	SpriteEnd();
	pDevice->EndScene();

	// draw to screen
	pDevice->Present(NULL, NULL, NULL, NULL);
}


// init render state
void InitRenderState()
{
	auto device = MyDirect3D_GetDevice();

	// init 3d rendering
	device->SetRenderState(D3DRS_LIGHTING, false);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	device->SetRenderState(D3DRS_ZENABLE, true);
	device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(100, 100, 100));
	device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

	// set alpha blending
	device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// set source of color and alpha
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);

	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

	// set texture sampling
	//device->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 8);
	//device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	//device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
}
