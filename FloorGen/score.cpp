#include <Windows.h>
#include <string>
#include "common.h"
#include "score.h"
#include "sprite.h"
#include "texture.h"
#include "sceneManagement.h"

#define FIGURE_NUM 4
#define SCORE_SPEED 3.0f

static int g_score;
static Sprite g_scoreSprite;
static int g_finalScore;


void AddScore(int value)
{
	g_finalScore += value;
}

void ResetScore()
{
	g_score = g_finalScore = 0;
}


void InitScore()
{
	g_score = 0;
	g_finalScore = 0;

	g_scoreSprite = Sprite(Texture_GetTexture(TEXTURE_INDEX_SCORE), D3DXVECTOR3(400, 200, 0),
		D3DXVECTOR3(Texture_GetWidth(TEXTURE_INDEX_SCORE) / 2, Texture_GetHeight(TEXTURE_INDEX_SCORE) / 2, 0),
		0, D3DXVECTOR2(0.5f, 0.5f), D3DCOLOR_RGBA(255,255,255, 255));
}

void UpdateScore()
{
	if (g_score < g_finalScore)
	{
		g_score += SCORE_SPEED;
	}
	else
	{
		g_score = g_finalScore;
	}
}

void DrawScore()
{
	int width = Texture_GetWidth(TEXTURE_INDEX_SCORE);
	int height = Texture_GetHeight(TEXTURE_INDEX_SCORE);

	RECT ScoreRect;

	if (GetScene() == SCENE_RESULTSCREEN)
	{
		g_scoreSprite.position.x = SCREEN_WIDTH / 2.0F + 250;
		g_scoreSprite.position.y = SCREEN_HEIGHT / 2.0F + 100;
	}
	else
	{
		g_scoreSprite.position.x = 600.0F;
		g_scoreSprite.position.y = 200.0F;
	}

	int fig = 0;
	int s = g_score;

	do
	{
		//表示する位の数字を取り出す
		int n = s % 10;

		ScoreRect.left = n % 5 * 0.2f * width;	//u座標
		ScoreRect.top = n / 5 * 0.5f * height;	//v座標
		ScoreRect.right = ScoreRect.left + 0.2f * width;
		ScoreRect.bottom = ScoreRect.top + 0.5f * height;

		//g_scoreSprite.color = D3DCOLOR_RGBA(rand() % 61 + 1, rand()%122 + 122, 60, 255);
		g_scoreSprite.color = D3DCOLOR_RGBA(255,0,0, 255);

		SpriteDraw(g_scoreSprite, true, &ScoreRect);

		//次の位を表示するため10で割る
		s *= 0.1F;

		//表示位置を左にずらす
		g_scoreSprite.position.x -= 0.2F * width * 0.3F;

		fig++;
	} while (fig < FIGURE_NUM);
}
