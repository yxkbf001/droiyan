// CELL.cpp: implementation of the CELL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "server.h"
#include "CELL.h"

#include "Extern.h"
#include "USER.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

POINT g_add[8][5] = 
{
	{{-2, -2}, {-1, -2}, {0, -2}, {-2, -1}, {-2, 0}},	// 1
	{{-1, -2}, {0, -2}, {1, -2}, {100, 0}, {100, 0}},	// 2
	{{0, -2}, {1, -2}, {2, -2}, {2, -1}, {2, 0}},		// 3
	{{-2, -1}, {-2, 0}, {-2, 1}, {100, 0}, {100, 0}},	// 4
	{{2, -1}, {2, 0}, {2, 1}, {100, 0}, {100, 0}},		// 5
	{{-2, 0}, {-2, 1}, {-2, 2}, {-1, 2}, {0, 2}},		// 6
	{{-1, 2}, {0, 2}, {1, 2}, {100, 0}, {100, 0}},		// 7
	{{2, 0}, {2, 1}, {2, 2}, {1, 2}, {0, 2}},			// 8
};

POINT g_del[8][5] = 
{
	{{1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}},			// 1
	{{-1, 1}, {0, 1}, {1, 1}, {100, 0}, {100, 0}},		// 2
	{{-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}},		// 3
	{{1, -1}, {1, 0}, {1, 1}, {100, 0}, {100, 0}},		// 4
	{{-1, -1}, {-1, 0}, {-1, 1}, {100, 0}, {100, 0}},	// 5
	{{-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}},		// 6
	{{-1, -1}, {0, -1}, {1, -1}, {100, 0}, {100, 0}},	// 7
	{{-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {-1, 1}},		// 8
};

//////////////////////////////////////////////////////////////////////
// CellInfo 
//////////////////////////////////////////////////////////////////////
CellInfo::CellInfo()
{
	::InitializeCriticalSection(&m_cs);
	::InitializeCriticalSection(&m_Itemcs);

	//::InitializeCriticalSectionAndSpinCount(&m_cs, SPIN_COUNT);
	//::InitializeCriticalSectionAndSpinCount(&m_Itemcs, SPIN_COUNT);
}

CellInfo::~CellInfo()
{
	m_arCell.RemoveAll();
	m_arItemCell.RemoveAll();

	::DeleteCriticalSection(&m_cs);
	::DeleteCriticalSection(&m_Itemcs);
}

/////////////////////////////////////////////////////////////////////
//	Cell Array �� USER(NPC) ID �� �ִ´�.
//
void CellInfo::Add(int nUid)
{
	::EnterCriticalSection(&m_cs);
	m_arCell.Add(nUid);
	::LeaveCriticalSection(&m_cs);
}

/////////////////////////////////////////////////////////////////////
//	Cell Array ���� USER(NPC) ID �� �����Ѵ�.
//
void CellInfo::Delete(int nUid)
{
	::EnterCriticalSection(&m_cs);
	for(int i = 0; i < m_arCell.GetSize(); i++)
	{
		if(m_arCell[i] == nUid)	
		{
			m_arCell.RemoveAt(i);	// Ȥ�ö� ���� break ���� �ȳ�
			// break;
		}
	}
	::LeaveCriticalSection(&m_cs);
}

/////////////////////////////////////////////////////////////////////
//	Cell Array �� copy �� �ش�.
//
void CellInfo::Get(CArray<int, int>& arCopy)
{
	::EnterCriticalSection(&m_cs);
	arCopy.RemoveAll();
	for(int i = 0; i < m_arCell.GetSize(); i++) arCopy.Add(m_arCell.GetAt(i));
	::LeaveCriticalSection(&m_cs);
}


void CellInfo::DeleteItem(CPoint nUid)
{
	::EnterCriticalSection(&m_Itemcs);

	for(int i = 0; i < m_arItemCell.GetSize(); i++)
	{
		if(m_arItemCell[i].x == nUid.x && m_arItemCell[i].y == nUid.y)	
		{
			m_arItemCell.RemoveAt(i);
			// break;
		}
	}

	::LeaveCriticalSection(&m_Itemcs);
}

void CellInfo::AddItem(CPoint nUid)
{
	::EnterCriticalSection(&m_Itemcs);
	m_arItemCell.Add(nUid);
	::LeaveCriticalSection(&m_Itemcs);
}

void CellInfo::GetItem(CArray<CPoint,CPoint>& arItemCopy)
{
	::EnterCriticalSection(&m_Itemcs);
	arItemCopy.RemoveAll();
	for(int i = 0; i < m_arItemCell.GetSize(); i++) arItemCopy.Add(m_arItemCell.GetAt(i));
	::LeaveCriticalSection(&m_Itemcs);
}

//////////////////////////////////////////////////////////////////////
// CELL 
//////////////////////////////////////////////////////////////////////
CELL::CELL(CSize size, int nZone, COM *pCom)
{
	m_pCom = pCom;
	m_sizeMap = size;
	
	int nX = size.cx / CELL_XSIZE;
	int nY = size.cy / CELL_YSIZE;
	
	if((nX * CELL_XSIZE) != size.cx) nX++;
	if((nY * CELL_YSIZE) != size.cy) nY++;
	
	m_sizeCell.cx = nX;
	m_sizeCell.cy = nY;
	
	m_Zone = nZone;
	
	m_pCell = new CellInfo* [m_sizeCell.cx];
	for(int i = 0; i < m_sizeCell.cx; i++) 
	{
		m_pCell[i] = new CellInfo[m_sizeCell.cy];
	}

}

CELL::~CELL()
{
	RemoveCellData();
}

///////////////////////////////////////////////////////////////////////
//	�޸� ����
//
void CELL::RemoveCellData()
{
	if(m_pCell)
	{
		for(int i = 0; i < m_sizeCell.cx; i++)
		{
			delete[] m_pCell[i];
		}
		
		delete[] m_pCell;
	}
}

////////////////////////////////////////////////////////////////////////
//	���� ���� ��ġ�� ���Ѵ�.
//
CPoint CELL::GetCellIndex(int xpos, int ypos)
{
	if(xpos > m_sizeMap.cx || xpos < 0) 
	{
		return CPoint(-1, -1);
	}
	if(ypos > m_sizeMap.cy || ypos < 0) 
	{
		return CPoint(-1, -1);
	}
	
	int x, y;
	int sx, sy, ex, ey;
	CRect rect;
	CPoint pt(xpos, ypos);
	CPoint ptReturn;
	
	for(y = 0; y < m_sizeCell.cy; y++)
	{
		sy = y * CELL_YSIZE;
		ey = sy + CELL_YSIZE;
		
		for(x = 0; x < m_sizeCell.cx; x++)
		{
			sx = x * CELL_XSIZE;
			ex = sx + CELL_XSIZE;
			
			rect.SetRect(sx, sy, ex, ey);
			
			if(rect.PtInRect(pt))
			{ 
				ptReturn.x = x;
				ptReturn.y = y;
				
				return ptReturn;
			}
		}
	}
	return CPoint(-1, -1);
}

////////////////////////////////////////////////////////////////////////
//	������ �̵��� Cell ������ ���Ѵ�.
//
//	1 2 3
//	4   5
//	6 7 8
int CELL::GetDirection(int sx, int sy, int ex, int ey)
{
	int dx = ex - sx;
	int dy = ey - sy;
	
	if(dx == -1 && dy == -1) return 1;
	if(dx ==  0 && dy == -1) return 2;
	if(dx ==  1 && dy == -1) return 3;
	if(dx == -1 && dy ==  0) return 4;
	if(dx ==  1 && dy ==  0) return 5;
	if(dx == -1 && dy ==  1) return 6;
	if(dx ==  0 && dy ==  1) return 7;
	if(dx ==  1 && dy ==  1) return 8;
	
	return -1;	// return ERROR
}

////////////////////////////////////////////////////////////////////////
//	������ �̵��� Cell ������ ���Ѵ�.
//
int CELL::GetDirection(CPoint ptOld, CPoint ptNew)
{
	int dx = ptNew.x - ptOld.x;
	int dy = ptNew.y - ptOld.y;

	if(dx == -1 && dy == -1) return 1;
	if(dx ==  0 && dy == -1) return 2;
	if(dx ==  1 && dy == -1) return 3;
	if(dx == -1 && dy ==  0) return 4;
	if(dx ==  1 && dy ==  0) return 5;
	if(dx == -1 && dy ==  1) return 6;
	if(dx ==  0 && dy ==  1) return 7;
	if(dx ==  1 && dy ==  1) return 8;
	
	return -1;	// return ERROR
}

////////////////////////////////////////////////////////////////////////
//	Cell �� ���ο� ������ NPC�� �߰��Ѵ�.
//
void CELL::AddCell(CPoint pt, int nUid)
{
	// Add User ID to Cell and Copy Cell Array
	CCellItemArray arCellItem; 
	CCellUidArray arUid;
	m_pCell[pt.x][pt.y].Add(nUid);				

	ItemInfoArray arMapItem; 
	arMapItem.RemoveAll();
	
	USER* pUser		= NULL;
	USER* pSendUser = NULL;
	CNpc* pNpc		= NULL;
	int nSize = 0, iSize = 0;
	int i, nX, nY;
	int cx, cy;

	DWORD dwDN = 0;
	
	if(nUid >= USER_BAND && nUid < NPC_BAND)
	{
		CPoint ptCell;

		pUser = m_pCom->GetUserUid(nUid - USER_BAND);
		if(pUser == NULL || pUser->m_curz != m_Zone) return;
		
		for(nX = -1; nX <= 1; nX++)
		{
			for(nY = -1; nY <= 1; nY++)
			{
				cx = pt.x + nX;
				cy = pt.y + nY;
				if(cx < 0 || cx >= m_sizeCell.cx) continue;
				if(cy < 0 || cy >= m_sizeCell.cy) continue;

				m_pCell[cx][cy].Get(arUid);
				nSize = arUid.GetSize();
				
				m_pCell[cx][cy].GetItem(arCellItem);					// �ʻ󿡼��� ������ ��ġ��...
				iSize = arCellItem.GetSize();							// �� �������� ������ ��ġ ��

				for(i = 0; i < iSize; i++)								// ��� ������ ��ġ������ ������.
				{
					dwDN = 0;
					ptCell = arCellItem[i];
					if(!GetViewMapItemList(arMapItem, dwDN, ptCell, pUser->m_ZoneIndex)) continue;

					if(dwDN > 0) pUser->GetMapMoneyInfo(dwDN, ptCell);
//					if(arMapItem.GetSize() > 0) pUser->GetMapItemInfo(arMapItem, ptCell, -1, ITEM_INFO_MODIFY);
					arMapItem.RemoveAll();
				}

				for(i = 0; i < nSize; i++)
				{
					nUid = arUid.GetAt(i);
					if(nUid >= USER_BAND && nUid < NPC_BAND)
					{
						pSendUser = m_pCom->GetUserUid(nUid - USER_BAND);		
						if(pSendUser == NULL || pSendUser->m_state != STATE_GAMESTARTED || pSendUser->m_curz != m_Zone) continue;
						
						pSendUser->SendUserInfo(pUser, INFO_MODIFY);	// Other <- Me
						pUser->SendUserInfo(pSendUser, INFO_MODIFY);	// Other -> Me
					}
					if(nUid >= NPC_BAND)
					{
						pNpc = pUser->GetNpc(nUid - NPC_BAND);
						if(!pNpc) continue;
						pUser->SendNpcInfo(pNpc, INFO_MODIFY);
					}
				}
			}
		}
	}
	else if(nUid >= NPC_BAND)
	{
		pNpc = GetNpc(nUid - NPC_BAND);
		if(pNpc == NULL) return;

		for(nX = -1; nX <= 1; nX++)
		{
			for(nY = -1; nY <= 1; nY++)
			{
				cx = pt.x + nX;
				cy = pt.y + nY;
				if(cx < 0 || cx >= m_sizeCell.cx) continue;
				if(cy < 0 || cy >= m_sizeCell.cy) continue;

				m_pCell[cx][cy].Get(arUid);
				nSize = arUid.GetSize();
				
				for(i = 0; i < nSize; i++)
				{
					nUid = arUid.GetAt(i);
					if(nUid >= USER_BAND && nUid < NPC_BAND)
					{
						pSendUser = m_pCom->GetUserUid(nUid - USER_BAND);		
						if(pSendUser == NULL || pSendUser->m_state != STATE_GAMESTARTED || pSendUser->m_curz != m_Zone) continue;
						
						pSendUser->SendNpcInfo(pNpc, INFO_MODIFY);
//						TRACE("Add Cell NPC %d\n", pNpc->m_sNid);
					}
				}
			}
		}
	}
	
	arMapItem.RemoveAll();
	arUid.RemoveAll();
}

////////////////////////////////////////////////////////////////////////
//	Cell ���� ������ NPC�� �����Ѵ�.
//
void CELL::DeleteCell(CPoint pt, int nUid)
{
	// Add User ID to Cell and Copy Cell Array
	CCellItemArray arCellItem; 
	ItemInfoArray arMapItem; 
	arMapItem.RemoveAll();

	CCellUidArray arUid;
	m_pCell[pt.x][pt.y].Delete(nUid);				
	
	USER* pUser		= NULL;
	USER* pSendUser = NULL;
	CNpc* pNpc		= NULL;
	int nSize = 0, iSize = 0;
	int i, nX, nY;
	int cx, cy;

	BOOL bZoneChanging = FALSE;

	DWORD dwDN = 0;
	
	if(nUid >= USER_BAND && nUid < NPC_BAND)
	{
		CPoint ptCell;

		pUser = m_pCom->GetUserUid(nUid - USER_BAND);
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED || pUser->m_curz != m_Zone) return;

		if(pUser->m_NowZoneChanging) bZoneChanging = TRUE;
		
		for(nX = -1; nX <= 1; nX++)
		{
			for(nY = -1; nY <= 1; nY++)
			{
				cx = pt.x + nX;
				cy = pt.y + nY;
				if(cx < 0 || cx >= m_sizeCell.cx) continue;
				if(cy < 0 || cy >= m_sizeCell.cy) continue;

				m_pCell[cx][cy].Get(arUid);
				nSize = arUid.GetSize();

				m_pCell[cx][cy].GetItem(arCellItem);					// �ʻ󿡼��� ������ ��ġ��...
				iSize = arCellItem.GetSize();							// �� �������� ������ ��ġ ��

				if(!bZoneChanging)
				{
					for(i = 0; i < iSize; i++)								// ��� ������ ��ġ������ ������.
					{
						dwDN = 0;
						ptCell = arCellItem[i];
						if(!GetViewMapItemList(arMapItem, dwDN, ptCell, pUser->m_ZoneIndex)) continue;

						if(dwDN > 0) pUser->GetMapMoneyInfo(dwDN, ptCell);
//						if(arMapItem.GetSize() > 0) pUser->GetMapItemInfo(arMapItem, ptCell, -1, ITEM_INFO_DEL);
						arMapItem.RemoveAll();
					}
				}

				for(i = 0; i < nSize; i++)
				{
					nUid = arUid.GetAt(i);
					if(nUid >= USER_BAND && nUid < NPC_BAND)
					{
						pSendUser = m_pCom->GetUserUid(nUid - USER_BAND);		
						if(pSendUser == NULL || pSendUser->m_state != STATE_GAMESTARTED || pSendUser->m_curz != m_Zone) continue;
						
						pSendUser->SendUserInfo(pUser, INFO_DELETE);	// Other <- Me
						if(!bZoneChanging) pUser->SendUserInfo(pSendUser, INFO_DELETE);	// Other -> Me
					}
					if(nUid >= NPC_BAND)
					{
						pNpc = pUser->GetNpc(nUid - NPC_BAND);
						if(!pNpc) continue;
						if(!bZoneChanging) pUser->SendNpcInfo(pNpc, INFO_DELETE);  // ��ü������ �������� ������.
					}
				}
			}
		}
	} 
	else if(nUid >= NPC_BAND)
	{
		pNpc = GetNpc(nUid - NPC_BAND);
		if(pNpc == NULL) return;

		for(nX = -1; nX <= 1; nX++)
		{
			for(nY = -1; nY <= 1; nY++)
			{
				cx = pt.x + nX;
				cy = pt.y + nY;
				if(cx < 0 || cx >= m_sizeCell.cx) continue;
				if(cy < 0 || cy >= m_sizeCell.cy) continue;

				m_pCell[cx][cy].Get(arUid);
				nSize = arUid.GetSize();
				
				for(i = 0; i < nSize; i++)
				{
					nUid = arUid.GetAt(i);
					if(nUid >= USER_BAND && nUid < NPC_BAND)
					{
						pSendUser = m_pCom->GetUserUid(nUid - USER_BAND);		
						if(pSendUser == NULL || pSendUser->m_state != STATE_GAMESTARTED || pSendUser->m_curz != m_Zone) continue;
						
						pSendUser->SendNpcInfo(pNpc, INFO_DELETE);
//						TRACE("Delete Cell\n");
					}
				}
			}
		}
	}
	
	arUid.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////
//	User �� NPC �� �þ�(Cell)�� ������� ����� ó��
//
void CELL::ChangeCell(CPoint ptOld, CPoint ptNew, int nUid)
{
	CCellItemArray arCellItem; 
	ItemInfoArray arMapItem; 
	arMapItem.RemoveAll();

	// Delete User ID at Old Cell
	m_pCell[ptOld.x][ptOld.y].Delete(nUid);
				
	// Add User ID to New Cell
	m_pCell[ptNew.x][ptNew.y].Add(nUid);
	
	// Get Direction
	int nDirection = GetDirection(ptOld.x, ptOld.y, ptNew.x, ptNew.y);
	ASSERT(nDirection >= 1 && nDirection <= 8);
	
	CCellUidArray arUid;
	USER* pUser		= NULL;
	USER* pSendUser	= NULL;
	CNpc* pNpc		= NULL;
	int nSize = 0, iSize = 0;
	int i, j;
	int cx, cy;

	DWORD dwDN = 0;
	CPoint ptCell;
	BOOL bZoneChanging = FALSE;

	if(nUid >= USER_BAND && nUid < NPC_BAND)
	{
		pUser = m_pCom->GetUserUid(nUid - USER_BAND);
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED || pUser->m_curz != m_Zone) return;
		
		int nD = nDirection - 1;
		int xx = 0, yy = 0;
		
		for(i = 0; i < 5; i++)
		{			
			if(g_add[nD][i].x == 100) break;
			
			// Add ...
			xx = g_add[nD][i].x;
			yy = g_add[nD][i].y;

			cx = ptOld.x + xx;
			cy = ptOld.y + yy;
			if(cx < 0 || cx >= m_sizeCell.cx) continue;
			if(cy < 0 || cy >= m_sizeCell.cy) continue;
			
			m_pCell[cx][cy].GetItem(arCellItem);					// �ʻ󿡼��� ������ ��ġ��...
			iSize = arCellItem.GetSize();							// �� �������� ������ ��ġ ��

			for(j = 0; j < iSize; j++)								// ��� ������ ��ġ������ ������.
			{
				dwDN = 0;
				ptCell = arCellItem[j];
				if(!GetViewMapItemList(arMapItem, dwDN, ptCell, pUser->m_ZoneIndex)) continue;

				if(dwDN > 0) pUser->GetMapMoneyInfo(dwDN, ptCell);
//				if(arMapItem.GetSize() > 0) pUser->GetMapItemInfo(arMapItem, ptCell, -1, ITEM_INFO_MODIFY);
				arMapItem.RemoveAll();
			}

			m_pCell[cx][cy].Get(arUid);
			nSize = arUid.GetSize();

			for(j = 0; j < nSize; j++)
			{
				nUid = arUid.GetAt(j);
				if(nUid >= USER_BAND && nUid < NPC_BAND)
				{
					pSendUser = m_pCom->GetUserUid(nUid - USER_BAND);		
					if(pSendUser == NULL || pSendUser->m_state != STATE_GAMESTARTED || pSendUser->m_curz != m_Zone) continue;
					
					pSendUser->SendUserInfo(pUser, INFO_MODIFY);	// Other <- Me
					pUser->SendUserInfo(pSendUser, INFO_MODIFY);	// Other -> Me
				}
				if(nUid >= NPC_BAND)
				{
					pNpc = pUser->GetNpc(nUid - NPC_BAND);
					if(!pNpc) continue;
					pUser->SendNpcInfo(pNpc, INFO_MODIFY);
				}
			}
		}

		if(pUser->m_NowZoneChanging) bZoneChanging = TRUE;

		for(i = 0; i < 5; i++)
		{
			if(g_add[nD][i].x == 100) break;
			
			// Del ...
			xx = g_del[nD][i].x;
			yy = g_del[nD][i].y;
			
			cx = ptOld.x + xx;
			cy = ptOld.y + yy;
			if(cx < 0 || cx >= m_sizeCell.cx) continue;
			if(cy < 0 || cy >= m_sizeCell.cy) continue;
			
			m_pCell[cx][cy].GetItem(arCellItem);					// �ʻ󿡼��� ������ ��ġ��...
			iSize = arCellItem.GetSize();							// �� �������� ������ ��ġ ��

			if(!bZoneChanging)
			{
				for(j = 0; j < iSize; j++)							// ��� ������ ��ġ������ ������.
				{
					dwDN = 0;
					ptCell = arCellItem[j];
					if(!GetViewMapItemList(arMapItem, dwDN, ptCell, pUser->m_ZoneIndex)) continue;

					if(dwDN > 0) pUser->GetMapMoneyInfo(dwDN, ptCell);
//					if(arMapItem.GetSize() > 0) pUser->GetMapItemInfo(arMapItem, ptCell, -1, ITEM_INFO_DEL);
					arMapItem.RemoveAll();
				}
			}

			m_pCell[cx][cy].Get(arUid);
			nSize = arUid.GetSize();
			
			for(j = 0; j < nSize; j++)
			{
				nUid = arUid.GetAt(j);
				if(nUid >= USER_BAND && nUid < NPC_BAND)
				{
					pSendUser = m_pCom->GetUserUid(nUid - USER_BAND);		
					if(pSendUser == NULL || pSendUser->m_state != STATE_GAMESTARTED || pSendUser->m_curz != m_Zone) continue;
					
					pSendUser->SendUserInfo(pUser, INFO_DELETE);	// Other <- Me
					pUser->SendUserInfo(pSendUser, INFO_DELETE);	// Other -> Me
				}
				if(nUid >= NPC_BAND)
				{
					pNpc = pUser->GetNpc(nUid - NPC_BAND);
					if(!pNpc) continue;
					pUser->SendNpcInfo(pNpc, INFO_DELETE);
				}
			}
		}
	}
	else if(nUid >= NPC_BAND)
	{
		pNpc = GetNpc(nUid - NPC_BAND);
		if(pNpc == NULL) return;
		
		int nD = nDirection - 1;
		int xx = 0, yy = 0;
		
		for(i = 0; i < 5; i++)
		{
			if(g_add[nD][i].x == 100) break;
			
			// Add ...
			xx = g_add[nD][i].x;
			yy = g_add[nD][i].y;
			
			cx = ptOld.x + xx;
			cy = ptOld.y + yy;
			if(cx < 0 || cx >= m_sizeCell.cx) continue;
			if(cy < 0 || cy >= m_sizeCell.cy) continue;
			
			m_pCell[cx][cy].Get(arUid);
			nSize = arUid.GetSize();
			
			for(j = 0; j < nSize; j++)
			{
				nUid = arUid.GetAt(j);
				if(nUid >= USER_BAND && nUid < NPC_BAND)
				{
					pSendUser = m_pCom->GetUserUid(nUid - USER_BAND);		
					if(pSendUser == NULL || pSendUser->m_state != STATE_GAMESTARTED || pSendUser->m_curz != m_Zone) continue;
					
					pSendUser->SendNpcInfo(pNpc, INFO_MODIFY);
				}
			}
		}
			
		for(i = 0; i < 5; i++)
		{
			if(g_add[nD][i].x == 100) break;
		
			// Del ...
			xx = g_del[nD][i].x;
			yy = g_del[nD][i].y;
			
			cx = ptOld.x + xx;
			cy = ptOld.y + yy;
			if(cx < 0 || cx >= m_sizeCell.cx) continue;
			if(cy < 0 || cy >= m_sizeCell.cy) continue;
			
			m_pCell[cx][cy].Get(arUid);
			nSize = arUid.GetSize();
			
			for(j = 0; j < nSize; j++)
			{
				nUid = arUid.GetAt(j);
				if(nUid >= USER_BAND && nUid < NPC_BAND)
				{
					pSendUser = m_pCom->GetUserUid(nUid - USER_BAND);		
					if(pSendUser == NULL || pSendUser->m_state != STATE_GAMESTARTED || pSendUser->m_curz != m_Zone) continue;
					
					pSendUser->SendNpcInfo(pNpc, INFO_DELETE);
				}
			}
		}
	}

	arUid.RemoveAll();
}

