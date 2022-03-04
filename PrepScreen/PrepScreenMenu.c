
#include "prep-screen.h"

extern void ResetPrepScreenHandCursor(ProcPtr);
extern void sub_80AD4A0(int, int);
extern void sub_80AD51C(int, int, int, int);

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


void StartPrepScreenMenu(ProcPtr proc){
	
	Proc_End( Proc_Find(gProc_PrepScreenMenu) );
	Proc_Start(gProc_PrepScreenMenu, proc);
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





// 
// Proc routine

// 8096CCC
void PrepScreenMenu_OnInit(struct Proc_PrepMainMenu* proc) {
	
	for( int i = 0; i < 8; i++)
		proc->cmds[i] = NULL;
	
	proc->cur_index = 0;
	proc->max_index = 0;
	
	ResetPrepScreenHandCursor(proc);
	sub_80AD4A0(0x600, 1); // PrepScreen_DrawHandGfxMaybe
	
	proc->on_bPress = NULL;
	proc->on_startPress = NULL;
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
		
		if( NULL == proc->on_bPress )
			return;
		
		if( 0 == proc->on_bPress(proc->proc_parent) )
			goto goto_on_failure;
			
		Proc_Goto(proc, 0);
		if( 0 == gRAMChapterData.unk41_8)
			m4aSongNumStart(0x6B);
		
		return;
	}// B_BUTTON
	
	
	if( START_BUTTON & gKeyStatusPtr->newKeys ){
		
		if( NULL == proc->on_startPress )
			return;
		
		if( 0 == proc->on_startPress(proc->proc_parent) )
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
