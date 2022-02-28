#pragma once

#include "gbafe.h"

struct Proc_AtMenu {
	/* 00 */ PROC_HEADER;
	/* 29 */ u8 unit_count;
	/* 2A */ u8 max_count;
	/* 2B */ u8 cur_count;
	/* 2C */ u8 unk_2C;
	/* 2D */ u8 unk_2D;
	/* 2E */ u8 unk_2E;
	/* 2F */ u8 unk_2F;
	/* 30 */ u8 unk_30;
	/* 31 */ u8 unk_31;
	/* 32 */ u8 unk_32;
	/* 33 */ u8 state;
	/* 34 */ u8 unk_34;
	/* 35 */ u8 unk_35;
	/* 36 */ u8 unk_36;
	/* 38 */ u8 unk_38[0x3C - 0x38];
	/* 3C */ u16 unk_3C;
	/* 3E */ u16 unk_3E; 
	/* 40 */ u32 unk_40;
};



struct PrepScreenUnitList {
	struct Unit* units[0x40];
	s32 size;
	u32 char_id;
};

extern struct PrepScreenUnitList gPrepScreenUnitList; // 0x20121CC


#define gPrepTexts gUnknown_0201117C
extern struct TextHandle gPrepTexts[]; // 0201117C

void SetPrepScreenUnitListCharID(u32 index); // 80953C0