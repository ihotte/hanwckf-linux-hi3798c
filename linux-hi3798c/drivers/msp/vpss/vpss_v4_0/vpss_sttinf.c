/*
 * Copyright (C) 2017, Hisilicon Tech. Co., Ltd.
 * SPDX-License-Identifier: GPL-2.0
 */
#include "vpss_sttinf.h"

/*Die Code*/
HI_VOID VPSS_STTINFO_CalDieBufSize(HI_U32 *pSize, HI_U32 *pStride, HI_U32 u32Width, HI_U32 u32Heigth)
{
    *pStride = ((u32Width + 3) / 4 * 2 + 15) / 16 * 16;
    *pStride = HI_ALIGN_8BIT_YSTRIDE(*pStride);
    *pSize = (*pStride) * u32Heigth / 2;

    return ;
}

HI_S32 VPSS_STTINFO_DieInit(VPSS_DIESTINFO_S *pstDieStInfo,
			    HI_U32 u32Width,
			    HI_U32 u32Heigth,
			    HI_BOOL bSecure)
{
    LIST *pListHead = HI_NULL;
    HI_U32 u32TotalBuffSize = 0;
    HI_U32 u32PhyAddr = 0;
    HI_S32 s32Ret = HI_FAILURE;

    if (HI_NULL == pstDieStInfo)
    {
	VPSS_ERROR("Vpss sttinfo die Init error(null pointer).\n");
	return HI_FAILURE;
    }
    if (HI_TRUE == pstDieStInfo->bInit)
    {
	(HI_VOID)VPSS_STTINFO_DieDeInit(pstDieStInfo);
    }

    memset(pstDieStInfo, 0, sizeof(VPSS_DIESTINFO_S));
    pstDieStInfo->u32Width = u32Width;
    pstDieStInfo->u32Height = u32Heigth;
    pListHead = &(pstDieStInfo->stDataList[0].node);
    INIT_LIST_HEAD(pListHead);
    pstDieStInfo->pstFirstRef = pListHead;

    VPSS_STTINFO_CalDieBufSize(&(pstDieStInfo->u32DieInfoSize), &(pstDieStInfo->u32Stride), u32Width, u32Heigth);
    u32TotalBuffSize = pstDieStInfo->u32DieInfoSize * VPSS_DIE_MAX_NODE;


    if (bSecure)
    {
	s32Ret = HI_DRV_SMMU_AllocAndMap( "VPSS_SttDieBuf", u32TotalBuffSize, 0, &(pstDieStInfo->stTEEBuf));
	if (HI_FAILURE == s32Ret)
	{
	    VPSS_FATAL("VPSS STTDie Alloc memory failed.\n");
	    return HI_FAILURE;
	}
	u32PhyAddr = pstDieStInfo->stTEEBuf.u32StartSmmuAddr;
    }
    else
    {
	s32Ret = HI_DRV_SMMU_AllocAndMap( "VPSS_SttDieBuf", u32TotalBuffSize, 0, &(pstDieStInfo->stMMUBuf));

	if (HI_FAILURE == s32Ret)
	{
	    VPSS_FATAL("VPSS STTDie Alloc memory failed.\n");
	    return HI_FAILURE;
	}
	memset((HI_U8 *)pstDieStInfo->stMMUBuf.pu8StartVirAddr, 0, pstDieStInfo->stMMUBuf.u32Size);

	u32PhyAddr = pstDieStInfo->stMMUBuf.u32StartSmmuAddr;
    }

    list_add_tail(&(pstDieStInfo->stDataList[1].node), pListHead);
    list_add_tail(&(pstDieStInfo->stDataList[2].node), pListHead);
    pstDieStInfo->stDataList[0].u32PhyAddr = u32PhyAddr;
    u32PhyAddr = u32PhyAddr + pstDieStInfo->u32DieInfoSize;
    pstDieStInfo->stDataList[1].u32PhyAddr = u32PhyAddr;
    u32PhyAddr = u32PhyAddr + pstDieStInfo->u32DieInfoSize;
    pstDieStInfo->stDataList[2].u32PhyAddr = u32PhyAddr;
    pstDieStInfo->bInit = HI_TRUE;
    return HI_SUCCESS;
}
HI_S32 VPSS_STTINFO_DieDeInit(VPSS_DIESTINFO_S *pstDieStInfo)
{
    if (HI_NULL == pstDieStInfo)
    {
	VPSS_ERROR("Vpss sttDie DeInit error(null pointer).\n");
	return HI_FAILURE;
    }

    if (HI_FALSE == pstDieStInfo->bInit)
    {
	VPSS_WARN("Vpss sttDie DeInit error(not init).\n");
	return HI_FAILURE;
    }
    if (0 != pstDieStInfo->stTEEBuf.u32StartSmmuAddr)
    {
	HI_DRV_SMMU_UnmapAndRelease(&(pstDieStInfo->stTEEBuf));
    }
    if (0 != pstDieStInfo->stMMUBuf.u32StartSmmuAddr)
    {
	HI_DRV_SMMU_UnmapAndRelease(&(pstDieStInfo->stMMUBuf));
    }
    memset(pstDieStInfo, 0, sizeof(VPSS_DIESTINFO_S));

    return HI_SUCCESS;
}
HI_S32 VPSS_STTINFO_DieGetInfo(VPSS_DIESTINFO_S *pstDieStInfo, VPSS_DIESTCFG_S *pstDieStCfg)
{
    VPSS_DIE_DATA_S *pstSTTNodeData = HI_NULL;
    LIST *pstSTTNode = HI_NULL;
    if ((HI_NULL == pstDieStInfo) || (HI_NULL == pstDieStCfg))
    {
	VPSS_ERROR("Vpss sttdie GetInfo error(null pointer).\n");
	return HI_FAILURE;
    }

    if (HI_FALSE == pstDieStInfo->bInit)
    {
	VPSS_ERROR("Vpss sttDie GetInfo error(null init).\n");
	return HI_FAILURE;
    }

    pstDieStCfg->u32Stride = pstDieStInfo->u32Stride;
    pstSTTNode = pstDieStInfo->pstFirstRef;
    pstSTTNodeData = list_entry(pstSTTNode, VPSS_DIE_DATA_S, node);
    pstDieStCfg->u32PPreAddr = pstSTTNodeData->u32PhyAddr;
    pstSTTNode = pstSTTNode->next;
    pstSTTNodeData = list_entry(pstSTTNode, VPSS_DIE_DATA_S, node);
    pstDieStCfg->u32PreAddr = pstSTTNodeData->u32PhyAddr;
    pstSTTNode = pstDieStInfo->pstFirstRef->prev;
    pstSTTNodeData = list_entry(pstSTTNode, VPSS_DIE_DATA_S, node);
    pstDieStCfg->u32CurAddr = pstSTTNodeData->u32PhyAddr;

    return HI_SUCCESS;
}
HI_S32 VPSS_STTINFO_DieComplete(VPSS_DIESTINFO_S *pstDieStInfo)
{
    if (HI_NULL == pstDieStInfo)
    {
	VPSS_ERROR("Vpss sttdie complete error(null pointer).\n");
	return HI_FAILURE;
    }

    if (HI_FALSE == pstDieStInfo->bInit)
    {
	VPSS_ERROR("Vpss sttDie complete error(null init).\n");
	return HI_FAILURE;
    }

    pstDieStInfo->u32Cnt++;
    pstDieStInfo->pstFirstRef = pstDieStInfo->pstFirstRef->next;

    return HI_SUCCESS;
}

