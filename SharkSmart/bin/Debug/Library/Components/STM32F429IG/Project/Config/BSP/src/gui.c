#include "gui.h"
#include "ff.h"
#include "log.h"
#include "touch.h"
#include "malloc.h"
#include "rtos.h"
#include "GLOBAL.h"

EV_PAGELIST EV_Pages;
EV_PAGE* EV_APage;

uint16_t* OpenImage(char * file_path)
{
    uint8_t		file_res;
	uint16_t*	file_buf;
    uint32_t 	cpu_sr;
    FIL			file_ld;
    UINT		file_bread;
	
	file_res = f_open(&file_ld, file_path, FA_READ);
	if(file_res != FR_OK)
	{
		return NULL;
	}
	file_buf = mymalloc(SRAMEX, file_ld.obj.objsize);
	if(file_buf == NULL)
	{
		f_close(&file_ld);
		return NULL;
	}

	OS_ENTER_CRITICAL();
	file_res = f_read(&file_ld, file_buf, file_ld.obj.objsize, &file_bread);
	OS_EXIT_CRITICAL();
	if(file_res != FR_OK)
	{
		f_close(&file_ld);
		myfree(SRAMEX, file_buf);
		return NULL;
	}
    LOG(LEVEL_WARNING, "OpenImage size:%d\n",file_bread);
	f_close(&file_ld);
	return file_buf;
}

void AddControl(EV_PAGE* page, EV_CONTROL* ctrl)
{
    page->Controls->List[page->Controls->Count] = ctrl;
    LOG(LEVEL_WARNING, "add - ptext:%x\n",page->Controls->List[page->Controls->Count]->Text[0]);
    page->Controls->Count++;
    //LOG(LEVEL_WARNING, "controllist count:%d\n",page->Controls->Count);
}

void AddPAGE(EV_PAGE* page)
{
    EV_Pages.List[EV_Pages.Count] = page;
    EV_Pages.Count++;
}

EV_PAGE* GetPage(uint8_t id)
{
    for(uint8_t i = 0; i < EV_Pages.Count; i++)
    {
        if(EV_Pages.List[i]->ID == id)
        {
            return EV_Pages.List[i];
        }
    }
    return (EV_PAGE*)NULL;
}

EV_PAGE* CreatePage(uint16_t width, uint16_t height, uint32_t offset, uint16_t* pdata, void (*_callback)())
{
    EV_PAGE* _page_private = (EV_PAGE *)mymalloc(SRAMEX, sizeof(EV_PAGE));
    _page_private->ID = EV_Pages.Count;
    _page_private->Width = width;
    _page_private->Height = height;
    _page_private->Offset = offset;
    EV_CONTROLLIST* ctrls = (EV_CONTROLLIST *)mymalloc(SRAMEX, sizeof(EV_CONTROLLIST));
    ctrls->List = mymalloc(SRAMEX, 128 * 4);
    ctrls->Count = 0;
    _page_private->Controls = ctrls;
    _page_private->BackGroundImage = pdata;
    _page_private->Callback = _callback;
    AddPAGE(_page_private);
    return _page_private;
}

EV_PAGE* CreatePageFormPath(uint16_t width, uint16_t height, uint32_t offset, char * path, void (*_callback)())
{
    EV_PAGE* _page_private = (EV_PAGE *)mymalloc(SRAMEX, sizeof(EV_PAGE));
    _page_private->ID = EV_Pages.Count;
    _page_private->Width = width;
    _page_private->Height = height;
    _page_private->Offset = offset;
    EV_CONTROLLIST* ctrls = (EV_CONTROLLIST *)mymalloc(SRAMEX, sizeof(EV_CONTROLLIST));
    ctrls->List = mymalloc(SRAMEX, 128 * 4);
    ctrls->Count = 0;
    _page_private->Controls = ctrls;
    _page_private->BackGroundImage = OpenImage(path);
    _page_private->Callback = _callback;
    AddPAGE(_page_private);
    return _page_private;
}


