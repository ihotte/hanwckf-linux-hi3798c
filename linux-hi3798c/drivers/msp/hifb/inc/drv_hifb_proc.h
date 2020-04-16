/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
******************************************************************************
File Name	: drv_hifb_proc.h
Version		: Initial Draft
Author		:
Created		: 2016/08/27
Description	:
Function List	:


History		:
Date		      Author		    Modification
2016/08/27	      y00181162		     Created file
******************************************************************************/

/* SPDX-License-Identifier: GPL-2.0 */

#ifndef __DRV_HIFB_PROC_H__
#define __DRV_HIFB_PROC_H__


/*********************************add include here******************************/

#include "drv_hifb_config.h"

/***************************** Macro Definition ******************************/
#define HIFB_PROC_VOCALLBACK_COUNT_COSTTIME	  10

/*************************** Structure Definition ****************************/
typedef struct tagHIFB_PROC_CHECK_INTERRUPT_STATE_S
{
    /** vo callback scanline information **/
    HI_U32 VoCallBackInScanLine;
    HI_U32 VoCallBackOutScanLine;
    HI_U32 VoCallBackStartScanLine;
    HI_U32 VoCallBackUpScanLine;
    HI_U32 VoCallBackScanLineAbnormalNum;
    /** vo callback cost times information **/
    HI_U32 CountVocallBackTimeAbnormalNum;
    HI_U32 CountCurTimesForCostTime;
    HI_U32 VoCallBackCostTime[HIFB_PROC_VOCALLBACK_COUNT_COSTTIME];
}HIFB_PROC_CHECK_INTERRUPT_STATE_S;

typedef struct tagDrvHiFBProcS
{
    HI_BOOL bCreateProc;
    HI_BOOL bHasDeCmpErr;
    HI_BOOL bDebugByMoreMessage;
    HI_BOOL b3DStatus;
    HI_BOOL bHwcRefreshInDeCmpStatus;  /** decompress status from hard composer refresh	 */
    HI_U32 WbcLayerID;
    HI_U32 VdpIntMaskValue;
    HI_U32 HwcRefreshInDispFmt;	       /** display pixel format from hard composer refresh */
    HI_U32 HwcRefreshInDispStride;     /** display stride from hard composer refresh	      */
    HI_U32 HwcRefreshInDispAdress;     /** display address from hard composer refresh	    */
    HI_U32 HwcRefreshUnSyncCnt;	       /** account unsync by hwc refresh			   */
    HI_U32 DeCmpARDataStatus[2];
    HI_U32 DeCmpGBDataStatus[2];
    HI_U32 DeCmpARDataErrCnt;	       /** account decompress AR data error times	   */
    HI_U32 DeCmpGBDataErrCnt;	       /** account decompress GB data error times	   */
    HIFB_PROC_CHECK_INTERRUPT_STATE_S stCheckInterruptState;
}DRV_HIFB_PROC_S;


/********************** Global Variable declaration **************************/



/******************************* API declaration *****************************/
HI_S32	DRV_HIFB_CreateProc(HI_U32 LayerID);
HI_VOID DRV_HIFB_DestoryProc(HI_U32 LayerID);
HI_VOID HIFB_PROC_VoCallBackCostTime(HI_U32 LayerId, HI_U32 TimeMs);
HI_VOID HIFB_PROC_CountVoCallBackScanLineAbnormalTimes(HI_U32 LayerID);
HI_VOID HIFB_PROC_SetStartAndUpScanLine(HI_U32 StartScanLine, HI_U32 UpScanLine);
HI_VOID HIFB_PROC_SetVoCallBackInOutScanLine(HI_U32 LayerId, HI_U32 ScanLine, HI_BOOL bInScanLine);
HI_VOID HIFB_PROC_CheckScanLineAbnormal(HI_U32 LayerID, HI_BOOL *pAbnormal);

#endif
