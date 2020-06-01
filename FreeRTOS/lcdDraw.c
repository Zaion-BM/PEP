#include "../application/lcdDraw.h"

SegmentLCD_UpperCharSegments_TypeDef upperCharSegments[SEGMENT_LCD_NUM_OF_UPPER_CHARS];
SegmentLCD_LowerCharSegments_TypeDef lowerCharSegments[SEGMENT_LCD_NUM_OF_LOWER_CHARS];

void lcdClearLowerSegment()
{
	for (uint8_t p = 0; p < SEGMENT_LCD_NUM_OF_LOWER_CHARS; p++)
		{
			lowerCharSegments[p].raw = 0;
			SegmentLCD_LowerSegments(lowerCharSegments);
		}
}

void lcdDrawBasket(uint8_t position)
{
	lowerCharSegments[position].d = 1;
	SegmentLCD_LowerSegments(lowerCharSegments);
}


void lcdDrawScore(uint8_t score, uint8_t life)
{
	SegmentLCD_Number(life*100 + score);
	SegmentLCD_Symbol(LCD_SYMBOL_COL10,1);
}


void lcdDrawBanana(uint8_t position,uint8_t age)
{
	switch(age)
	{
	case 0:
		lowerCharSegments[position].a = 1;
		SegmentLCD_LowerSegments(lowerCharSegments);
		return;
	case 1:
		lowerCharSegments[position].j = 1;
		SegmentLCD_LowerSegments(lowerCharSegments);
		return;
	case 2:
		lowerCharSegments[position].p = 1;
		SegmentLCD_LowerSegments(lowerCharSegments);
		return;
	case 3:
		return;
	}
}