HI_S32 VPSS_STTINFO_DieReset(VPSS_DIESTINFO_S *pstDieStInfo)
{
    if (HI_NULL == pstDieStInfo)
    {
	VPSS_WARN("Vpss sttdie complete error(null pointer).\n");
	return HI_FAILURE;
    }

    if (HI_FALSE == pstDieStInfo->bInit)
    {
	VPSS_WARN("Vpss sttDie complete error(null init).\n");
	return HI_FAILURE;
    }

    pstDieStInfo->u32Cnt = 0;

    if (0 != pstDieStInfo->stMMUBuf.pu8StartVirAddr)
    {
	memset((HI_U8 *)pstDieStInfo->stMMUBuf.pu8StartVirAddr, 0, pstDieStInfo->stMMUBuf.u32Size);
    }

    return HI_SUCCESS;
}

/*CCCL Code*/






























/*NR Code*/
HI_U32 VPSS_STTINFO_GetNrTotalBufSize(VPSS_NR_ATTR_S *pstAttr, HI_U32 u32NodeBufSize)
{
    if (NR_MODE_FRAME == pstAttr->enMode)
    {
	return	u32NodeBufSize * 2;
    }
    else if (NR_MODE_3FIELD == pstAttr->enMode)
    {
	return	u32NodeBufSize * 3;
    }
    else
    {
	return	u32NodeBufSize * 4;
    }

}

