#ifndef __GUI_H
#define __GUI_H	
#include "lcd.h"


typedef struct _tagCONTROL
{
    uint8_t ID;
    uint8_t ParentID;
	uint16_t X;
	uint16_t Y;
	uint16_t Width;
	uint16_t Height;
	uint32_t Offset;
    uint16_t* BackGroundImage;
    uint16_t BackGroundColor;
    uint16_t ForeColor;
    uint8_t FontSize;
    uint16_t TextLength;
    uint16_t* Text;
}EV_CONTROL;

typedef struct _tagCONTROLLIST
{
    EV_CONTROL**    List;
    uint8_t        Count;
}EV_CONTROLLIST;

typedef struct _tagPAGE
{
    uint8_t ID;
    EV_CONTROLLIST* Controls;
	uint16_t Width;
	uint16_t Height;
	uint32_t Offset;
    uint32_t Size;
    uint16_t* BackGroundImage;
    void (*Callback)();
}EV_PAGE;

typedef struct _tagPAGELIST
{
    EV_PAGE**   List;
    uint8_t     Count;
}EV_PAGELIST;


extern EV_PAGELIST EV_Pages;
extern EV_PAGE* EV_APage;
extern int32_t tp_x, tp_y;

extern uint16_t* OpenImage(char * file_path);
extern EV_PAGE* GetPage(uint8_t id);
extern void AddControl(EV_PAGE* page, EV_CONTROL* ctrl);
extern void AddPAGE(EV_PAGE* page);
extern EV_PAGE* CreatePage(uint16_t width, uint16_t height, uint32_t offset, uint16_t* pdata, void (*_callback)());
extern EV_PAGE* CreatePageFormPath(uint16_t width, uint16_t height, uint32_t offset, char * path, void (*_callback)());
extern EV_CONTROL* CreateControl(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t offset, EV_PAGE* parent, uint16_t* pdata, uint16_t *ptext, uint16_t plength, uint8_t fontsize, uint16_t fontcolor);

extern void Gui_Init();
extern void DrawControl_BACK(EV_CONTROL* ctrl);
extern void DrawControl_FORE(EV_CONTROL* ctrl);
extern void DrawPage_BACK(EV_PAGE* page);
extern void DrawPage_FORE(EV_PAGE* page);

extern void ShowPage(EV_PAGE* page);

extern uint8_t ContainsPoint(EV_CONTROL* ctrl, uint32_t x, uint32_t y);
extern void GuiTouchExec();
#endif