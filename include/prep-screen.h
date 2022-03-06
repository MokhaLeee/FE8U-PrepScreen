#pragma once

// ====================================================

// C-Lib version: FE-CLib-Mokha
// https://github.com/MokhaLeee/FE-CLib-Mokha.git

#include "gbafe.h"
// ====================================================

struct Proc_SALLYCURSOR {
	
	/* 00 */ PROC_HEADER;
	/* 29 */ u8 pad_29[0x58 - 0x29];
	/* 58 */ u32 status;
	/* 5C */
};


enum{
	
	// status of proc: Sally-Cursor (proc_prep + 0x58)
	
	STAT_PREPMAIN_NORMAL = 1,
	STAT_PREPMAIN_VIEWMAP = 2,
	
	
};

struct Proc_AtMenu {
	/* 00 */ PROC_HEADER;
	/* 29 */ u8 unit_count;
	/* 2A */ u8 max_counter; // Total unit number can be on battle
	/* 2B */ u8 cur_counter; // Total unit number to be on battle
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
	/* 36 */ u8 end_prep;
	/* 38 */ u8 unk_38[0x3C - 0x38];
	/* 3C */ u16 yDiff; // y Pos offset of Unit SMS
	/* 3E */ u16 unk_3E; 
	/* 40 */ u32 unk_40;
};


struct Proc_PrepMainMenuCmd;

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
	/* 58 */ int (*on_bPress)(ProcPtr);
	/* 5C */ int (*on_startPress)(ProcPtr);
	/* 60 */ void (*on_end)(ProcPtr parent);
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


struct Proc_PrepUnit {
	/* 00 */ PROC_HEADER;
	/* 29 */ u8 cur_counter; // Total unit number to be on battle
	/* 2A */ u8 max_counter; // Total unit number can be on battle
	/* 2B */ u8 unk_2B;
	/* 2C */ u16 list_num_pre; // pre unit index in prep-list(for scroll)
	/* 2E */ u16 list_num_cur; // current unit index in prep-list
	/* 30 */ u16 yDiff_cur; // y Pos offset of Unit SMS (current)
	/* 32 */ u16 unk_32;
	/* 34 */ u8 pad_34[0x36 - 0x34];
	/* 36 */ u8 scroll_val; // each px to scroll at each frame
	/* 37 */ u8 unk_37;
	/* 38 */ u8 pad_38[0x3C - 0x38];
	/* 3C */ u16 unk_3C;
};


struct PrepScreenUnitList {
	struct Unit* units[0x40];
	s32 size;
	u32 char_id;
};




// Text
#define gPrepTexts gUnknown_0201117C
extern struct TextHandle gPrepTexts[]; // 0201117C


// Prep Unit stack
void InitUnitStack(void* dest);
void PushUnit(struct Unit*);
void LoadPlayerUnitsFromUnitStack();

// Prep Unit List
extern struct PrepScreenUnitList gPrepScreenUnitList; // 0x20121CC
int GetChapterAllyUnitCount();
void SetPrepScreenUnitListCharID(int index); // 80953C0
struct Unit* GetPrepScreenUnitListEntry(int num);
void ReorderPlayerUnitsBasedOnDeployment();
extern int GetPrepScreenUnitListSize();
extern void ShrinkPlayerUnitList();

// SallyCursor
extern const struct ProcCmd gProc_PrepScreen[];

// AtMenu
void StartPrepScreenItemsMenu(struct Proc_AtMenu* proc);
void StartPrepScreenSupportMenu(int, struct Proc_AtMenu* proc);
void StartPrepScreenSaveMenu(struct Proc_AtMenu* proc);


// Unit menu
extern struct ProcCmd gProc_PrepScreenPickUnitsMenu[];


// prep menu
extern const struct ProcCmd gProc_PrepScreenMenu[]; // 8A186EC
extern const struct ProcCmd gProc_PrepScreenMenuDummyItem[]; // 8A186DC

void StartPrepScreenMenu(ProcPtr);
extern void ClosePrepScreenMapMenu(void);
void EndPrepScreenMenu(void);
void SetPrepScreenMenuItem(int index, void* effect_routine, int color, int msg, int msg_rtext); // 8097024


// Prep-main-menu effect routine
void PrepScreenMenu_OnPickUnits(struct Proc_AtMenu*);
void PrepScreenMenu_OnItems(struct Proc_AtMenu*);
void PrepScreenMenu_OnCheckMap(struct Proc_AtMenu*);
void PrepScreenMenu_OnSave(struct Proc_AtMenu*);
int PrepScreenMenu_OnBPress(struct Proc_AtMenu*);
int PrepScreenMenu_OnStartPress(struct Proc_AtMenu*);

void PrepScreenMapMenu_OnViewMap(struct Proc_SALLYCURSOR*);
void PrepScreenMapMenu_OnFormation(struct Proc_SALLYCURSOR*);
void PrepScreenMapMenu_OnOptions(struct Proc_SALLYCURSOR*);
void PrepScreenMapMenu_OnSave(struct Proc_SALLYCURSOR*);
int PrepScreenMapMenu_OnBPress(struct Proc_SALLYCURSOR*);
int PrepScreenMapMenu_OnStartPress(struct Proc_SALLYCURSOR*);
void PrepScreenMapMenu_OnEnd(struct Proc_SALLYCURSOR*);



// others
void EndPrepScreen();







