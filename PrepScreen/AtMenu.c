#include "prep-screen.h"

extern int CheckSomethingSomewhere();
extern void InitPrepScreenUnitList();
void AtMenu_AutoCapDeployPrepScreenUnits(struct Proc_AtMenu*);
void AtMenu_InitPrepScreenMenu(struct Proc_AtMenu*);
extern void sub_8096BFC(ProcPtr);
extern void RestartScreenMenuScrollingBg();
extern void sub_8095A44(int, int);
extern void sub_80AD1D0(int);
extern int GetActivePrepScreenMenuItemId();
extern void DrawPrepScreenMenuHelpText(int);
extern int GetActivePrepScreenMenuItemHelpTextId();
extern void DisplayPrepScreenMenuHelpText();
extern void sub_80AD1AC(ProcPtr);
extern void AtMenu_AddPrepScreenSupportMenuItem();
extern int CanPrepScreenCheckMap();
extern int CanPrepScreenSave();
extern void SetPrepScreenMenuOnBPress(void*);
extern void SetPrepScreenMenuOnStartPress(void*);
extern void SetPrepScreenMenuPosition(int, int);
extern void SetPrepScreenMenuSelectedItem(int);
extern void sub_8096BE0();
extern void sub_8096BC4();
extern void EndBG3Slider_(ProcPtr);
extern void sub_8096C20();
extern void SetColorEffectsFirstTarget(int, int, int, int, int);
extern void SetColorEffectBackdropFirstTarget(int);
extern void PositionUnitsAccordingToDeployment();

extern u16 gGenericBuffer[]; // 2020188
extern u16 gBgConfig_8A181E8[];
extern struct TextHandle gPrepScreenMenuHelpTextArray[];









// 8096424
void AtMenu_LockGame(struct Proc_AtMenu* proc){
	
	// CheckGameLinkArenaBit
	if( 0 == CheckSomethingSomewhere() )
	{	
		AddSkipThread2();
		BMapDispSuspend();
	}
	
}

// 8095AD8
void AtMenu_OnInit(struct Proc_AtMenu* proc){
	
	SetPrepScreenUnitListCharID(0);
	
	proc->unk_40 = 0;
	proc->unk_3C = 0;
	
	if ( 0 == CheckSomethingSomewhere())
		proc->select_count_max = GetChapterAllyUnitCount();
	else
		proc->select_count_max = 5;
	
	proc->unk_30 = 0;
	proc->unk_31 = 0;
	proc->unk_32 = 0;
	proc->state = 0;
	proc->unk_34 = 0;
	proc->end_prep = 0;
	proc->unk_2D = 0;
	proc->unk_2E = 0;
}



// 80953C0
void SetPrepScreenUnitListCharID(int index){
	
	extern void* gGMData;
	void sub_80C409C(u32);
	
	if ( 0 != (1 & *(u8*)gGMData) )
		sub_80C409C(index);
	else
		gPrepScreenUnitList.char_id = index;
		
	
}