////////////////////////////////////////////////////////////////////////
//	Cell�� �ʿ����� ������ ������ ��� �������� �˸���.
//
void CELL::DeleteItemCell(CPoint pt, CPoint MapPt) // ����ǥ, ����ǥ
{
/*	CCellUidArray	arUid;
	ItemListArray	arItem;

	USER* pSendUser = NULL;
	USER* pUser		= NULL;

	CPoint iCurpt;

	int nSize = 0;
	int i, nX, nY;
	int cx, cy;
	int nUid = 0;

	for(nX = -1; nX <= 1; nX++)							// 9�� ���̿� �ִ� �������� ��������
	{
		for(nY = -1; nY <= 1; nY++)
		{
			cx = pt.x + nX;
			cy = pt.y + nY;
			if(cx < 0 || cx >= m_sizeCell.cx) continue;
			if(cy < 0 || cy >= m_sizeCell.cy) continue;

			m_pCell[cx][cy].Get(arUid);
			nSize = arUid.GetSize();
				
			for(i = 0; i < nSize; i++)
			{
				nUid = arUid.GetAt(i);
				if(nUid >= USER_BAND && nUid < NPC_BAND)
				{
					pSendUser = m_pCom->user_array[nUid - USER_BAND];		
					if(pSendUser == NULL || pSendUser->m_state != STATE_GAMESTARTED || pSendUser->m_curz != m_Zone) continue;
					
					pSendUser->GetMapItemInfo(arItem, FALSE, MapPt, ITEM_INFO_ALL_DELETE);
				}
			}
		}
	}
	
	arItem.RemoveAll();
	arUid.RemoveAll();
*/
}

