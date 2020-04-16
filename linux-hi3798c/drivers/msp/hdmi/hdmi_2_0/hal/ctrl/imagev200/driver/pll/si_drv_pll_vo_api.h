/*
 * Copyright (C) 2017, Hisilicon Tech. Co., Ltd.
 * SPDX-License-Identifier: GPL-2.0
 */
/*************************************************************************
* si_drv_pll_vo_api.h
*************************************************************************/
#ifndef __SI_DRV_PLL_VO_API_H__
#define __SI_DRV_PLL_VO_API_H__

#include "si_datatypes.h"
#include "si_lib_obj_api.h"
#include "si_lib_video_api.h"

/************************************************************************/
/*	Type Definitions						*/
/************************************************************************/
typedef enum
{
	SII_DRV_PLL_VO_ERROR__NONE,
	SII_DRV_PLL_VO_EEROR__FREQ_NOT_FOUND
} SiiDrvPllVoError_t;

/************************************************************************/
/*	Function Prototype						    */
/************************************************************************/
//-------------------------------------------------------------------------------------------------
//! @brief	Standard module initialization.
//-------------------------------------------------------------------------------------------------
void SiiDrvPllVoCreate( SiiInst_t inst );

//-------------------------------------------------------------------------------------------------
//! @brief	Standard module closure.
//-------------------------------------------------------------------------------------------------
void SiiDrvPllVoDelete( void );

//-------------------------------------------------------------------------------------------------
//! @brief	Provides module ID.
//-------------------------------------------------------------------------------------------------
void* SiiDrvPllVoModuleId( void );

//-------------------------------------------------------------------------------------------------
//! @brief	Module Initializtion.
//-------------------------------------------------------------------------------------------------
void* SiiDrvPllVoModuleId( void );

//-------------------------------------------------------------------------------------------------
//! @brief	Set VidProc Input clock frequency.
//!
//! @param[in]	clkFrq	 : clock frequency
//-------------------------------------------------------------------------------------------------
void SiiDrvPllVoInputClockFrequencySet( SiiLibVideoFrq_t clkFrq );

//-------------------------------------------------------------------------------------------------
//! @brief	Get VidProc Input clock frequency.
//!
//! @return	clock frequency.
//-------------------------------------------------------------------------------------------------
SiiLibVideoFrq_t SiiDrvPllVoInputClockFrequencyGet( void );

//-------------------------------------------------------------------------------------------------
//! @brief	Set VidProc Output clock frequency.
//!
//! @param[in]	clkFrq	 : clock frequency
//-------------------------------------------------------------------------------------------------
void SiiDrvPllVoOutputClockFrequencySet(SiiInst_t inst, SiiLibVideoFrq_t clkFrq );

//-------------------------------------------------------------------------------------------------
//! @brief	Get VidProc Output clock frequency.
//!
//! @return	clock frequency.
//-------------------------------------------------------------------------------------------------
SiiLibVideoFrq_t SiiDrvPllVoOutputClockFrequencyGet( void );

#endif /* __SI_DRV_PLL_VO_API_H__ */