// 0x8095C84
void AtMenu_Reinitialize(struct Proc_AtMenu* proc) {
	
	extern u16 gUnknown_08A181E8[]; // BG Config
	extern u16 gUnknown_08A1A4C8[]; // maybe gfx
	extern u16 gUnknown_08A1D510[]; // maybe tsa
	extern u16 gPal_SmolBrownNameBox[]; // gPal_SmolBrownNameBox
	
	extern u16 gUnknown_08A1AC88[]; // should be tsa
	extern u16 gUnknown_08A1B174[]; // pal
	extern u16 gUnknown_08A1D4C8[]; // pal
	// extern u16 gUiFramePaletteB[];
	
	extern u16 gUnknown_08A1B658[]; // tsa
	extern u16 gUnknown_08A1B698[]; // tsa
	
	
	
	SetupBackgrounds(gUnknown_08A181E8);
	Font_InitForUIDefault();
	LoadUiFrameGraphics();
	
	LoadDialogueBoxGfx(0, 0xE);
	
	gLCDControlBuffer.dispcnt.bg0_on = 0;
	gLCDControlBuffer.dispcnt.bg1_on = 0;
	gLCDControlBuffer.dispcnt.bg2_on = 0;
	gLCDControlBuffer.dispcnt.bg3_on = 0;
	gLCDControlBuffer.dispcnt.obj_on = 0;
	
	LoadObjUIGfx();
	SMS_ClearUsageTable();
	InitPrepScreenUnitList();
	AtMenu_AutoCapDeployPrepScreenUnits(proc);
	ReorderPlayerUnitsBasedOnDeployment();
	
	BG_Fill(gBG0TilemapBuffer, 0);
	BG_Fill(gBG1TilemapBuffer, 0);
	BG_Fill(gBG2TilemapBuffer, 0);
	
	
	for( int i = 0; i < 5; i++)
		Text_Init(&gPrepTexts[i + 5], 0xE);
	
	for( int i = 0; i < 4; i++)
		Text_Init(&gPrepTexts[i + 1], 0x8);
	
	Text_Init(&gPrepTexts[0], 0xA);
	
	CopyDataWithPossibleUncomp(
		gUnknown_08A1A4C8,
		BG_SCREEN_ADDR(0x29) );
		
	CopyDataWithPossibleUncomp(
		gUnknown_08A1D510,
		BG_SCREEN_ADDR(0x2C) );
	
	CopyToPaletteBuffer(gPal_SmolBrownNameBox, 0x320, 0x40);
	
	// 8095C50
	CopyDataWithPossibleUncomp(
		gUnknown_08A1AC88,
		OBJ_VRAM0 + 0x7000 );
	
	CopyToPaletteBuffer( gUnknown_08A1B174, 0x2C0, 0x20);
	CopyToPaletteBuffer( gUnknown_08A1D4C8, 0x280, 0x20);
	EnablePaletteSync();
	
	gLCDControlBuffer.bg0cnt.priority = 0;
	gLCDControlBuffer.bg1cnt.priority = 0b10;
	gLCDControlBuffer.bg2cnt.priority = 0b01;
	gLCDControlBuffer.bg3cnt.priority = 0b11;
	gLCDControlBuffer.dispcnt.win0_on = 0;
	gLCDControlBuffer.dispcnt.win1_on = 0;
	gLCDControlBuffer.dispcnt.objWin_on = 0;
	
	BG_SetPosition( 0, 0, 0);
	BG_SetPosition( 1, 0, 0);
	BG_SetPosition( 2, 0, 0);
	
	// <!>
	AtMenu_InitPrepScreenMenu(proc);
	
	BG_EnableSyncByMask(0b1111);
	SetDefaultColorEffects();
	
	// Draw special character
	sub_8096BFC(proc);
	
	// BG scrolling
	RestartScreenMenuScrollingBg();
	
	CopyToPaletteBuffer( gUiFramePaletteB, 0x40, 0x60);
	
	// Main menu window
	CopyDataWithPossibleUncomp(
		gUnknown_08A1B658,
		gGenericBuffer );
	
	CallARM_FillTileRect(
		TILEMAP_LOCATED(gBG1TilemapBuffer, 0x10, 2),
		gGenericBuffer, 0x1000 );
		
	CopyDataWithPossibleUncomp(
		gUnknown_08A1B698,
		gGenericBuffer );
	
	CallARM_FillTileRect(
		TILEMAP_LOCATED(gBG1TilemapBuffer, 1, 6),
		gGenericBuffer, 0x1000 );
	
	
	// chapter name
	sub_8095A44(0x5800, 0xB);
	
	sub_80AD1AC(proc);
	sub_80AD1D0(0x6800);
	
	proc->unk_35 = GetActivePrepScreenMenuItemId();
	
	// Draw infomation text
	DrawPrepScreenMenuHelpText( GetActivePrepScreenMenuItemHelpTextId() );
	DisplayPrepScreenMenuHelpText();
}




// 8095524
void AtMenu_InitPrepScreenMenu(struct Proc_AtMenu* proc) {
	
	// <!>
	StartPrepScreenMenu(proc);
	
	// here neglect Arena case
	SetPrepScreenMenuItem(0, PrepScreenMenu_OnPickUnits, TEXT_COLOR_NORMAL, 0x574, 0);
	SetPrepScreenMenuItem(1, PrepScreenMenu_OnItems, TEXT_COLOR_NORMAL, 0x576, 0);
	AtMenu_AddPrepScreenSupportMenuItem(proc);
	
	SetPrepScreenMenuItem(
		7, PrepScreenMenu_OnCheckMap,
		0 == CanPrepScreenCheckMap()
			? TEXT_COLOR_GRAY
			: TEXT_COLOR_NORMAL,
		0x578, // Check Map[.][X]
		0
	);
	
	SetPrepScreenMenuItem(
		2, PrepScreenMenu_OnSave,
		0 == CanPrepScreenSave()
			? TEXT_COLOR_GRAY
			: TEXT_COLOR_NORMAL,
		0x579, // Save[X]
		0 
	);
	
	SetPrepScreenMenuOnBPress(PrepScreenMenu_OnBPress);
	SetPrepScreenMenuOnStartPress(PrepScreenMenu_OnStartPress);
	
	TileMap_FillRect(gBG0TilemapBuffer, 0xC, 0x13, 0);
	TileMap_FillRect(gBG1TilemapBuffer, 0xC, 0x13, 0);
	
	SetPrepScreenMenuPosition(1, 6);
	SetPrepScreenMenuSelectedItem(proc->unk_2D);
}



