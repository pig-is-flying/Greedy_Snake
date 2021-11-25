#include<ctime>
#include<conio.h>
#include<graphics.h>

#define BLOCK_SIZE 20
#define WIDTH 40
#define HEIGHT 30

int  block[HEIGHT][WIDTH] = { 0 };
char move_direction,hit;
int  food_x, food_y;
int isGameOver =0;

bool timer(clock_t ms)//
{
	static int t = 0;
	if (clock() - t > ms)
	{
		t = clock();
		return true;
	}
	return false;
}

void initialize()//
{
	srand(time(NULL));
	initgraph(WIDTH * BLOCK_SIZE, HEIGHT * BLOCK_SIZE);
	
	block[HEIGHT / 2][WIDTH / 2-3] = 1;
	for (int i = 1; i < 4; i++)
		block[HEIGHT / 2][WIDTH / 2 - i-3] = i + 1;

	food_x = rand() % 29;
	food_y = rand() % 29;
	move_direction = 'd';
}

void draw()
{
	BeginBatchDraw();
	cleardevice();
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			if (block[y][x] > 0)
				setfillcolor(HSVtoRGB(block[y][x] * 10, 0.9, 1));
			else
				setfillcolor(RGB(150, 150, 150));
			
			fillrectangle(x * BLOCK_SIZE, y * BLOCK_SIZE, (x + 1) * BLOCK_SIZE, (y + 1) * BLOCK_SIZE);
		}
	}
	setfillcolor(RGB(0, 250, 0));
	fillrectangle(food_x * BLOCK_SIZE, food_y * BLOCK_SIZE, (food_x + 1) * BLOCK_SIZE, (food_y + 1) * BLOCK_SIZE);
	setlinecolor(RGB(200, 200, 200));
	EndBatchDraw();
}

void move()
{
	for (int y = 0; y < HEIGHT; y++)
		for (int x = 0; x < WIDTH; x++)
			if (block[y][x] > 0)
				block[y][x]++;
	int old_tail_x, old_tail_y, old_head_x, old_head_y;
	int max = 0;//
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			if (block[y][x] > max)//
			{
				max = block[y][x];
				old_tail_x = x;
				old_tail_y = y;
			}
			if (block[y][x] == 2)//
			{
				old_head_x = x;
				old_head_y = y;
			}
		}
	}
	int new_head_x = old_head_x, new_head_y = old_head_y;
	if (move_direction == 'a' || move_direction == 'A')
		new_head_x = old_head_x - 1;
	else if (move_direction == 'd' || move_direction == 'D')
		new_head_x = old_head_x + 1;
	else if (move_direction == 'w' || move_direction == 'W')
		new_head_y = old_head_y - 1;
	else if (move_direction == 's' || move_direction == 'S')
		new_head_y = old_head_y + 1;
	
	if (new_head_x >= WIDTH)new_head_x = 0;
	else if (new_head_y >= HEIGHT)new_head_y = 0;
	else if (new_head_x < 0)new_head_x = WIDTH-1;//
	else if (new_head_y < 0)new_head_y = HEIGHT-1;
	
	if (block[new_head_y][new_head_x] > 1)
		isGameOver = 1;
	else block[new_head_y][new_head_x] = 1;
	if (new_head_y == food_y && new_head_x == food_x)
	{
		food_x = rand() % 29;
		food_y = rand() % 29;
		if (block[food_y][food_x] > 1)//
		{
			food_x = rand() % 29;
			food_y = rand() % 29;
		}
	}
	else block[old_tail_y][old_tail_x] = 0;//
}

void input()
{
	if (_kbhit())
		move_direction = _getch();
	//
}

void GameOver()
{
	outtextxy(WIDTH / 2, HEIGHT / 2, _T("Game Over!"));
}

int main()
{
    initialize();
	while (1)
	{
		input();
		if (timer(120))
		{
			draw();
			move();
			if (isGameOver)
				break;
		}
	}
	GameOver();
	hit = _getch();
	closegraph();
    return 0;
}