////////////////////////////////////////////////////////////////////////
//	Cell�� �ʿ����� ������ ��ǥ����.
//
void CELL::DeleteCellCoordinate(CPoint ptCell, CPoint pt)
{
	m_pCell[ptCell.x][ptCell.y].DeleteItem(pt);
}

////////////////////////////////////////////////////////////////////////
//	Cell�� �ʿ����� ������ ��ȭ�� ��� �������� �˸���.
//
void CELL::ChangeItemCell(ItemListArray &arItem, CPoint pt, CPoint ptMap, int uid)//(����ǥ, ����ǥ)
{
	CCellUidArray	arUid;
	USER* pSendUser = NULL;
//	USER* pUser		= NULL;

	int nSize = 0;
	int i, nX, nY;
	int cx, cy;
	int nUid = 0;

	for(nX = -1; nX <= 1; nX++)							// 9�� ���̿� �ִ� �������� ��������
	{
		for(nY = -1; nY <= 1; nY++)
		{
			cx = pt.x + nX;
			cy = pt.y + nY;
			if(cx < 0 || cx >= m_sizeCell.cx) continue;
			if(cy < 0 || cy >= m_sizeCell.cy) continue;

			m_pCell[cx][cy].Get(arUid);
			nSize = arUid.GetSize();
				
			for(i = 0; i < nSize; i++)
			{
				nUid = arUid.GetAt(i);
				if(nUid >= USER_BAND && nUid < NPC_BAND)
				{
					pSendUser = m_pCom->GetUserUid(nUid - USER_BAND);		
					if(pSendUser == NULL || pSendUser->m_state != STATE_GAMESTARTED || pSendUser->m_curz != m_Zone) continue;

//					pSendUser->GetMapItemInfo(arItem, ptMap, uid, ITEM_PICK_UP);
//					TRACE("ChangeItemCell --> ID : %s, x : %d, y : %d\n", pSendUser->m_strUserID, ptMap.x, ptMap.y);
				}
			}
		}
	}
	
	arUid.RemoveAll();
}

