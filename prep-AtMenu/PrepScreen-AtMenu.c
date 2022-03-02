#include "prep-screen.h"

int CheckSomethingSomewhere();

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
	
	void PrepScreenMenu_OnPickUnits(struct Proc_AtMenu*);
	void PrepScreenMenu_OnItems(struct Proc_AtMenu*);
	void PrepScreenMenu_OnCheckMap(struct Proc_AtMenu*);
	void PrepScreenMenu_OnSave(struct Proc_AtMenu*);
	void PrepScreenMenu_OnBPress(struct Proc_AtMenu*);
	void PrepScreenMenu_OnStartPress(struct Proc_AtMenu*);
	
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



// 8097024
void SetPrepScreenMenuItem(int index, void *effect, int color, int msg, int msg_rtext) {
	
	struct Proc_PrepMainMenu *menu;
	struct Proc_PrepMainMenuCmd *cmd;
	
	menu = (struct Proc_PrepMainMenu*) Proc_Find(gProc_PrepScreenMenu);
	
	if( NULL == menu )
		return;
	
	cmd = menu->cmds[0];
	
	for ( int i = 0; i < 8; i++){
		
		cmd = menu->cmds[i];
		
		if ( NULL == cmd )
			continue;
		
		if ( index == cmd->index )
		{
			cmd->effect = effect;
			cmd->color = color;
			cmd->msg = msg;
			cmd->msg_rtext = msg_rtext;
			return;
		}

	}
	
	cmd = (struct Proc_PrepMainMenuCmd*) Proc_Start(gProc_PrepScreenMenuDummyItem, menu);
	
	menu->cmds[menu->max_index] = cmd;
	cmd->index = index;
	cmd->effect = effect;
	cmd->color = color;
	cmd->msg = msg;
	cmd->msg_rtext = msg_rtext;
	
	Text_Init(&cmd->text, 7);
	menu->max_index++;
	
	
}



// =======================================================
// =============== Proc Prep Main Menu ===================
// =======================================================
void PrepScreenMenu_OnInit(struct Proc_PrepMainMenu*);
void PrepScreenMenu_OnEnd(struct Proc_PrepMainMenu*);
void PrepScreenMenu_OnLoop_0(struct Proc_PrepMainMenu*);
void PrepScreenMenu_OnLoop_2(struct Proc_PrepMainMenu*);
void PrepScreenMenu_OnActiveLoop(struct Proc_PrepMainMenu*);

// 8A186EC
const struct ProcCmd _gProc_PrepScreenMenu[] = {
	
	PROC_CALL	(PrepScreenMenu_OnInit),
	PROC_SET_END_CB(PrepScreenMenu_OnEnd),
	PROC_YIELD,
	
PROC_LABEL(0),
	PROC_REPEAT	(PrepScreenMenu_OnLoop_0),

PROC_LABEL(1),
	PROC_REPEAT	(PrepScreenMenu_OnActiveLoop),

PROC_LABEL(2),
	PROC_REPEAT	(PrepScreenMenu_OnLoop_2),

PROC_LABEL(0xA),
	PROC_END,
};


void StartPrepScreenMenu(struct Proc_AtMenu* proc){
	
	Proc_End( Proc_Find(gProc_PrepScreenMenu) );
	Proc_Start(gProc_PrepScreenMenu, proc);
}



// 8096CCC
void PrepScreenMenu_OnInit(struct Proc_PrepMainMenu* proc) {
	
	for( int i = 0; i < 8; i++)
		proc->cmds[i] = NULL;
	
	proc->cur_index = 0;
	proc->max_index = 0;
	
	ResetPrepScreenHandCursor(proc);
	sub_80AD4A0(0x600, 1); // PrepScreen_DrawHandGfxMaybe
	
	proc->b_press = NULL;
	proc->start_press = NULL;
	proc->on_end = NULL;
	proc->rtext_on = 0;
}



void PrepScreenMenu_OnEnd(struct Proc_PrepMainMenu* proc) {
	
	if( NULL != proc->on_end )
		proc->on_end( proc->proc_parent );
	
}



void PrepScreenMenu_OnLoop_0(struct Proc_PrepMainMenu* proc) {
	
	// PrepScreenItemUseScreen_Init_ShowHand
	sub_80AD51C(
		(proc->xPos + 1) * 8 + 4,
		(proc->yPos + 1) * 8 + proc->cur_index * 16,
		6, 0x400);
}


