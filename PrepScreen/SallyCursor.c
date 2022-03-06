
#include "prep-screen.h"

extern struct Struct0202BCB0 gGameState;

extern void DeletePlayerPhaseInterface6Cs();
extern int sub_8095970();
extern int CanPrepScreenSave();
extern void sub_8033620(struct Proc_SALLYCURSOR*);
extern void SetPrepScreenMenuOnBPress(void*);
extern void SetPrepScreenMenuOnStartPress(void*);
extern void SetPrepScreenMenuOnEnd(void*);
extern void DrawPrepScreenMenuFrameAt(int, int);
extern void SetPrepScreenMenuSelectedItem(int);
static int (*InitChapterDeployedUnitAndGetCount)(void) = (const void*) 0x8095971;

void PrepScreenProc_StartMapMenu(struct Proc_SALLYCURSOR*);



void InitPrepScreenCursorPosition(struct Proc_SALLYCURSOR* proc_prep){
	
	struct Unit* unit;
	u8 x, y;
	
	unit = GetUnitFromCharId( GetPlayerLeaderUnitId() );
	
	if( (NULL != unit) && (InitChapterDeployedUnitAndGetCount() != 0) )
		SetCursorMapPosition(unit->xPos, unit->yPos);
	
	else
	{
		GetPreferredPositionForUNIT(
			CountForceDeployedUnits() + GetChapterAllyUnitDataPointer(),
			&x, &y, 0);
		
		SetCursorMapPosition(x,y);
	}
	
	gGameState.camera.x = sub_8015A40(gGameState.playerCursor.x * 16);
	gGameState.camera.y = sub_8015A6C(gGameState.playerCursor.y * 16);
}



// =======================================================
// ================== Prep-map-menu ======================
// =======================================================

void PrepScreenProc_InitMapMenu(struct Proc_SALLYCURSOR* proc){
	
	proc->status = 1;
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
	
	SetPrepScreenMenuSelectedItem(proc_prep->status);
	BG_EnableSyncByMask(0b11);
}






// On End
void EndPrepScreen(){
	
	for( int i = 1; i < FACTION_GREEN; i++ )
	{
		struct Unit* unit = GetUnit(i);
		
		if( !UNIT_IS_VALID(unit) )
			continue;
		
		unit->state = unit->state &~ (US_UNSELECTABLE);
		
		// here are Some Useless BWL operations
		
	} // for unit index
	
	ShrinkPlayerUnitList();
	Proc_EndEach(gProc_PrepScreen);
	
	gGameState.gameStateBits &= 0b11101111;
	gRAMChapterData.chapterStateBits &= 0b11101111;
	gRAMChapterData.unk4A_1 = 1; // nextTutorialEventParams
}





// View Map: Label 9

void PrepScreenMapMenu_OnViewMap(struct Proc_SALLYCURSOR* proc_prep){
	
	proc_prep->status = 1;
	
	Proc_Break(proc_prep);
	
	ClosePrepScreenMapMenu();
	
}


// 8033978
void _PrepScreenProc_MapIdle(struct Proc_SALLYCURSOR* proc_prep){
	
	return;
	
}