////////////////////////////////////////////////////////////////////////
//	������ �ݴ� ���۸� �Ͼ��.
//
void CELL::ChangeMoneyCell(DWORD dwDN, CPoint pt, CPoint ptMap, int uid)
{
	CCellUidArray	arUid;

	USER* pSendUser = NULL;
//	USER* pUser		= NULL;

	int nSize = 0;
	int i, nX, nY;
	int cx, cy;
	int nUid = 0;

	for(nX = -1; nX <= 1; nX++)							// 9�� ���̿� �ִ� �������� ��������
	{
		for(nY = -1; nY <= 1; nY++)
		{
			cx = pt.x + nX;
			cy = pt.y + nY;
			if(cx < 0 || cx >= m_sizeCell.cx) continue;
			if(cy < 0 || cy >= m_sizeCell.cy) continue;

			m_pCell[cx][cy].Get(arUid);
			nSize = arUid.GetSize();
				
			for(i = 0; i < nSize; i++)
			{
				nUid = arUid.GetAt(i);
				if(nUid >= USER_BAND && nUid < NPC_BAND)
				{
					pSendUser = m_pCom->GetUserUid(nUid - USER_BAND);		
					if(pSendUser == NULL || pSendUser->m_state != STATE_GAMESTARTED || pSendUser->m_curz != m_Zone) continue;

					pSendUser->GetMapMoneyInfo(dwDN, ptMap, uid);
				}
			}
		}
	}
	
	arUid.RemoveAll();
}

