/*
 * Copyright (C) 2017, Hisilicon Tech. Co., Ltd.
 * SPDX-License-Identifier: GPL-2.0
 */
#include <linux/slab.h>
#include "drv_vdec_alg.h"


//#define OPTM_ALG_ACMCHECKPRINT 0

static HI_VOID OPTM_CalAvrgFrameRate(OPTM_ALG_FRD_S *pPtsInfo)
{
    HI_U32 nowRate;
    HI_S32 rateDiff;
    HI_U32 CalRate;
    HI_S32 s32WaveDelta;

    nowRate = OPTM_ALG_InPTSSqn_CalNowRate(pPtsInfo, 1);

    rateDiff = (pPtsInfo->InFrameRate > nowRate)? (pPtsInfo->InFrameRate - nowRate): (nowRate - pPtsInfo->InFrameRate);

    /* For stream about 30fps, use bigger JUMP hreshold, accept wider PTS diff range */
    /* CNcomment: ��30fps���ҵĳ�������,�Ӵ�΢С�仯�ع����ޣ�
	��Ϊǰ�����޼Ӵ�
	���ܻᵼ��ȫ���������ƽ��֡�ʴ��ڶ������Ŵ�����ޣ��������֡��Ƶ��΢С�仯*/
    if(pPtsInfo->InFrameRateLast >= 200 && pPtsInfo->InFrameRateLast <= 400)
    {
	s32WaveDelta = 50;
    }
    else
    {
	s32WaveDelta = pPtsInfo->InFrameRateLast
	    / OPTM_FRD_INFRAME_RATE_JUMP_THRESHOLD
	    / OPTM_FRD_INFRAME_RATE_WAVE_THRESHOLD;
    }
    /* detect the tiny rate wave*/
    if (rateDiff > s32WaveDelta)
    {
	CalRate = (pPtsInfo->InFrameRate != 0)? (nowRate+pPtsInfo->InFrameRate) / 2 : nowRate;
	OPTM_ALG_InPTSSqn_ChangeInFrameRate(pPtsInfo, CalRate);
	pPtsInfo->InFrameRateLast = nowRate;
    }
    else
    {
	/* the second time pPtsInfo->InFrameRateLast == nowRate,we adjust
	InFrameRate as nowRate,because the tiny ratediff between InframeRate and actual
	FrameRate may exist*/
	if (pPtsInfo->InFrameRateLast == nowRate)
	{
	    if (OPTM_ALG_ABS(rateDiff) > (pPtsInfo->InFrameRateLast / OPTM_FRD_INFRAME_RATE_JUMP_THRESHOLD / OPTM_FRD_INFRAME_RATE_WAVE_THRESHOLD)
	     && pPtsInfo->InFrameRate != 0)
	    {
		CalRate = (nowRate+pPtsInfo->InFrameRate) / 2;
	    }
	    else
	    {
		CalRate = nowRate;
	    }
	    OPTM_ALG_InPTSSqn_ChangeInFrameRate(pPtsInfo, CalRate);
	}
	else
	{
	    pPtsInfo->InFrameRateLast = nowRate;
	}
    }

    /* reset the PTSSqn,reserve the lastest one pts data*/
    OPTM_ALG_FrdInfo_Reset(pPtsInfo, 1);
}

