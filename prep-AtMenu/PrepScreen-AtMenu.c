#include "prep-screen.h"

int CheckSomethingSomewhere();

extern u16 gGenericBuffer[]; // 2020188
extern u16 gBgConfig_8A181E8[];

extern struct TextHandle gPrepScreenMenuHelpTextArray[];

// 8096424
void _AtMenu_LockGame(struct Proc_AtMenu* proc){
	
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
		proc->max_count = GetChapterAllyUnitCount();
	else
		proc->max_count = 5;
	
	proc->unk_30 = 0;
	proc->unk_31 = 0;
	proc->unk_32 = 0;
	proc->state = 0;
	proc->unk_34 = 0;
	proc->unk_36 = 0;
	proc->unk_2D = 0;
	proc->unk_2E = 0;
}



// 80953C0
void SetPrepScreenUnitListCharID(u32 index){
	
	extern void* gGMData;
	void sub_80C409C(u32);
	
	if ( 0 != (1 & *(u8*)gGMData) )
		sub_80C409C(index);
	else
		gPrepScreenUnitList.char_id = index;
		
	
}




// 0x8095C84
void _AtMenu_Reinitialize(struct Proc_AtMenu* proc) {
	
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
	
	void PrepScreenMenu_OnPickUnits(struct Proc_AtMenu*);
	void PrepScreenMenu_OnItems(struct Proc_AtMenu*);
	void PrepScreenMenu_OnCheckMap(struct Proc_AtMenu*);
	void PrepScreenMenu_OnSave(struct Proc_AtMenu*);
	void PrepScreenMenu_OnBPress(struct Proc_AtMenu*);
	void PrepScreenMenu_OnStartPress(struct Proc_AtMenu*);
	
	// <!>
	StartPrepScreenMenu();
	
	// here neglect Arena case
	SetPrepScreenMenuItem(0, PrepScreenMenu_OnPickUnits, TEXT_COLOR_NORMAL, 0x574, NULL);
	SetPrepScreenMenuItem(1, PrepScreenMenu_OnItems, TEXT_COLOR_NORMAL, 0x576, NULL);
	AtMenu_AddPrepScreenSupportMenuItem(proc);
	
	SetPrepScreenMenuItem(
		7, PrepScreenMenu_OnCheckMap,
		0 == CanPrepScreenCheckMap()
			? TEXT_COLOR_GRAY
			: TEXT_COLOR_NORMAL,
		0x578, // Check Map[.][X]
		NULL
	);
	
	SetPrepScreenMenuItem(
		2, PrepScreenMenu_OnSave,
		0 == CanPrepScreenSave()
			? TEXT_COLOR_GRAY
			: TEXT_COLOR_NORMAL,
		0x579, // Save[X]
		NULL 
	);
	
	SetPrepScreenMenuOnBPress(PrepScreenMenu_OnBPress);
	SetPrepScreenMenuOnStartPress(PrepScreenMenu_OnStartPress);
	
	TileMap_FillRect(gBG0TilemapBuffer, 0xC, 0x13, 0);
	TileMap_FillRect(gBG1TilemapBuffer, 0xC, 0x13, 0);
	
	SetPrepScreenMenuPosition(1, 6);
	SetPrepScreenMenuSelectedItem(proc->unk_2D);
}



// 8097024
void SetPrepScreenMenuItem(int index, void(*effect)(struct Proc_AtMenu*), int color, int msg, void* maybe_func) {
	
	struct Proc_PrepMainMenu *menu;
	struct Proc_PrepMainMenuCmd *cmd;
	
	menu = (struct Proc_PrepMainMenu*) Proc_Find(gProc_PrepScreenMenu);
	
	if( NULL == menu )
		return;
	
	cmd = menu->cmds[0];
	
	for ( int i = 0; i < 7; i++){
		
		cmd = menu->cmds[i];
		
		if ( NULL == cmd )
			continue;
		
		if ( index == cmd->index )
		{
			cmd->effect = effect;
			cmd->color = color;
			cmd->msg = msg;
			cmd->unk_func = maybe_func;
			return;
		}

	}
	
	cmd = (struct Proc_PrepMainMenuCmd*) Proc_Start(gProc_PrepScreenMenuDummyItem, menu);
	
	menu->cmds[menu->cur_index] = cmd;
	cmd->index = index;
	cmd->effect = effect;
	cmd->color = color;
	cmd->msg = msg;
	cmd->unk_func = maybe_func;
	
	Text_Init(&cmd->text, 7);
	menu->cur_index++;
	
	
}













