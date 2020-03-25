#ifndef __GLOBAL_USER_H
#define __GLOBAL_USER_H

extern void DisplayUnit_Init();
extern EV_PAGE* page0;
extern EV_CONTROL* p0_ctrl0;
extern EV_PAGE* page1;
extern EV_CONTROL* p1_ctrl0;
extern void CreatePage0();
extern void CreatePage1();

extern char*  Data_version;
extern char*  Data_Page0_IMAGE0;
extern char*  Data_Page0_IMAGE1;
extern void Data_Init();


#endif