////////////////////////////////////////////////////////////////////////
//	Cell�� �ʿ����� ������ ��ǥ�� ����ϰ� ��� �������� �˸���.
//
void CELL::AddItemCell(int uid, CPoint itemPt, int isReg)//(����, ����ǥ, ����㰡:01:��02:������)
{														 //(����, ����ǥ, ������:11:��12:������)				
	DWORD dwDN = 0;
	int nSize = 0, iSize = 0;

	CPoint pt;
	CCellUidArray	arUid;
	ItemInfoArray	arMapItem; 

	arMapItem.RemoveAll();
	USER* pSendUser = NULL;
	USER* pUser		= NULL;
	CNpc* pNpc		= NULL;

	int i, nX, nY;
	int cx, cy;
	int nUid = 0;

	if(uid >= USER_BAND && uid < NPC_BAND)
	{
		pUser = m_pCom->GetUserUid(uid - USER_BAND);
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED || pUser->m_curz != m_Zone) goto go_result;
		pt.x = pUser->m_ptCell.x;
		pt.y = pUser->m_ptCell.y;

		if(isReg < 10) m_pCell[pt.x][pt.y].AddItem(itemPt);									// ���� ���

		if(!GetMapItemList(arMapItem, dwDN, itemPt, pUser->m_ZoneIndex)) goto go_result;		// �ش� �ʿ��� ������ ����Ʈ�� ���´�.

		for(nX = -1; nX <= 1; nX++)							// 9�� ���̿� �ִ� �������� ��������
		{
			for(nY = -1; nY <= 1; nY++)
			{
				cx = pt.x + nX;
				cy = pt.y + nY;
				if(cx < 0 || cx >= m_sizeCell.cx) continue;
				if(cy < 0 || cy >= m_sizeCell.cy) continue;

				m_pCell[cx][cy].Get(arUid);
				nSize = arUid.GetSize();
					
				for(i = 0; i < nSize; i++)
				{
					nUid = arUid.GetAt(i);
					if(nUid >= USER_BAND && nUid < NPC_BAND)
					{
						pSendUser = m_pCom->GetUserUid(nUid - USER_BAND);		
						if(pSendUser == NULL || pSendUser->m_state != STATE_GAMESTARTED || pSendUser->m_curz != m_Zone) continue;

//						if(isReg%2 == 0) pSendUser->GetMapItemInfo(arMapItem, itemPt, -1, ITEM_INFO_ADD);
//						else pSendUser->GetMapMoneyInfo(dwDN, itemPt);
					}
				}
			}
		}
	}
	else if(uid >= NPC_BAND)
	{
		pNpc = GetNpc(uid - NPC_BAND);
		if(pNpc == NULL) return;
		pt.x = pNpc->m_ptCell.x;
		pt.y = pNpc->m_ptCell.y;

		if(isReg < 10) m_pCell[pt.x][pt.y].AddItem(itemPt);									// ���� ���

		if(!GetMapItemList(arMapItem, dwDN, itemPt, pNpc->m_ZoneIndex))  goto go_result;		// �ش� �ʿ��� ������ ����Ʈ�� ���´�.

		for(nX = -1; nX <= 1; nX++)
		{
			for(nY = -1; nY <= 1; nY++)
			{
				cx = pt.x + nX;
				cy = pt.y + nY;
				if(cx < 0 || cx >= m_sizeCell.cx) continue;
				if(cy < 0 || cy >= m_sizeCell.cy) continue;

				m_pCell[cx][cy].Get(arUid);
				nSize = arUid.GetSize();
				
				for(i = 0; i < nSize; i++)
				{
					nUid = arUid.GetAt(i);
					if(nUid >= USER_BAND && nUid < NPC_BAND)
					{
						pSendUser = m_pCom->GetUserUid(nUid - USER_BAND);		
						if(pSendUser == NULL || pSendUser->m_state != STATE_GAMESTARTED || pSendUser->m_curz != m_Zone) continue;

//						if(isReg%2 == 0) pSendUser->GetMapItemInfo(arMapItem, itemPt, -1, ITEM_INFO_ADD);
//						else pSendUser->GetMapMoneyInfo(dwDN, itemPt);						
//						if(arMapItem.GetSize() > 0) pSendUser->GetMapItemInfo(arMapItem, itemPt, ITEM_INFO_ADD);
//						if(dwDN > 0) pSendUser->GetMapMoneyInfo(dwDN, itemPt);
//						TRACE("Add Item Cell\n");
					}
				}
			}
		}

	}

