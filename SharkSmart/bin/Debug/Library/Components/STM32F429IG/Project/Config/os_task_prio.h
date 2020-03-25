#ifndef __OS_TASK_PRIO_H
#define __OS_TASK_PRIO_H	 


//System task prio
#define TCPIP_THREAD_PRIO_NUM               2
#define LWIP_DHCP_TASK_PRIO_NUM             7
#define USB_DATA_PROCESSING_TASK_PRIO_NUM   10
#define M2_TASK_CHECK_INSERT_PRIO_NUM       13
#define ETH_USER_TASK_PRIO_NUM              14
#define START_TASK_PRIO_NUM                 15
//User Unit

#define WIFI_TASK_UPDATE_PRIO_NUM           27
#define M2_TASK_BCAST_PRIO_NUM              29
#define M2_TASK_NORMAL_PRIO_NUM             31
#define UDP_BCAST_PRIO_NUM                  33
#define UDP_NORMAL_PRIO_NUM                 35
#define LWIP_UPGRADE_PRIO_NUM               40

//User Method
#define USER_TASK1_PRIO_NUM                 70

// UI
#define TOUCH_TASK_PRIO_NUM                 90
#define EMWINDEMO_TASK_PRIO_NUM             95

#endif


