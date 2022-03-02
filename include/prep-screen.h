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

struct Proc_PrepMainMenuCmd {
	/* 00 */ PROC_HEADER;
	/* 29 */ u8 pad_29[0x2C - 0x29];
	/* 2C */ void (*effect)(ProcPtr);
	/* 30 */ int msg_rtext;
	/* 34 */ u32 msg;
	/* 38 */ u8 color;
	/* 39 */ u8 index;
	/* 3A */ u8 pad_3A[0x3C - 0x3A];
	/* 3C */ struct TextHandle text;
};


struct Proc_PrepMainMenu {
	
	/* 00 */ PROC_HEADER;
	/* 29 */ s8 rtext_on;
	/* 2A */ u8 cur_index;
	/* 2B */ u8 max_index;
	/* 2C */ u8 unk_2C[0x33 - 0x2C];
	/* 33 */ u8 end_state;
	/* 34 */ s16 xPos;
	/* 36 */ s16 yPos;
	/* 38 */ struct Proc_PrepMainMenuCmd* cmds[0x8];
	/* 58 */ int (*b_press)(ProcPtr);
	/* 5C */ int (*start_press)(ProcPtr);
	/* 60 */ void (*on_end)(ProcPtr parent);
};


struct PrepScreenUnitList {
	struct Unit* units[0x40];
	s32 size;
	u32 char_id;
};

extern struct PrepScreenUnitList gPrepScreenUnitList; // 0x20121CC
extern const struct ProcCmd gProc_PrepScreenMenu[]; // 8A186EC
extern const struct ProcCmd gProc_PrepScreenMenuDummyItem[]; // 8A186DC
extern const struct ProcCmd gProc_PrepScreenMenu[]; // 8A186EC

#define gPrepTexts gUnknown_0201117C
extern struct TextHandle gPrepTexts[]; // 0201117C

void SetPrepScreenUnitListCharID(u32 index); // 80953C0
void SetPrepScreenMenuItem(int index, void* effect_routine, int color, int msg, int msg_rtext); // 8097024