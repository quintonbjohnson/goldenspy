#include <stdlib.h>
#include "myLib.h"
#include "text.h"
#include "mainMenu.h"
#include "mainMenu.c"
#include "gameOver.h"
#include "gameOver.c"
#include "playerSprite.h"
#include "playerSprite.c"
#include "enemySprite.h"
#include "enemySprite.c"
#include "winScreen.h"
#include "winScreen.c"

char* message1 = "Lives:";

PLAYER player;
ENEMY enemy[4];

int main() {

	REG_DISPCNT = MODE_3 | BG2_ENABLE;

	enum GBAState state = STARTSCREEN;

	waitForVBlank();

	// Game loop
	while (1) {
		switch(state) {

			// Home screen
			case STARTSCREEN:
				drawImage3(0, 0, MAINMENU_WIDTH, MAINMENU_HEIGHT, mainMenu);
				state = STARTSCREEN_NODRAW;
				break;

			case STARTSCREEN_NODRAW:
				while (!KEY_DOWN_NOW(BUTTON_A)) {
					player.lives = 3;
					state = PLAYSCREEN;
				}

				break;

			// Play screen where you are actually playing the game
			case PLAYSCREEN:
				// Go to play function where game logic occurs
				state = play();

				break;
			
			// Lose Screen
			case ENDSCREEN:
				drawImage3(0, 0, GAMEOVER_WIDTH, GAMEOVER_HEIGHT, gameOver);
				state = ENDSCREEN_NODRAW;
				break;

			case ENDSCREEN_NODRAW:
				while (!KEY_DOWN_NOW(BUTTON_SELECT)) {
					state = STARTSCREEN;
				}

				break;

			// Win screen
			case WINSCREEN:
				drawImage3(0, 0, WINSCREEN_WIDTH, WINSCREEN_HEIGHT, winScreen);
				state = WINSCREEN_NODRAW;
				
				break;

			case WINSCREEN_NODRAW:
				while (!KEY_DOWN_NOW(BUTTON_SELECT)) {
					state = STARTSCREEN;
				}

				break;
		}
	}
}