HI_VOID VPSS_STTINFO_CalNrBufSize(HI_U32 *pSize, HI_U32 *pStride, VPSS_NR_ATTR_S *pstAttr)
{
    if (NR_MODE_FRAME == pstAttr->enMode)
    {
	*pStride = ((pstAttr->u32Width * 5 + 7) / 8 + 0xf) & 0xfffffff0;
	*pStride = HI_ALIGN_8BIT_YSTRIDE(*pStride);
	*pSize = (*pStride) * pstAttr->u32Height;
    }
    else
    {
	*pStride = ((pstAttr->u32Width * 5 + 7) / 8 + 0xf) & 0xfffffff0;
	*pStride = HI_ALIGN_8BIT_YSTRIDE(*pStride);
	*pSize = (*pStride) * pstAttr->u32Height;
    }

    return;
}

HI_S32 VPSS_STTINFO_NrInit(VPSS_NRMADINFO_S *pstNrMadInfo, VPSS_NR_ATTR_S *pstAttr)
{
    LIST *pListHead = HI_NULL;
    HI_U32 u32TotalBuffSize = 0;
    HI_U32 u32PhyAddr = 0;
    HI_S32 s32Ret = HI_FAILURE;

    if (HI_NULL == pstNrMadInfo)
    {
	VPSS_ERROR("Vpss NrInit error(null pointer).\n");
	return HI_FAILURE;
    }
    if (HI_TRUE == pstNrMadInfo->bInit)
    {
	(HI_VOID)VPSS_STTINFO_NrDeInit(pstNrMadInfo);
    }

    memset(pstNrMadInfo, 0, sizeof(VPSS_NRMADINFO_S));
    VPSS_SAFE_MEMCPY(&(pstNrMadInfo->stAttr), pstAttr, sizeof(VPSS_NR_ATTR_S));
    pListHead = &(pstNrMadInfo->stDataList[0].node);
    INIT_LIST_HEAD(pListHead);
    pstNrMadInfo->pstFirstRef = pListHead;
    VPSS_STTINFO_CalNrBufSize(&pstNrMadInfo->u32NRMADSize, &pstNrMadInfo->u32madstride, pstAttr);
    u32TotalBuffSize = VPSS_STTINFO_GetNrTotalBufSize(pstAttr, pstNrMadInfo->u32NRMADSize);


    if (pstAttr->bSecure)
    {
	s32Ret = HI_DRV_SMMU_AllocAndMap( "VPSS_SttNrBuf_SMMU",
					  u32TotalBuffSize, 0, &(pstNrMadInfo->stTEEBuf));
	if (HI_FAILURE == s32Ret)
	{
	    VPSS_FATAL("VPSS Nr Alloc memory failed.\n");
	    return HI_FAILURE;
	}

	u32PhyAddr = pstNrMadInfo->stTEEBuf.u32StartSmmuAddr;
    }
    else
    {
	s32Ret = HI_DRV_SMMU_AllocAndMap( "VPSS_SttNrBuf_SMMU",
					  u32TotalBuffSize, 0, &(pstNrMadInfo->stMMUBuf));

	if (HI_FAILURE == s32Ret)
	{
	    VPSS_FATAL("VPSS Nr Alloc memory failed.\n");
	    return HI_FAILURE;
	}

	memset((HI_U8 *)pstNrMadInfo->stMMUBuf.pu8StartVirAddr, 0, pstNrMadInfo->stMMUBuf.u32Size);

	u32PhyAddr = pstNrMadInfo->stMMUBuf.u32StartSmmuAddr;
    }


    switch (pstAttr->enMode)
    {
	case NR_MODE_FRAME:
	{
	    list_add_tail(&(pstNrMadInfo->stDataList[1].node), pListHead);
	    pstNrMadInfo->stDataList[0].u32PhyAddr = u32PhyAddr;
	    u32PhyAddr = u32PhyAddr + pstNrMadInfo->u32NRMADSize;
	    pstNrMadInfo->stDataList[1].u32PhyAddr = u32PhyAddr;
	    break;
	}
	case NR_MODE_3FIELD:
	{
	    list_add_tail(&(pstNrMadInfo->stDataList[1].node), pListHead);
	    list_add_tail(&(pstNrMadInfo->stDataList[2].node), pListHead);
	    pstNrMadInfo->stDataList[0].u32PhyAddr = u32PhyAddr;
	    u32PhyAddr = u32PhyAddr + pstNrMadInfo->u32NRMADSize;
	    pstNrMadInfo->stDataList[1].u32PhyAddr = u32PhyAddr;
	    u32PhyAddr = u32PhyAddr + pstNrMadInfo->u32NRMADSize;
	    pstNrMadInfo->stDataList[2].u32PhyAddr = u32PhyAddr;
	    break;
	}
	case NR_MODE_5FIELD:
	{
	    list_add_tail(&(pstNrMadInfo->stDataList[1].node), pListHead);
	    list_add_tail(&(pstNrMadInfo->stDataList[2].node), pListHead);
	    list_add_tail(&(pstNrMadInfo->stDataList[3].node), pListHead);
	    pstNrMadInfo->stDataList[0].u32PhyAddr = u32PhyAddr;
	    u32PhyAddr = u32PhyAddr + pstNrMadInfo->u32NRMADSize;
	    pstNrMadInfo->stDataList[1].u32PhyAddr = u32PhyAddr;
	    u32PhyAddr = u32PhyAddr + pstNrMadInfo->u32NRMADSize;
	    pstNrMadInfo->stDataList[2].u32PhyAddr = u32PhyAddr;
	    u32PhyAddr = u32PhyAddr + pstNrMadInfo->u32NRMADSize;
	    pstNrMadInfo->stDataList[3].u32PhyAddr = u32PhyAddr;
	    break;
	}
	default:
	{
	    VPSS_ERROR("Vpss Nr GetRefInfo error(Mode = %d).\n", pstAttr->enMode);
	    return HI_FAILURE;
	}
    }
    pstNrMadInfo->bInit = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 VPSS_STTINFO_NrDeInit(VPSS_NRMADINFO_S *pstNrMadInfo)
{
    if (HI_NULL == pstNrMadInfo)
    {
	VPSS_ERROR("Vpss Nr DeInit error(null pointer).\n");
	return HI_FAILURE;
    }

    if (HI_FALSE == pstNrMadInfo->bInit)
    {
	VPSS_WARN("Vpss Nr DeInit error(not init).\n");
	return HI_FAILURE;
    }
    if (0 != pstNrMadInfo->stTEEBuf.u32StartSmmuAddr)
    {
	HI_DRV_SMMU_UnmapAndRelease(&(pstNrMadInfo->stTEEBuf));
    }

    if (0 != pstNrMadInfo->stMMUBuf.u32StartSmmuAddr)
    {
	HI_DRV_SMMU_UnmapAndRelease(&(pstNrMadInfo->stMMUBuf));
    }
    memset(pstNrMadInfo, 0, sizeof(VPSS_NRMADINFO_S));
    return HI_SUCCESS;
}
HI_S32 VPSS_STTINFO_NrGetInfo(VPSS_NRMADINFO_S *pstNrMadInfo,
			      VPSS_NRMADCFG_S *pstNrMadCfg)
{
    VPSS_NR_DATA_S *pstRefNodeData = HI_NULL;
    LIST *pstRefNode = HI_NULL;

    if ((HI_NULL == pstNrMadInfo) || (HI_NULL == pstNrMadCfg))
    {
	VPSS_ERROR("Vpss Nr GetRefInfo error(null pointer).\n");
	return HI_FAILURE;
    }

    if (HI_FALSE == pstNrMadInfo->bInit)
    {
	VPSS_ERROR("Vpss Nr GetRefInfo error(not init).\n");
	return HI_FAILURE;
    }

    switch (pstNrMadInfo->stAttr.enMode)
    {
	case NR_MODE_FRAME:
	{
	    pstRefNode = pstNrMadInfo->pstFirstRef;
	    pstRefNodeData = list_entry(pstRefNode, VPSS_NR_DATA_S, node);
	    pstNrMadCfg->u32Tnrmad_raddr = pstRefNodeData->u32PhyAddr;
	    pstRefNode = pstNrMadInfo->pstFirstRef->prev;
	    pstRefNodeData = list_entry(pstRefNode, VPSS_NR_DATA_S, node);
	    pstNrMadCfg->u32Tnrmad_waddr = pstRefNodeData->u32PhyAddr;
	    break;
	}
	case NR_MODE_3FIELD:
	{
	    pstRefNode = pstNrMadInfo->pstFirstRef;
	    pstRefNodeData = list_entry(pstRefNode, VPSS_NR_DATA_S, node);
	    pstNrMadCfg->u32Tnrmad_raddr = pstRefNodeData->u32PhyAddr;

	    pstRefNode = pstRefNode->next;
	    pstRefNodeData = list_entry(pstRefNode, VPSS_NR_DATA_S, node);
	    pstNrMadCfg->u32Snrmad_raddr = pstRefNodeData->u32PhyAddr;

	    pstRefNode = pstNrMadInfo->pstFirstRef->prev;
	    pstRefNodeData = list_entry(pstRefNode, VPSS_NR_DATA_S, node);
	    pstNrMadCfg->u32Tnrmad_waddr = pstRefNodeData->u32PhyAddr;
	    break;
	}
	case NR_MODE_5FIELD:
	{
	    pstRefNode = pstNrMadInfo->pstFirstRef;
	    pstRefNodeData = list_entry(pstRefNode, VPSS_NR_DATA_S, node);
	    pstNrMadCfg->u32Snrmad_raddr = pstRefNodeData->u32PhyAddr;
	    pstRefNode = pstRefNode->next;
	    pstRefNodeData = list_entry(pstRefNode, VPSS_NR_DATA_S, node);
	    pstNrMadCfg->u32Tnrmad_raddr = pstRefNodeData->u32PhyAddr;
	    pstRefNode = pstNrMadInfo->pstFirstRef->prev;
	    pstRefNodeData = list_entry(pstRefNode, VPSS_NR_DATA_S, node);
	    pstNrMadCfg->u32Tnrmad_waddr = pstRefNodeData->u32PhyAddr;
	    break;
	}
	default:
	{
	    VPSS_ERROR("Vpss Nr GetRefInfo error(Mode = %d).\n", pstNrMadInfo->stAttr.enMode);
	    return HI_FAILURE;
	}
    }

    pstNrMadCfg->u32madstride = pstNrMadInfo->u32madstride;
    return HI_SUCCESS;
}
HI_S32 VPSS_STTINFO_NrComplete(VPSS_NRMADINFO_S *pstNrMadInfo)
{
    if (HI_NULL == pstNrMadInfo)
    {
	VPSS_ERROR("Vpss Nr complete error(null pointer).\n");
	return HI_FAILURE;
    }

    if (HI_FALSE == pstNrMadInfo->bInit)
    {
	VPSS_ERROR("Vpss Nr complete error(not init).\n");
	return HI_FAILURE;
    }

    pstNrMadInfo->u32Cnt++;
    pstNrMadInfo->pstFirstRef = pstNrMadInfo->pstFirstRef->next;

    return HI_SUCCESS;
}

HI_S32 VPSS_STTINFO_NrReset(VPSS_NRMADINFO_S *pstNrMadInfo)
{
    if (HI_NULL == pstNrMadInfo)
    {
	VPSS_WARN("Vpss Nr complete error(null pointer).\n");
	return HI_FAILURE;
    }

    if (HI_FALSE == pstNrMadInfo->bInit)
    {
	VPSS_WARN("Vpss Nr complete error(not init).\n");
	return HI_FAILURE;
    }

    pstNrMadInfo->u32Cnt = 0;

    if (0 != pstNrMadInfo->stMMUBuf.pu8StartVirAddr)
    {
	memset((HI_U8 *)pstNrMadInfo->stMMUBuf.pu8StartVirAddr, 0, pstNrMadInfo->stMMUBuf.u32Size);
    }

    return HI_SUCCESS;


}

/* STT WBC */

HI_S32 VPSS_STTINFO_SttWbcInit(VPSS_STTWBC_S *psttWbc)
{
    HI_S32 s32Ret = HI_FAILURE;

    if (HI_NULL == psttWbc)
    {
	VPSS_ERROR("Vpss sttwbc Init error(null pointer).\n");
	return HI_FAILURE;
    }

    if (HI_TRUE == psttWbc->bInit)
    {
	(HI_VOID)VPSS_STTINFO_SttWbcDeInit(psttWbc);
    }

    s32Ret = HI_DRV_SMMU_AllocAndMap( "VPSS_SttWbcBuf", VPSS_STTWBC_SIZE, 0, &(psttWbc->stMMUBuf));

    if (HI_FAILURE == s32Ret)
    {
	VPSS_FATAL("VPSS WBC Alloc memory failed.\n");
	return HI_FAILURE;
    }
    memset((HI_U8 *)psttWbc->stMMUBuf.pu8StartVirAddr, 0, VPSS_STTWBC_SIZE);
    psttWbc->bInit = HI_TRUE;
    psttWbc->u32Cnt = 0;
    return HI_SUCCESS;
}

HI_S32 VPSS_STTINFO_SttWbcDeInit(VPSS_STTWBC_S *psttWbc)
{
    if (HI_NULL == psttWbc)
    {
	VPSS_ERROR("Vpss sttwbc DeInit error(null pointer).\n");
	return HI_FAILURE;
    }

    if (HI_FALSE == psttWbc->bInit)
    {
	VPSS_WARN("Vpss sttwbc DeInit error(not init).\n");
	return HI_FAILURE;
    }

    if (0 != psttWbc->stMMUBuf.u32StartSmmuAddr)
    {
	HI_DRV_SMMU_UnmapAndRelease(&(psttWbc->stMMUBuf));
    }
    memset(psttWbc, 0, sizeof(VPSS_STTWBC_S));
    return HI_SUCCESS;
}

HI_S32 VPSS_STTINFO_SttWbcGetAddr(VPSS_STTWBC_S *psttWbc,
				  HI_U32 *pu32stt_w_phy_addr,
				  HI_U8 **ppu8stt_w_vir_addr)
{
    if ((HI_NULL == psttWbc)
	|| (HI_NULL == pu32stt_w_phy_addr)
	|| (HI_NULL == ppu8stt_w_vir_addr))
    {
	VPSS_ERROR("Vpss sttwbc getcfg error(null pointer).\n");
	return HI_FAILURE;
    }

    if (HI_FALSE == psttWbc->bInit)
    {
	VPSS_ERROR("Vpss sttwbc getcfg error(not init).\n");
    }

    *pu32stt_w_phy_addr = psttWbc->stMMUBuf.u32StartSmmuAddr;
    *ppu8stt_w_vir_addr = psttWbc->stMMUBuf.pu8StartVirAddr;

    return HI_SUCCESS;
}


HI_S32 VPSS_STTINFO_SttWbcComplete(VPSS_STTWBC_S *psttWbc)
{
    if (HI_NULL == psttWbc)
    {
	VPSS_ERROR("Vpss sttwbc Complete error(null pointer).\n");
	return HI_FAILURE;
    }

    if (HI_FALSE == psttWbc->bInit)
    {
	VPSS_ERROR("Vpss sttwbc Complete error(not init).\n");
    }
    // keep interface
    psttWbc->u32Cnt++;
    return HI_SUCCESS;
}

HI_S32 VPSS_STTINFO_SttWbcReset(VPSS_STTWBC_S *psttWbc)
{
    if (HI_NULL == psttWbc)
    {
	VPSS_WARN("Vpss sttwbc Complete error(null pointer).\n");
	return HI_FAILURE;
    }

    if (HI_FALSE == psttWbc->bInit)
    {
	VPSS_WARN("Vpss sttwbc Complete error(not init).\n");
	return HI_FAILURE;
    }

    psttWbc->u32Cnt = 0;

    if (0 != psttWbc->stMMUBuf.pu8StartVirAddr)
    {
	memset((HI_U8 *)psttWbc->stMMUBuf.pu8StartVirAddr, 0, VPSS_STTWBC_SIZE);
    }

    return HI_SUCCESS;

}