static HI_VOID OPTM_AdjustRateDiff(OPTM_ALG_FRD_S *pPtsInfo, HI_U32 nowRate)
{
    HI_U32 u32Stable;
    HI_S32 rateDiff;

    pPtsInfo->InFrameRateEqueTime++;

    /* if 3 == pPtsInfo->InFrameRateEqueTime,
	we think the fps has been stable,
	so reset the unableTime */
    if (3 == pPtsInfo->InFrameRateEqueTime
	|| nowRate == pPtsInfo->InFrameRate)
    {
	pPtsInfo->unableTime = 0;
    }

    /* For stream about 30fps, use bigger JUMP hreshold, accept wider PTS diff range */
    /* CNcomment: ��30fps���ҵĳ�������,�Ӵ�֡�ʱ仯�ȶ����ޣ�
	    ��֤������Ϊ����֡�ȶ���PTS���������޷�������Ч֡��*/
    /* calculate the times of occurrence of stability of new frame rate*/
    if(pPtsInfo->InFrameRateLast >= 200 && pPtsInfo->InFrameRateLast <= 400)
    {
	u32Stable = 10;
    }
    else
    {
	u32Stable = pPtsInfo->StableThreshold;
    }

    if (pPtsInfo->InFrameRateEqueTime == u32Stable)
    {
	nowRate = OPTM_ALG_InPTSSqn_CalNowRate(pPtsInfo, 1);
	rateDiff = nowRate - pPtsInfo->InFrameRate;

	if (OPTM_ALG_ABS(rateDiff) > pPtsInfo->InFrameRate / OPTM_FRD_INFRAME_RATE_JUMP_THRESHOLD)
	{
	    OPTM_ALG_InPTSSqn_ChangeInFrameRate(pPtsInfo, nowRate);

	    pPtsInfo->InFrameRateLast = pPtsInfo->InFrameRate;
	}
    }

    /* when InPTSSqn is full,calculate the average fps */
    if (pPtsInfo->nowPTSPtr == pPtsInfo->length-1)
    {
	OPTM_CalAvrgFrameRate(pPtsInfo);
    }
    else
    {
	pPtsInfo->nowPTSPtr++;
    }
}

static HI_VOID OPTM_AdjustRate(OPTM_ALG_FRD_S *pPtsInfo, HI_U32 nowRate)
{
    HI_U32 u32JumpDelta;
    HI_S32 rateDiff;

    rateDiff = nowRate - pPtsInfo->InFrameRateLast;

    /* For stream about 30fps, use bigger JUMP hreshold, accept wider PTS diff range */
    /* CNcomment: ��30fps���ҵĳ�������,�Ŵ�JUMP���ޣ���PTS��������������Χ*/
    if(pPtsInfo->InFrameRateLast >= 200 && pPtsInfo->InFrameRateLast <= 400)
    {
	u32JumpDelta = 200;
    }
    else
    {
	u32JumpDelta = pPtsInfo->InFrameRateLast / OPTM_FRD_INFRAME_RATE_JUMP_THRESHOLD;
    }

    if (OPTM_ALG_ABS(rateDiff) > u32JumpDelta)
    {
	pPtsInfo->unableTime++;
	if (10 == pPtsInfo->unableTime)
	{
	    pPtsInfo->InFrameRate = 0;
	    pPtsInfo->unableTime   = 0;
	}

	pPtsInfo->InFrameRateLast = nowRate;

	/* the StableThreshold = fps/10;
	   in order to detect the fps change quickly; */
	if (nowRate > 300)
	    pPtsInfo->StableThreshold = nowRate / 100;
	else
	    pPtsInfo->StableThreshold = 3;

	/* reset the PTSSqn,reserve the lastest two pts data*/
	OPTM_ALG_FrdInfo_Reset(pPtsInfo, 2);
    }
    else
    {
	OPTM_AdjustRateDiff(pPtsInfo, nowRate);
    }
}

static HI_VOID OPTM_PTSSample(OPTM_ALG_FRD_S *pPtsInfo, HI_U32 Pts)
{
    HI_U32 nowRate;

    pPtsInfo->InPTSSqn[pPtsInfo->nowPTSPtr] = Pts;

    nowRate = OPTM_ALG_InPTSSqn_CalNowRate(pPtsInfo, 0);

    if (nowRate)
    {
	OPTM_AdjustRate(pPtsInfo, nowRate);
    }
    else
    {
	/* nowRate = 0,we think it is a unstable fps*/
	pPtsInfo->unableTime++;
	if (10 == pPtsInfo->unableTime)
	{
	    pPtsInfo->InFrameRate = 0;
	    pPtsInfo->unableTime  = 0;
	}

	OPTM_ALG_FrdInfo_Reset(pPtsInfo, 1);
    }
}