go_result:
	arUid.RemoveAll();
	arMapItem.RemoveAll();
}


////////////////////////////////////////////////////////////////////////
//	�ʿ��� ������ ����Ʈ�� �����´�.(���� : ���� �ܰ迡�� �� Lock�� �ɷ� �־����)
//
BOOL CELL::GetMapItemList(ItemInfoArray &arMapItem, DWORD &dwDN, CPoint pt, int zoneIndex)
{
	int j = 0;
	int iSize = 0;

	iSize = g_zone[zoneIndex]->m_pMap[pt.x][pt.y].m_Item.GetSize();
	dwDN = g_zone[zoneIndex]->m_pMap[pt.x][pt.y].m_FieldDN;
											//	�ش� �ʿ� �ƹ��͵� ����.				
	if(iSize <= 0 && dwDN <= 0) return FALSE;
											//	�� ������ ���� �ִ�...
	if(iSize > 0) arMapItem.Add(g_zone[zoneIndex]->m_pMap[pt.x][pt.y].m_Item[iSize - 1]);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////
//	�ʿ��� ������ ����Ʈ�� �����´�.(AddCell, DeleteCell�������μ� �ش� ��  �� ������ ���´�.)
//
BOOL CELL::GetViewMapItemList(ItemInfoArray &arMapItem, DWORD &dwDN, CPoint pt, int zoneIndex)
{
	int j = 0;
	int iSize = 0;
	long lUsed = 0;

	lUsed = g_zone[zoneIndex]->m_pMap[pt.x][pt.y].m_FieldUse;
	if(InterlockedCompareExchange((long*)&lUsed, (long)1, (long)0) == (long)0)
	{
		iSize = g_zone[zoneIndex]->m_pMap[pt.x][pt.y].m_Item.GetSize();
		dwDN = g_zone[zoneIndex]->m_pMap[pt.x][pt.y].m_FieldDN;

		if(iSize <= 0 && dwDN <= 0) return FALSE;

		for(j=0; j < iSize; j++) 
		{
			arMapItem.Add(g_zone[zoneIndex]->m_pMap[pt.x][pt.y].m_Item[j]);
		}

		::InterlockedExchange(&g_zone[zoneIndex]->m_pMap[pt.x][pt.y].m_FieldUse, 0); 
		return TRUE;
	}
	else return FALSE;
}

CNpc* CELL::GetNpc(int nid)
{
	CNpc* pNpc = NULL;
	
//	int nSize = g_arNpcThread.GetSize();
	int nSize = g_arNpc.GetSize();

//	if(nid < 0 || nid >= nSize) return NULL;

/*	pNpc = g_arNpcThread[nid]->m_pNpc;
*/
	for( int i = 0; i < g_arNpc.GetSize(); i++)
	{
		pNpc = g_arNpc[i];
		if( !pNpc ) continue;

		if( pNpc->m_sNid == nid )
		{
			return pNpc;
		}
	}

	return NULL;
}


////////////////////////////////////////////////////////////////////////
//	�ѱ�迭������ Ÿ����Ż�� ���� ���� User�� �߰��Ѵ�.(��Ŷ���� ���̱�����)  
//
void CELL::AddCellFromTownPotal(CPoint pt, int nUid)
{
	CCellItemArray arCellItem; 
	CCellUidArray arUid;
	m_pCell[pt.x][pt.y].Add(nUid);				

	ItemInfoArray arMapItem; 
	arMapItem.RemoveAll();
	
	USER* pUser		= NULL;
	USER* pSendUser = NULL;
	CNpc* pNpc		= NULL;
	int nSize = 0, iSize = 0;
	int i, nX, nY;
	int cx, cy;

	DWORD dwDN = 0;
	
	if(nUid >= USER_BAND && nUid < NPC_BAND)
	{
		CPoint ptCell;

		pUser = m_pCom->GetUserUid(nUid - USER_BAND);
		if(pUser == NULL || pUser->m_curz != m_Zone) return;
		
		for(nX = -1; nX <= 1; nX++)
		{
			for(nY = -1; nY <= 1; nY++)
			{
				cx = pt.x + nX;
				cy = pt.y + nY;
				if(cx < 0 || cx >= m_sizeCell.cx) continue;
				if(cy < 0 || cy >= m_sizeCell.cy) continue;

				m_pCell[cx][cy].Get(arUid);
				nSize = arUid.GetSize();
				
				m_pCell[cx][cy].GetItem(arCellItem);					// �ʻ󿡼��� ������ ��ġ��...
				iSize = arCellItem.GetSize();							// �� �������� ������ ��ġ ��

				for(i = 0; i < iSize; i++)								// ��� ������ ��ġ������ ������.
				{
					dwDN = 0;
					ptCell = arCellItem[i];
					if(!GetViewMapItemList(arMapItem, dwDN, ptCell, pUser->m_ZoneIndex)) continue;

					if(dwDN > 0) pUser->GetMapMoneyInfo(dwDN, ptCell);
//					if(arMapItem.GetSize() > 0) pUser->GetMapItemInfo(arMapItem, ptCell, -1, ITEM_INFO_MODIFY);
					arMapItem.RemoveAll();
				}

				for(i = 0; i < nSize; i++)
				{
					nUid = arUid.GetAt(i);
					if(nUid >= USER_BAND && nUid < NPC_BAND)
					{
						pSendUser = m_pCom->GetUserUid(nUid - USER_BAND);		
						if(pSendUser == NULL || pSendUser->m_state != STATE_GAMESTARTED || pSendUser->m_curz != m_Zone) continue;
						
						pSendUser->SendUserTownPotal(pUser, INFO_TOWNPOTAL);// Other <- Me(���� Ÿ����Ż���̴�)
						pUser->SendUserInfo(pSendUser, INFO_MODIFY);	// Other -> Me(�Ϲ� ���� ����)
					}
					if(nUid >= NPC_BAND)
					{
						pNpc = pUser->GetNpc(nUid - NPC_BAND);
						if(!pNpc) continue;
						pUser->SendNpcInfo(pNpc, INFO_MODIFY);
					}
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////
//	�ѱ�迭������ Ÿ����Ż�� ���� ���� User�� �����Ѵ�.  
//
void CELL::DeleteCellFromTownPotal(CPoint pt, int nUid)
{
	// Add User ID to Cell and Copy Cell Array
	CCellItemArray arCellItem; 
	ItemInfoArray arMapItem; 
	arMapItem.RemoveAll();

	CCellUidArray arUid;
	m_pCell[pt.x][pt.y].Delete(nUid);				
	
	USER* pUser		= NULL;
	USER* pSendUser = NULL;
	CNpc* pNpc		= NULL;
	int nSize = 0, iSize = 0;
	int i, nX, nY;
	int cx, cy;

	BOOL bZoneChanging = FALSE;

	DWORD dwDN = 0;
	
	if(nUid >= USER_BAND && nUid < NPC_BAND)
	{
		CPoint ptCell;

		pUser = m_pCom->GetUserUid(nUid - USER_BAND);
		if(pUser == NULL || pUser->m_state != STATE_GAMESTARTED || pUser->m_curz != m_Zone) return;

		if(pUser->m_NowZoneChanging) bZoneChanging = TRUE;
		
		for(nX = -1; nX <= 1; nX++)
		{
			for(nY = -1; nY <= 1; nY++)
			{
				cx = pt.x + nX;
				cy = pt.y + nY;
				if(cx < 0 || cx >= m_sizeCell.cx) continue;
				if(cy < 0 || cy >= m_sizeCell.cy) continue;

				m_pCell[cx][cy].Get(arUid);
				nSize = arUid.GetSize();

				m_pCell[cx][cy].GetItem(arCellItem);					// �ʻ󿡼��� ������ ��ġ��...
				iSize = arCellItem.GetSize();							// �� �������� ������ ��ġ ��

				if(!bZoneChanging)
				{
					for(i = 0; i < iSize; i++)							// ��� ������ ��ġ������ ������.
					{
						dwDN = 0;
						ptCell = arCellItem[i];
						if(!GetViewMapItemList(arMapItem, dwDN, ptCell, pUser->m_ZoneIndex)) continue;

						if(dwDN > 0) pUser->GetMapMoneyInfo(dwDN, ptCell);
//						if(arMapItem.GetSize() > 0) pUser->GetMapItemInfo(arMapItem, ptCell, -1, ITEM_INFO_DEL);
						arMapItem.RemoveAll();
					}
				}

				for(i = 0; i < nSize; i++)
				{
					nUid = arUid.GetAt(i);
					if(nUid >= USER_BAND && nUid < NPC_BAND)
					{
						pSendUser = m_pCom->GetUserUid(nUid - USER_BAND);		
						if(pSendUser == NULL || pSendUser->m_state != STATE_GAMESTARTED || pSendUser->m_curz != m_Zone) continue;
						
						pSendUser->SendUserTownPotal(pUser, INFO_TOWNPOTAL_DELETE);	// Other <- Me
						if(!bZoneChanging) pUser->SendUserInfo(pSendUser, INFO_DELETE);	// Other -> Me
					}
					if(nUid >= NPC_BAND)
					{
						pNpc = pUser->GetNpc(nUid - NPC_BAND);
						if(!pNpc) continue;
						if(!bZoneChanging) pUser->SendNpcInfo(pNpc, INFO_DELETE);  // ��ü������ �������� ������.
					}
				}
			}
		}
	} 

}
