#include "prep-screen.h"

extern void EndBG3Slider_();
extern void Get6CDifferedLoop6C(void* func, ProcPtr);
extern struct Unit* sub_8095394(); // maybe get leader?
extern int sub_80958FC(struct Unit*); // Get index in Prep-Unit-List
static void (*SetPrepScreenUnitListCharID)(int index) = (const void*) 0x80953C1;

// OAM Misc
static void (*PrepDrawHand)(int x, int y, int, int) = (const void*) 0x80AD51D;
extern void ResetPrepScreenHandCursor(ProcPtr);
static void (*PrepScreen_DrawHandGfxMaybe)(int, int) = (const void*) 0x80AD4A1;
extern void sub_80AD4E4(int); // also update hand


// BG Scroll
static void (*PrepStartScroll)(ProcPtr, int, int, int, int) = (const void*) 0x8097749;
extern void RestartScreenMenuScrollingBg();
static void (*PrepMenu_UpdateTsaScroll)(int) = (const void*) 0x809A645;
// static void (*HandleMenuScroll)(int,int,int,int) = (const void*) 0x80976CD;

// On Draw
// static void (*PrepUnit_InitTexts)(void) = (const void*) 0x809A815;
// static void (*PrepUnit_InitGfx)(void) = (const void*) 0x809A875;
static void (*PrepUnit_InitSMS)(struct Proc_PrepUnit*) = (const void*) 0x809A8F9;
// static void (*PrepUnit_DrawUnitItems)(struct Unit*) = (const void*) 0x809A9F9;
static void (*PrepUnit_DrawLeftUnitName)(struct Unit*) = (const void*) 0x809A931;
static void (*PrepUnit_DrawUnitListNames)(struct Proc_PrepUnit*, int) = (const void*) 0x809A581;
static void (*PrepUnit_DrawText_Pick_Left)(struct Proc_PrepUnit*,int) = (const void*) 0x809AAF1;
// static void (*Prep_DrawChapterGoal)(int, int) = (const void*) 0x8095A45;

void PrepUnit_InitTexts();
void PrepUnit_InitGfx();
void PrepUnit_DrawUnitItems(struct Unit*);
void Prep_DrawChapterGoal(int VRAM_offset, int index);
void PrepUnit_DrawSMSsAndObjs(struct Proc_PrepUnit*);



// 809B40C
void ProcPrepUnit_OnEnd(struct Proc_PrepUnit* proc){
	
	struct Proc_AtMenu* parent;
	
	parent = (struct Proc_AtMenu*)proc->proc_parent;
	
	parent->yDiff = proc->yDiff_cur;
	parent->cur_counter = proc->cur_counter;
	
	SetPrepScreenUnitListCharID(
		GetPrepScreenUnitListEntry(proc->list_num_cur)->pCharacterData->number);
	
	EndBG3Slider_();
}


// 809AE3C
void ProcPrepUnit_OnInit(struct Proc_PrepUnit* proc){
	
	struct Proc_AtMenu* parent;
	
	proc->list_num_cur = sub_80958FC( sub_8095394() );
	
	parent = (struct Proc_AtMenu*)proc->proc_parent;
	
	proc->max_counter = parent->max_counter;
	proc->cur_counter = parent->cur_counter;
	proc->yDiff_cur = parent->yDiff;
 	proc->list_num_pre = proc->list_num_cur;
	proc->button_blank = 0;
}