void PrepScreenMenu_OnLoop_2(struct Proc_PrepMainMenu* proc){
	
	DisplayFrozenUiHand(
		(proc->xPos + 1) * 8 + 4,
		(proc->yPos + 1) * 8 + proc->cur_index * 16);
	
}




void PrepScreenMenu_OnActiveLoop(struct Proc_PrepMainMenu* proc) {
	
	int index, hand_xPos, hand_yPos;
	struct Proc_PrepMainMenuCmd* cmd;
	
	index = proc->cur_index;
	hand_xPos = (proc->xPos + 1) * 8 + 4;
	hand_yPos = (proc->yPos + 1) * 8 + proc->cur_index * 16;
	
	sub_80AD51C(hand_xPos, hand_yPos, 6, 0x400);
	
	cmd = proc->cmds[proc->cur_index];
	
	if( 0 != proc->rtext_on ){
		
		if( (R_BUTTON & gKeyStatusPtr->newKeys) || (B_BUTTON & gKeyStatusPtr->newKeys) ){
			CloseHelpBox();
			proc->rtext_on = 0;
			return;
		} // R_BUTTON |  B_BUTTON
		
		else
			goto goto_no_startpress;
		
	} // rtext_on
	
	if( R_BUTTON & gKeyStatusPtr->newKeys ){
		
		if( cmd->msg_rtext ){
			
			StartHelpBox(hand_xPos, hand_yPos, cmd->msg_rtext);
			proc->rtext_on = 1;
			
		} // if( cmd->msg_rtext )
		
		return;
		
	} // R_BUTTON
	
	
	if( A_BUTTON & gKeyStatusPtr->newKeys ){
		
		if( (TEXT_COLOR_GRAY & cmd->color) || (NULL == cmd->effect) )
			goto goto_on_failure;
		
		Proc_Goto(proc, 0);
		
		cmd->effect(proc->proc_parent);
		
		if( 0 == gRAMChapterData.unk41_8)
			m4aSongNumStart(0x6A);
		
		return;
		
	} // A_BUTTON
	
	if( B_BUTTON & gKeyStatusPtr->newKeys ){
		
		if( NULL == proc->b_press )
			return;
		
		if( 0 == proc->b_press(proc->proc_parent) )
			goto goto_on_failure;
			
		Proc_Goto(proc, 0);
		if( 0 == gRAMChapterData.unk41_8)
			m4aSongNumStart(0x6B);
		
		return;
	}// B_BUTTON
	
	
	if( START_BUTTON & gKeyStatusPtr->newKeys ){
		
		if( NULL == proc->start_press )
			return;
		
		if( 0 == proc->start_press(proc->proc_parent) )
			goto goto_on_failure;
			
		if( 0 == gRAMChapterData.unk41_8)
			m4aSongNumStart(0x6B);
		
		Proc_Goto(proc, 0);
		return;
		
	} // START_BUTTON
	
	goto goto_no_startpress;
	
	
goto_no_startpress:
	
	if( DPAD_UP & gKeyStatusPtr->repeatedKeys ){
		
		if( 0 != proc->cur_index )
			proc->cur_index--;
		
		else if( DPAD_UP & gKeyStatusPtr->newKeys )
			proc->cur_index = proc->max_index - 1;
		
	} // DPAD_UP
	
	if( DPAD_DOWN & gKeyStatusPtr->repeatedKeys ){
		
		if( proc->cur_index < (proc->max_index - 1) )
			proc->cur_index++;
		
		else if( DPAD_DOWN & gKeyStatusPtr->newKeys )
			proc->cur_index = 0;
		
	} // DPAD_DOWN
	
	if( index == proc->cur_index )
		return;
	
	if( 0 == gRAMChapterData.unk41_8)
		m4aSongNumStart(0x66);
	
	
	if( 0 != proc->rtext_on )
		StartHelpBox( 
			(proc->xPos + 1) * 8 + 4,
			(proc->yPos + 1) * 8 + proc->cur_index * 16,
			proc->cmds[proc->cur_index]->msg_rtext);
	
	return;


goto_on_failure:
	if( 0 == gRAMChapterData.unk41_8)
		m4aSongNumStart(0x6C);
	return;
}




















