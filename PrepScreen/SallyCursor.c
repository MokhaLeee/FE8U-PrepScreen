
#include "prep-screen.h"

extern void DeletePlayerPhaseInterface6Cs();
extern int sub_8095970();
extern int CanPrepScreenSave();
extern void sub_8033620(struct Proc_SALLYCURSOR*);
extern void SetPrepScreenMenuOnBPress(void*);
extern void SetPrepScreenMenuOnStartPress(void*);
extern void SetPrepScreenMenuOnEnd(void*);
extern void DrawPrepScreenMenuFrameAt(int, int);
extern void SetPrepScreenMenuSelectedItem(int);

void PrepScreenProc_StartMapMenu(struct Proc_SALLYCURSOR*);


// =======================================================
// ================== Prep-map-menu ======================
// =======================================================

void PrepScreenProc_InitMapMenu(struct Proc_SALLYCURSOR* proc){
	
	proc->unk_58 = 1;
	PrepScreenProc_StartMapMenu(proc);
}



// 8033648
void PrepScreenProc_StartMapMenu(struct Proc_SALLYCURSOR* proc_prep){
	
	LoadDialogueBoxGfx(0, -1);
	Font_InitForUIDefault();
	DeletePlayerPhaseInterface6Cs();
	HideMoveRangeGraphics();
	
	// <!>
	StartPrepScreenMenu(proc_prep);
	
	SetPrepScreenMenuItem(
		1,
		PrepScreenMapMenu_OnViewMap,
		TEXT_COLOR_NORMAL,
		0x590, // View Map[X]
		0x5BB  // View maps to battle on or[.][NL]
	);
	
	SetPrepScreenMenuItem(
		2,
		PrepScreenMapMenu_OnFormation,
		0 == sub_8095970()
			? TEXT_COLOR_GRAY
			: TEXT_COLOR_NORMAL,
		0x591, // Formation[.][X]
		0x5BC  // Rearrange the starting[NL] formation of your units.[X]
	);

	SetPrepScreenMenuItem(
		8,
		PrepScreenMapMenu_OnOptions,
		TEXT_COLOR_NORMAL,
		0x592, // Options[.][X]
		0x5BD  // et game speed and[NL] window options.[.][X]
	);

	SetPrepScreenMenuItem(
		9,
		PrepScreenMapMenu_OnSave,
		0 == CanPrepScreenSave()
			? TEXT_COLOR_GRAY
			: TEXT_COLOR_NORMAL,
		0x579, // Save[X]
		0x5BE  // Save the current campaign.[X]
	);
	
	sub_8033620(proc_prep);
	
	SetPrepScreenMenuOnBPress(PrepScreenMapMenu_OnBPress);
	SetPrepScreenMenuOnStartPress(PrepScreenMapMenu_OnStartPress);
	SetPrepScreenMenuOnEnd(PrepScreenMapMenu_OnEnd);
	
	// <!>
	DrawPrepScreenMenuFrameAt(0xA, 0x2);
	
	SetPrepScreenMenuSelectedItem(proc_prep->unk_58);
	BG_EnableSyncByMask(0b11);
}