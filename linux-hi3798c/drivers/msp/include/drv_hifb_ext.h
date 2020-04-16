/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
******************************************************************************
File Name	: drv_hifb_ext.h
Version		: Initial Draft
Author		:
Created		: 2016/02/02
Description	:
Function List	:


History		:
Date		    Author		  Modification
2016/02/02	      y00181162		     Created file
******************************************************************************/

/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __DRV_HIFB_EXT_H__
#define __DRV_HIFB_EXT_H__


/*********************************add include here******************************/
#include "hi_type.h"
#include "drv_pq_define.h"

/***************************** Macro Definition ******************************/
typedef HI_S32	(*FN_HIFB_ModInit)(HI_VOID);
typedef HI_VOID (*FN_HIFB_ModeExit)(HI_VOID);
typedef HI_VOID (*FN_HIFB_SetLogoLayerEnable)(HI_BOOL);
typedef HI_S32	(*FN_HIFB_UpdatePqData)(HI_U32 u32UpdateType,PQ_PARAM_S * pstPqParam);



#define hifb_init_module_k	 DRV_HIFB_ModInit_K
#define hifb_cleanup_module_k	 DRV_HIFB_ModExit_K

/*************************** Structure Definition ****************************/
typedef struct
{
    FN_HIFB_ModInit		 pfnHifbModInit;
    FN_HIFB_ModeExit		 pfnHifbModExit;
    FN_HIFB_SetLogoLayerEnable	 pfnHifbSetLogoLayerEnable;
    FN_HIFB_UpdatePqData	 pfnHifbUpdatePqData;
} HIFB_EXPORT_FUNC_S;

/***************************  The enum of Jpeg image format  ******************/

/********************** Global Variable declaration **************************/


/******************************* API declaration *****************************/
HI_S32	HIFB_DRV_ModInit(HI_VOID);
HI_VOID HIFB_DRV_ModExit(HI_VOID);

HI_S32	DRV_HIFB_ModInit_K(HI_VOID);
HI_VOID DRV_HIFB_ModExit_K(HI_VOID);

#endif