// Play function where main game logic occurs
int play() {

	// Initialize Player struct
	player.x = 0;
	player.y = 0;
	int oldx = 0;
	int oldy = 0;

	// Initialize Enemy structs
	enemy[0].x = 40;
	enemy[0].y = 30;
	enemy[0].delx = 1;
	enemy[0].delx = 1;

	enemy[1].x = 120;
	enemy[1].y = 10;
	enemy[1].delx = 1;
	enemy[1].delx = 1;

	// Faster enemies (#2 and #4)
	enemy[2].x = 80;
	enemy[2].y = 100;
	enemy[2].delx = 1;
	enemy[2].delx = 1;

	enemy[3].x = 170;
	enemy[3].y = 30;
	enemy[3].delx = 1;
	enemy[3].delx = 1;

	fillScreen(WHITE);

	// Draw boundaries

	// Top rectangle
	drawRect(0, 40, 16, 208, BLACK);

	// Bottom rectangle
	drawRect(150, 0, 10, 208, BLACK);

	// Left rectangle
	drawRect(16, 0, 200, 10, BLACK);

	// Right rectangle
	drawRect(16, 230, 200, 10, BLACK);

	// Draw player
	drawImage3(player.x, player.y, PLAYERSPRITE_WIDTH, PLAYERSPRITE_HEIGHT, playerSprite);
	int canGoFurther = 1;

	// Draw enemies
	for (int i = 0; i < NUMOBJS; i++) {
		drawImage3(enemy[i].x, enemy[i].y, ENEMYSPRITE_WIDTH, ENEMYSPRITE_HEIGHT, enemySprite);
	}
	
	while (1) {
		
		// Keep track of the player's old coordinates	
		oldy = player.y;
		oldx = player.x;	

		// Move the player around
		if (canGoFurther == 1) {
			if (KEY_DOWN_NOW(BUTTON_UP) && !KEY_DOWN_NOW(BUTTON_RIGHT) && !KEY_DOWN_NOW(BUTTON_LEFT)) {
				player.y -= 2;
			}

			if (KEY_DOWN_NOW(BUTTON_DOWN) && !KEY_DOWN_NOW(BUTTON_RIGHT) && !KEY_DOWN_NOW(BUTTON_LEFT)) {
				player.y += 2;
			}

			if (KEY_DOWN_NOW(BUTTON_LEFT) && !KEY_DOWN_NOW(BUTTON_RIGHT)) {
				player.x -= 2;
			}

			if (KEY_DOWN_NOW(BUTTON_RIGHT) && !KEY_DOWN_NOW(BUTTON_LEFT)) {
				player.x += 2;
			}
		}

		// Update player position 
		waitForVBlank();

		// Draw over player's path
		drawRect(oldy, oldx, 32, 32, WHITE);

		// Check boundary collision
		// Top left boundary
		if (player.y < 3 && player.x < 3) {
			player.x += 2;
		}
		if (player.y < 5 && player.x < 32) {
			player.y += 2;
		}
		// Top boundary
		if (player.y < 16 && player.x > 15) {
			player.y += 2;
		}
		// Bottom boundary
		if (player.y > 124 && player.x < 208) {
			player.y -= 2;
		}
		// Left boundary
		if (player.y > 0 && player.x < 10) {
			player.x += 2;
		} 
		// Right boundary
		if (player.x > 210) {
			player.x -= 2;
		}

		// Enemy patrols
		for (int i = 0; i < NUMOBJS / 2; i++) {
			if (enemy[i].y < 16) {
				enemy[i].dely = 1;
			}
			if (enemy[i].y > 110) {
				enemy[i].dely = -1;
			}
			if (enemy[i].dely == 1) {
				enemy[i].y += 1;
			} else {
				enemy[i].y -= 1;
			}
		}

		// Faster enemy patrols
		for (int i = NUMOBJS / 2; i < NUMOBJS; i++) {
			if (enemy[i].y < 16) {
				enemy[i].dely = 1;
			}
			if (enemy[i].y > 110) {
				enemy[i].dely = -1;
			}
			if (enemy[i].dely == 1) {
				enemy[i].y += 2;
			} else {
				enemy[i].y -= 2;
			}
		}

		// Update enemy positions
		for (int i = 0; i < NUMOBJS; i++) {
			drawImage3(enemy[i].x, enemy[i].y, ENEMYSPRITE_WIDTH, ENEMYSPRITE_HEIGHT, enemySprite);
			int isColliding = collision(player.x, player.y, enemy[i].x, enemy[i].y);
			if (isColliding == 1) {
				player.lives -= 1;
				return PLAYSCREEN;
			}
		}

		// Redraw player's position
		drawImage3(player.x, player.y, PLAYERSPRITE_WIDTH, PLAYERSPRITE_HEIGHT, playerSprite);

		// Top boundary
		drawRect(0, 32, 16, 208, BLACK);

		// Bottom boundary
		drawRect(144, 0, 16, 208, BLACK);

		// Left boundary
		drawRect(16, 0, 200, 10, BLACK);

		// Right boundary
		drawRect(16, 230, 200, 10, BLACK);

		// Showing number of lives left
		drawString(150, 10, message1, RED);
		char message2 = player.lives + '0';
		drawChar(150, 50, message2, RED);

		// Win state
		if (player.x >= 200 && player.y >= 140) {
			return WINSCREEN;
		}

		// Lose state
		if (player.lives < 0) {
			return ENDSCREEN;
		}

		// Exit state
		while (KEY_DOWN_NOW(BUTTON_SELECT)) {
			return STARTSCREEN;
		}
	}		
}

// Check if the player and an enemy are colliding
int collision(int playerx, int playery, int enemyx, int enemyy) {
	if (playerx > enemyx - 12 && playerx < enemyx + 16 && playery > enemyy - 12 && playery < enemyy + 16) {
		return 1;
	} else {
		return 0;
	}
}