// 809AE7C
void ProcPrepUnit_InitScreen(struct Proc_PrepUnit* proc){

	extern void sub_809ADC8(struct Proc_PrepUnit*);
	
	extern void sub_809A874();
	// extern void sub_809A66C(struct Proc_PrepUnit*);
	extern void sub_80976CC(int, int, u16, int);
	extern void sub_80894E0();
	
	u16 BgConfig[12] = // gUnknown_08A181E8
	{
		// tile offset	map offset	screen size
		0x0000,			0xE000,		0,			// BG 0
		0x0000,			0xE800,		0,			// BG 1
		0x0000,			0xF000,		0,			// BG 2
		0x8000,			0xF800,		0,			// BG 3
	};
	
	SetupBackgrounds(BgConfig);
	
	gLCDControlBuffer.dispcnt.bg0_on = 0;
	gLCDControlBuffer.dispcnt.bg1_on = 0;
	gLCDControlBuffer.dispcnt.bg2_on = 0;
	gLCDControlBuffer.dispcnt.bg3_on = 0;
	gLCDControlBuffer.dispcnt.obj_on = 0;
	
	// Set proc+0x30? a unit index in list?
	sub_809ADC8(proc); 
	
	BG_Fill(gBG0TilemapBuffer, 0);
	BG_Fill(gBG1TilemapBuffer, 0);
	BG_Fill(gBG2TilemapBuffer, 0);
	
	gLCDControlBuffer.bg0cnt.priority = 0b10;
	gLCDControlBuffer.bg1cnt.priority = 0b10;
	gLCDControlBuffer.bg2cnt.priority = 0b01;
	gLCDControlBuffer.bg3cnt.priority = 0b11;
	
	BG_SetPosition( 0, 0, 0);
	BG_SetPosition( 1, 0, 0);
	BG_SetPosition( 2, 0, proc->yDiff_cur - 0x18);
	BG_SetPosition( 3, 0, 0);
	
	// <!> Init TextHandle
	PrepUnit_InitTexts();
	
	// <!> Init BG Gfx
	PrepUnit_InitGfx();
	
	BG_EnableSyncByMask(0b1111);
	SetDefaultColorEffects();
	
	// <!> Init SMS
	PrepUnit_InitSMS(proc);
	
	// <!> Also set SMS
	Get6CDifferedLoop6C(PrepUnit_DrawSMSsAndObjs, proc);
	
	// Hand
	ResetPrepScreenHandCursor(proc);
	PrepScreen_DrawHandGfxMaybe(0x600, 0x1);
	PrepDrawHand(
		(proc->list_num_cur % 2) * 56 + 0x70,
		(proc->list_num_cur / 2) * 16 - proc->yDiff_cur + 0x18,
		0x7, 0x800);
		
	PrepStartScroll(proc, 0xE0, 0x20, 0x200, 2);
	
	// Menu scroll bar (right var)
	sub_80976CC(0xA, proc->yDiff_cur,0,6);
	
	// OAM: [R]:info
	StartHelpPromptSprite(0x20, 0x8F, 9, (ProcPtr)proc);
	
	// Draw Texts
	PrepUnit_DrawUnitItems( GetPrepScreenUnitListEntry(proc->list_num_cur) );
	PrepUnit_DrawLeftUnitName( GetPrepScreenUnitListEntry(proc->list_num_cur) );
	
	for( int i = 0; i < 6; i++)
		PrepUnit_DrawUnitListNames(proc, proc->yDiff_cur / 16 + i);
	
	PrepUnit_DrawText_Pick_Left(proc, 0);
	NewGreenTextColorManager((ProcPtr)proc);
	LoadDialogueBoxGfx(BG_SCREEN_ADDR(0x29), 5);
	RestartScreenMenuScrollingBg();
}



// On Draw

// 0x809A815;
void PrepUnit_InitTexts(){
	
	// gPrepUnitTexts = 2013598
	
	Font_InitForUIDefault();
	
	// 0x0 ~ 0xD: (total 14) unit name
	for( int i = 0; i < 0xE; i++ )
		Text_Init(&gPrepUnitTexts[i], 5);
	
	// 0xE ~ 0x12: (total 5) item name
	for( int i = 0; i < 0x5; i++ )
		Text_Init(&gPrepUnitTexts[i + 0xE], 7);
	
	Text_Init(&gPrepUnitTexts[0x13], 7);
	Text_Init(&gPrepUnitTexts[0x14], 10);
	Text_Init(&gPrepUnitTexts[0x15], 12);
}