EV_CONTROL* CreateControl(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t offset, EV_PAGE* parent, uint16_t* pdata, uint16_t *ptext, uint16_t plength, uint8_t fontsize, uint16_t fontcolor)
{
    EV_CONTROL* _ctrl_private = (EV_CONTROL *)mymalloc(SRAMEX, sizeof(EV_CONTROL));
    _ctrl_private->ParentID = parent->ID;
    _ctrl_private->ID = parent->Controls->Count;
    _ctrl_private->X = x;
    _ctrl_private->Y = y;
    _ctrl_private->Width = width;
    _ctrl_private->Height = height;
    _ctrl_private->Offset = offset;
    _ctrl_private->BackGroundImage = pdata;
    _ctrl_private->BackGroundColor = TRANSPARENT;
    _ctrl_private->ForeColor = fontcolor;
    _ctrl_private->FontSize = fontsize;
    _ctrl_private->Text = ptext;
    _ctrl_private->TextLength = plength;
    AddControl(parent, _ctrl_private);
    LOG(LEVEL_WARNING, "ptext:%x\n",_ctrl_private->Text[0]);
    //LOG(LEVEL_WARNING, "page0 - ctrl->text%x\n",page0->Controls->List[0]->Text[0]);
    return _ctrl_private;
}

void Gui_Init()
{
    EV_LCD_Init();
    EV_Pages.List = mymalloc(SRAMEX, 32 * 4);
    EV_Pages.Count = 0;
}

//���ƿؼ�
void DrawControl_BACK(EV_CONTROL* ctrl)
{
    if(ctrl)
    {
        EV_LCD_FillColor(ctrl->X, ctrl->Y, (ctrl->Width-1) + ctrl->X, (ctrl->Height-1) + ctrl->Y, ctrl->BackGroundImage, 0);
    }
}

char* GetFontPath(uint8_t fontsize)
{

}

void DrawControl_FORE(EV_CONTROL* ctrl)
{
    char fontpath[20];
    if(ctrl)
    {
        if(ctrl->Text)
        {
            sprintf(fontpath, "0:/SY%d.ssf", ctrl->FontSize); 
            LOG(LEVEL_WARNING, "fontpath:%s\n", fontpath);
            EV_LCD_ShowUnicodeString(ctrl->X, ctrl->Y, ctrl->Width, ctrl->Height, ctrl->FontSize, ctrl->TextLength, EV_TA_HCENTER|EV_TA_VCENTER, ctrl->Text, fontpath, TRANSPARENT, ctrl->ForeColor, 1);
        }
    }
}

//����ҳ��
void DrawPage_BACK(EV_PAGE* page)
{
    if(page)
    {
        EV_LCD_FillColor(0, 0, page->Width-1, page->Height-1, page->BackGroundImage, 0);
    }
}
void DrawPage_FORE(EV_PAGE* page)
{
    if(page)
    {
        //EV_LCD_FillColor(0, 0, 799, 479, page->Data, 0);
    }
}

void ShowPage(EV_PAGE* page)
{
    EV_LCD_Clear(TRANSPARENT, 0);
    EV_LCD_Clear(TRANSPARENT, 1);

    DrawPage_BACK(page);
    LOG(LEVEL_WARNING, "page->ctrlcount:%d\n",page->Controls->Count);
    for(uint8_t i = 0; i < page->Controls->Count; i++)
    {
        LOG(LEVEL_WARNING, "%d ctrl\n",i);
        LOG(LEVEL_WARNING, "for - ctrl->text%x\n",page->Controls->List[0]->Text[0]);
        DrawControl_FORE(page->Controls->List[i]);
    }
    EV_APage = page;
}


uint8_t ContainsPoint(EV_CONTROL* ctrl, uint32_t x, uint32_t y)
{
	if(ctrl)
	{
		if(x >= ctrl->X && x <= ctrl->X + ctrl->Width && y >= ctrl->Y && y <= ctrl->Y + ctrl->Height)
		{
            return 1;
		}
	}
    return 0;
}


int32_t tp_x, tp_y;
uint8_t is_tp_down = 0;
uint8_t _notification = 0;
uint8_t isdowncount = 0;

void Notification()
{
    switch (_notification)
    {
        case 1:
            if(EV_APage)
            {
                EV_APage->Callback();
            }
            _notification = 0;
            break;
        default:
            //
            _notification = 0;
            break;
    }
}
void GuiTouchExec()
{
    tp_dev.scan(0);
    if(tp_dev.sta&TP_PRES_DOWN)
    {
        tp_x = tp_dev.x[0];
        tp_y = tp_dev.y[0];
    }
    else
    {
        tp_x = 1200;
        tp_y = 1200;
    }
    if(tp_x != 1200 && tp_y != 1200 && is_tp_down == 0)
    {
        is_tp_down = 1;
        isdowncount++;
        _notification = 1;
        LOG(LEVEL_WARNING, "isdowncount:%d\n",isdowncount);
    }

    if(tp_x == 1200 && tp_y == 1200 && is_tp_down == 1)
    {
        is_tp_down = 0;
    }
    Notification();
	//LOG(LEVEL_WARNING, "X:%d Y:%d\n",tp_x,tp_y);
}