void AtMenu_AutoCapDeployPrepScreenUnits(struct Proc_AtMenu* proc){
	
	proc->select_count = 0;
	proc->unit_count = 0;
	
	for( int i = 0; i < GetChapterAllyUnitCount(); i++){
		
		struct Unit* unit = GetPrepScreenUnitListEntry(i);
		
		if( unit->state & US_NOT_DEPLOYED ){
			proc->unit_count++;
			continue;
		}
		
		if( proc->select_count_max < proc->select_count ){
			unit->state = US_NOT_DEPLOYED;
			proc->unit_count++;
			continue;
		}
		
		proc->select_count++;
		proc->unit_count++;
		
	} // for
	
	if( proc->select_count_max < proc->select_count )
		proc->select_count_max = proc->select_count;
		
}






// =======================================================
// ====================== On End =========================
// =======================================================
// Label 5 & 6

void AtMenu_OnEnd_8096294(struct Proc_AtMenu* proc){
	
	EndBG3Slider_(proc);
	sub_8096C20();
	SetupBackgrounds(0);
	SetSpecialColorEffectsParameters(3,0,0,0x10);
	SetColorEffectsFirstTarget(1,1,1,1,1);
	SetColorEffectBackdropFirstTarget(1);
}


void AtMenu_OnEnd_80962E0(struct Proc_AtMenu* proc){
	
	extern void sub_8042EA8();
	
	ReorderPlayerUnitsBasedOnDeployment();
	
	if( 1 == proc->end_prep )
		EndPrepScreen();
	else if( 0 != CheckSomethingSomewhere() )
		sub_8042EA8(); // maybe arena
	
	PositionUnitsAccordingToDeployment();
	SMS_ClearUsageTable();
	RefreshEntityBmMaps();
	SMS_UpdateFromGameData();
		
}

void AtMenu_OnEnd_809643C(struct Proc_AtMenu* proc){
	
	if( 0 != CheckSomethingSomewhere() )
		return;
	
	BMapDispResume();
	SubSkipThread2();
	
}

// =======================================================
// ================ Sub menu routine =====================
// =======================================================

void AtMenu_StartSubmenu(struct Proc_AtMenu* proc){
	
	extern void sub_8095C2C(ProcPtr);
	extern void sub_808E79C(ProcPtr);
	
	sub_8095C2C(proc); // end scroll and some procs, etc
	
	switch( proc->state )
	{
		case 5:
			sub_808E79C(proc);
			break;
			
		case 2: // Item
			StartPrepScreenItemsMenu(proc);
			break;
			
		case 1: // Pick Units
			Proc_StartBlocking(gProc_PrepScreenPickUnitsMenu, proc);
			break;
			
		case 4: // Support
			StartPrepScreenSupportMenu(2, proc);
			break;
		
		case 3: // Save
			StartPrepScreenSaveMenu(proc);
			break;
			
		default:
			break;
	}
	
	Proc_Break(proc);
	
}


void AtMenu_OnSubmenuEnd(struct Proc_AtMenu* proc){
	
	extern void StartSongVolumeTransitionB(int,int,int,int);
	
	if( 3 == proc->state )
		StartSongVolumeTransitionB(0x80, 0x100, 0x20, 0);
	
	switch( proc->state )
	{
		case 4:
			Proc_Goto(proc, 0x9);
			break;
			
		case 3: // Save
			Proc_Goto(proc, 0x7);
			break;
			
		case 1:
		case 2:
		case 5:
			Proc_Goto(proc, 0x9);
			break;
			
		default:
			break;
	}
	
	proc->state = 0;
	
}



// =======================================================
// ========== Prep-main-menu effect routine ==============
// =======================================================

void PrepScreenMenu_OnPickUnits(struct Proc_AtMenu* proc){
	
	proc->state = 1;
	Proc_Goto(proc, 0xA);
	
}

void PrepScreenMenu_OnItems(struct Proc_AtMenu* proc){
	
	proc->state = 2;
	Proc_Goto(proc, 0xA);
}

void PrepScreenMenu_OnCheckMap(struct Proc_AtMenu* proc){
	
	Proc_Goto(proc, 0x5);
	
}
void PrepScreenMenu_OnSave(struct Proc_AtMenu* proc){
	if( 0 == gRAMChapterData.unk41_8)
		m4aSongNumStart(0x6A);
	
	proc->state = 3;
	Proc_Goto(proc, 0x8);
}

int PrepScreenMenu_OnBPress(struct Proc_AtMenu* proc){
	
	// CheckGameLinkArenaBit
	if( 0 == CheckSomethingSomewhere() ){
		if( 0 == CanPrepScreenCheckMap() )
			return 0;
		else
			sub_8096BE0();
	}
	
	Proc_Goto(proc, 0x5);
	return 1;
}

int PrepScreenMenu_OnStartPress(struct Proc_AtMenu* proc){
	
	if( 0 == proc->select_count )
		return 0;
	
	sub_8096BC4();
	Proc_Goto(proc, 0xB);
	return 1;
	
}