static HI_VOID OPTM_UpdateNewRate(OPTM_ALG_FRD_S *pPtsInfo, HI_U32 Pts)
{
    HI_U32 u32Delta = 0;
    HI_U32 u32NewRate = 0;
    HI_U32 u32RateABS = 0;

    if (Pts > pPtsInfo->u320_Pts)
    {
	u32Delta = (Pts - pPtsInfo->u320_Pts)/60;
    }
    else
    {
	u32Delta = (pPtsInfo->u320_Pts - Pts)/60;
    }

    if(0 != u32Delta)
    {
	u32NewRate = 10000/u32Delta;
    }

    u32RateABS = (u32NewRate > pPtsInfo->u32QueRate)? (u32NewRate - pPtsInfo->u32QueRate): (pPtsInfo->u32QueRate - u32NewRate);

    if (u32RateABS < 10)
    {
	if (pPtsInfo->u32QueStable < 3)
	    pPtsInfo->u32QueStable++;
    }
    else
    {
	pPtsInfo->u32QueRate = u32NewRate;
	pPtsInfo->u32QueStable = 0;
    }

    pPtsInfo->u320_Pts = Pts;
    pPtsInfo->u32QueCnt = 0;
}

HI_U32 OPTM_ALG_FrameRateDetect(OPTM_ALG_FRD_S *pPtsInfo, HI_U32 Pts)
{
    if (pPtsInfo->u320_Pts == HI_INVALID_PTS)
    {
	pPtsInfo->u320_Pts = Pts;
    }
    else
    {
	pPtsInfo->u32QueCnt = pPtsInfo->u32QueCnt+1;
    }

    if (pPtsInfo->u32QueCnt == 61)
    {
	OPTM_UpdateNewRate(pPtsInfo, Pts);
    }

    if (pPtsInfo->InPTSSqn[0] == HI_INVALID_PTS)
    {
	OPTM_ALG_InPTSSqn_ChangeInFrameRate(pPtsInfo, 0);

	OPTM_ALG_FrdInfo_Reset(pPtsInfo, 0);

	pPtsInfo->InPTSSqn[0] = Pts;
	pPtsInfo->nowPTSPtr = 1;
	pPtsInfo->unableTime = 0;
	pPtsInfo->InFrameRateLast = pPtsInfo->InFrameRate;
    }
    else
    {
	OPTM_PTSSample(pPtsInfo, Pts);
    }

    if(pPtsInfo->u32QueStable == 3 && pPtsInfo->InFrameRate == 0)
    {
	pPtsInfo->InFrameRate = pPtsInfo->u32QueRate;
    }

    return pPtsInfo->InFrameRate;
}

HI_VOID OPTM_ALG_FrdInfo_Reset(OPTM_ALG_FRD_S *pPtsInfo, HI_U32 ptsNum)
{
    HI_U32 nowPts;
    HI_U32 prePts;
    HI_U32 i;

    if (ptsNum)
    {
	if (ptsNum == 1)
	{
	    nowPts = pPtsInfo->InPTSSqn[pPtsInfo->nowPTSPtr];
	    for (i = 0; i < OPTM_FRD_IN_PTS_SEQUENCE_LENGTH; i++)
	    {
		pPtsInfo->InPTSSqn[i] = 0xffffffff;
	    }
	    pPtsInfo->InPTSSqn[0] = nowPts;
	    pPtsInfo->nowPTSPtr = 1;
	    pPtsInfo->InFrameRateEqueTime = 0;
	}
	else//ptsNum == 2
	{
	    nowPts = pPtsInfo->InPTSSqn[pPtsInfo->nowPTSPtr];
	    prePts = pPtsInfo->InPTSSqn[pPtsInfo->nowPTSPtr-1];
	    for (i = 0; i < OPTM_FRD_IN_PTS_SEQUENCE_LENGTH; i++)
	    {
		pPtsInfo->InPTSSqn[i] = 0xffffffff;
	    }
	    pPtsInfo->InPTSSqn[0] = prePts;
	    pPtsInfo->InPTSSqn[1] = nowPts;
	    pPtsInfo->nowPTSPtr = 2;
	    pPtsInfo->InFrameRateEqueTime = 1;
	}
    }
    else
    {
	for (i = 0; i < OPTM_FRD_IN_PTS_SEQUENCE_LENGTH; i++)
	{
	    pPtsInfo->InPTSSqn[i] = 0xffffffff;
	}

	pPtsInfo->InFrameRateEqueTime = 0;
	pPtsInfo->u320_Pts = 0xffffffff;
	pPtsInfo->u32120_Pts = 0xffffffff;
	pPtsInfo->u32QueCnt = 0;
	pPtsInfo->u32QueStable = 0;
    }

}