// 0x809A874
void PrepUnit_InitGfx(){
	
	static void (*DrawPrepWindowGfx2)(int, int)= (const void*) 0x80950E9;
	extern u16 gUnknown_08A1B730[]; // gfx
	extern u16 gUnknown_08A1B7C8[]; // tsa
	extern u16 gUnknown_08A1D510[]; // gGfx_PrepButton
	
	ResetIconGraphics_();
	LoadUiFrameGraphics();
	LoadObjUIGfx();
	
	LoadIconPalettes(4); // item icon
	
	Prep_DrawChapterGoal(0x6000, 8);
	DrawPrepWindowGfx2(0x6000, 0xF);
	
	CopyDataWithPossibleUncomp(gUnknown_08A1B730, (void*)0x06000440);
	CopyDataWithPossibleUncomp(gUnknown_08A1B7C8, gGenericBuffer);
	CallARM_FillTileRect(gBG1TilemapBuffer, gGenericBuffer, 0x1000);
	
	CopyDataWithPossibleUncomp(gUnknown_08A1D510, (void*)0x6010800);
	CopyToPaletteBuffer(gUnknown_08A1D79C, 0x320, 0x20);
	EnablePaletteSync();
}


// 0x809A9F9
void PrepUnit_DrawUnitItems(struct Unit* unit){
	
	int item_count, item;
	
	ResetIconGraphics_();
	
	// 清空原有的道具列表
	TileMap_FillRect(
		TILEMAP_LOCATED( gBG0TilemapBuffer, 1, 5),
		0xB, 0xA, 0);
		
	item_count = GetUnitItemCount(unit);
	
	if( item_count < 1 )
		goto goto_return;
	
	for( int i = 0; i < item_count; i++ )
	{
		item = unit->items[i];
		
		DrawIcon(
			TILEMAP_LOCATED( gBG0TilemapBuffer, 1, 5 + 2 * i),
			GetItemIconId(item),
			TILEREF(0, STATSCREEN_BGPAL_ITEMICONS) );
		
		Text_Clear( &gPrepUnitTexts[i + 0xE] );
		
		DrawTextInline(
			&gPrepUnitTexts[i + 0xE],
			TILEMAP_LOCATED( gBG0TilemapBuffer, 3, 5 + 2 * i),
			IsItemDisplayUsable(unit, item)
				? TEXT_COLOR_NORMAL
				: TEXT_COLOR_GRAY,
			0, 0, GetItemName(item) );
		
		DrawDecNumber(
			TILEMAP_LOCATED( gBG0TilemapBuffer, 11, 5 + 2 * i),
			IsItemDisplayUsable(unit, item)
				? TEXT_COLOR_BLUE
				: TEXT_COLOR_GRAY,
			GetItemUses(item) );
		
	} // for
	
	
goto_return:
	BG_EnableSyncByMask(0b01);
	return;

}



// 8095A44
void Prep_DrawChapterGoal(int VRAM_offset, int pal_index){
	
	extern u16 gPal_UIFont[]; // gUnknown_0859EF00
	
	struct Font tempFont;
	struct TextHandle tempTextHandle;
	int msg;
	char* string;
	
	InitSomeOtherGraphicsRelatedStruct(&tempFont, OBJ_VRAM0 + VRAM_offset, pal_index); // Font_InitForObj
	CopyToPaletteBuffer(gPal_UIFont, (0x10 + pal_index) * 32, 0x20);
	
	Text_Init3( &tempTextHandle );
	SetFont( &tempFont );
	SetFontGlyphSet(0);
	
	Text_80046B4( &tempTextHandle, 0); // Text_Fill2DLine
	
	if( 2 == GetChapterThing() )
		msg = 0x19E;
	else
		msg = GetROMChapterStruct(gRAMChapterData.chapterIndex)->goalWindowTextId;
	
	string = GetStringFromIndex(msg);
	
	Text_InsertString(
		&tempTextHandle,
		GetStringTextCenteredPos(0x60, string),
		0, string);
	
	SetFont(0);
}




