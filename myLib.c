#include "myLib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int r, int c, unsigned short color) {
	videoBuffer[OFFSET(r, c, 240)] = color;
}

void drawRect(int row, int col, int height, int width, unsigned short color) {
	for(int r=0; r<height; r++) {
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(row+r, col, 240)];
		DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
	}
}

void drawImage3(int r, int c, int width, int height, const u16* image) {
	for (int i = 1; i < height; i++) {
		DMA[3].src = image + i * width;
		DMA[3].dst = videoBuffer + (c + i) * 240 + r;
		DMA[3].cnt = width | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON; 
	}
}

void delay(int n) {
	volatile int x = 0;
	for(int i=0; i<n*10000; i++) {
		x++;
	}
}

void fillScreen(volatile u16 color) {
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = 38400 | DMA_ON | DMA_SOURCE_FIXED;
}	


void waitForVBlank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