// 809A66C
void PrepUnit_DrawSMSsAndObjs(struct Proc_PrepUnit* proc){
	
	static void (*DrawUnitSMS)(int root_node, int xPos, int yPos, struct Unit* unit) = (const void*) 0x8027B61;
	extern const u16 gUnknown_08A18E34[]; // gfx
	extern const u16 gUnknown_08A18E4E[]; // gfx
	
	for( int i = 0; i < GetPrepScreenUnitListSize(); i++ )
	{
		int yoff = (i >> 1) << 4;
		yoff -= proc->yDiff_cur;
		
		if( yoff < 0 )
			continue;
		
		if( (yoff + 0xF) > 0x5F )
			continue;
		
		DrawUnitSMS(
			0, 
			(i & 1)* 56 + 0x70,
			yoff + 0x18,
			GetPrepScreenUnitListEntry(i) );
		
	} // for
	
	if( 0 == (proc->yDiff_cur & 0xF) )
	{
		gLCDControlBuffer.dispcnt.win0_on = 0;
		gLCDControlBuffer.dispcnt.win1_on = 0;
		gLCDControlBuffer.dispcnt.objWin_on = 0;
	}
	else
	{
		
		gLCDControlBuffer.dispcnt.win0_on = 1;
		gLCDControlBuffer.dispcnt.win1_on = 1;
		gLCDControlBuffer.dispcnt.objWin_on = 1;
		
		gLCDControlBuffer.win0_left = 0;
		gLCDControlBuffer.win0_top = 0;
		gLCDControlBuffer.win0_right = 0xF0;
		gLCDControlBuffer.win0_bottom = 0x1A;
		
		gLCDControlBuffer.win1_left = 0;
		gLCDControlBuffer.win1_top = 0x78;
		gLCDControlBuffer.win1_right = 0xF0;
		gLCDControlBuffer.win1_bottom = 0xA0;
		
		gLCDControlBuffer.wincnt.win0_enableBg0 = 1;
		gLCDControlBuffer.wincnt.win0_enableBg1 = 1;
		gLCDControlBuffer.wincnt.win0_enableBg2 = 0;
		gLCDControlBuffer.wincnt.win0_enableBg3 = 1;
		gLCDControlBuffer.wincnt.win0_enableObj = 0;
		
		gLCDControlBuffer.wincnt.win1_enableBg0 = 1;
		gLCDControlBuffer.wincnt.win1_enableBg1 = 1;
		gLCDControlBuffer.wincnt.win1_enableBg2 = 0;
		gLCDControlBuffer.wincnt.win1_enableBg3 = 1;
		gLCDControlBuffer.wincnt.win1_enableObj = 1;
		
		gLCDControlBuffer.wincnt.wout_enableBg0 = 1;
		gLCDControlBuffer.wincnt.wout_enableBg1 = 1;
		gLCDControlBuffer.wincnt.wout_enableBg2 = 1;
		gLCDControlBuffer.wincnt.wout_enableBg3 = 1;
		gLCDControlBuffer.wincnt.wout_enableObj = 1;
		
	}
	
	if( proc->button_blank != 0 )
		proc->button_blank++;
	
	
	// Insert Chapter goal text
	for( int i = 0; i < 3; i++ )
		PutSpriteExt(4, 4 + i * 0x20, 0x81, gObject_32x16, 0x8700+ 4 * i);
	
	
	// "Start" button and control blank
	if( (0 == (proc->button_blank & 0b100)) && (0 != proc->cur_counter) )
		PutSpriteExt(4, 0x80, 0x82, gUnknown_08A18E4E, 0x40);
	
	// "Select" button
	PutSpriteExt(4, 0x80, 0x8F, gUnknown_08A18E34, 0x40);
	
	SMS_FlushDirect();
}











// 0x809B038
void ProcPrepUnit_Idle(struct Proc_PrepUnit* proc){
	
	extern int sub_809ACB4(struct Proc_PrepUnit* proc);
	extern void sub_809A9E8(struct Proc_PrepUnit*);
	extern void sub_80ACE20(void*, int, ProcPtr);
	extern int sub_809AD90(struct Proc_PrepUnit*);
	extern void sub_809AE10(struct Proc_PrepUnit*);
	
	const u16 key_pre = gKeyStatusPtr->repeatedKeys;
	
	if( proc->list_num_cur != proc->list_num_pre )
		goto goto_MenuScroll;
	
	proc->scroll_val = 4;
	
	// If L pressed, menu scrolls faster (well...)
	if( L_BUTTON & gKeyStatusPtr->heldKeys )
		proc->scroll_val = 8;
	
	
	if( START_BUTTON & gKeyStatusPtr->newKeys )
	{
		if( 0 == proc->cur_counter ){
			if( 0 == gRAMChapterData.unk41_8)
				m4aSongNumStart(0x6C);
			return;
		}
		else{
			if( 0 == gRAMChapterData.unk41_8)
				m4aSongNumStart(0x6A);
			Proc_Goto(proc, 0x63);
			return;
		}
	} // START_BUTTON
	
	else if( SELECT_BUTTON & gKeyStatusPtr->newKeys )
	{
		if( 0 == gRAMChapterData.unk41_8)
			m4aSongNumStart(0x6A);
		
		Proc_Goto(proc, 0x3);
		return;
	} // SELECT_BUTTON
	
	else if( R_BUTTON & gKeyStatusPtr->newKeys )
	{
		Proc_Goto(proc, 0x4);
		return;
	} // R_BUTTON
	
	else if( A_BUTTON & gKeyStatusPtr->newKeys )
	{
		if( 0 == sub_809ACB4(proc) )
			return;
		
		PrepUnit_DrawText_Pick_Left(proc, 1);
		return;
	} // A_BUTTON
	
	else if( B_BUTTON & gKeyStatusPtr->newKeys )
	{
		if( 0 == gRAMChapterData.unk41_8)
			m4aSongNumStart(0x6B);
		
		Proc_Goto(proc, 0xA);
		return;
	} // B_BUTTON


	// DPAD
	if( DPAD_LEFT & key_pre )
		if( 1 & proc->list_num_cur )
			proc->list_num_cur--;
	
	if( DPAD_RIGHT & key_pre )
		if( 0 == (1 & proc->list_num_cur) )
			if( (proc->list_num_cur + 1) < GetPrepScreenUnitListSize() )
				proc->list_num_cur++;
			
	if( DPAD_UP & key_pre )
		if( (proc->list_num_cur - 2) >= 0 )
			proc->list_num_cur -= 2;
	
	if( DPAD_DOWN & key_pre )
		if( (proc->list_num_cur + 2) < GetPrepScreenUnitListSize() )
			proc->list_num_cur += 2;
	
	if( proc->list_num_cur == proc->list_num_pre )
		return;
	
	
	// New Draw items
	PrepUnit_DrawUnitItems(GetPrepScreenUnitListEntry(proc->list_num_cur));
	
	// Draw Left Text
	sub_80ACE20(sub_809A9E8, 1, proc);
	
	if( 0 == gRAMChapterData.unk41_8)
		m4aSongNumStart(0x65);
	
	// 
	if( 0 == sub_809AD90(proc) )
	{
		proc->list_num_pre = proc->list_num_cur;
		
		PrepDrawHand(
			(proc->list_num_pre % 2) * 56 + 0x70,
			(proc->list_num_cur / 2) * 16 - proc->yDiff_cur + 0x18,
			0x7, 0x800);
			
	}
	else
	{
		if( proc->list_num_cur < proc->list_num_pre )
			PrepUnit_DrawUnitListNames(proc, proc->yDiff_cur / 16 - 1);
		
		if( proc->list_num_cur > proc->list_num_pre )
			PrepUnit_DrawUnitListNames(proc, proc->yDiff_cur / 16 + 6);
		
		sub_80AD4E4( (proc->list_num_pre % 2) * 56 + 0x70 );
	}
	
	if( proc->list_num_pre == proc->list_num_cur )
		return;
	
	goto goto_MenuScroll;
	
	
	
goto_MenuScroll:
	if( proc->list_num_cur < proc->list_num_pre )
		proc->yDiff_cur -= proc->scroll_val;
	
	if( proc->list_num_cur > proc->list_num_pre )
		proc->yDiff_cur += proc->scroll_val;
	
	if( 0 == (proc->yDiff_cur & 0xF) )
	{
		PrepMenu_UpdateTsaScroll( proc->yDiff_cur / 16 - 1 );
		PrepMenu_UpdateTsaScroll( proc->yDiff_cur / 16 + 6 );
		sub_809AE10(proc);
		proc->list_num_pre = proc->list_num_cur;
	}
	
	BG_SetPosition(2,0, proc->yDiff_cur - 0x18);
	
	return;
}


// Label 63
// 809B438
void ProcPrepUnit_OnGameStart(struct Proc_PrepUnit* proc){
	
	struct Proc_AtMenu* parent;
	
	parent = proc->proc_parent;
	parent->end_prep = 1;
	
	// Here goto AtMenu's label !!
	Proc_Goto(parent, 0x6); // AtMenu label 6: end
	
	proc->button_blank = 1;
}

























