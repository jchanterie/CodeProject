// EdgeBarDialog.cpp : implementation file
//

#include "stdafx.h"
#include "AddInDemo.h"
#include "EdgeBarDialog.h"
#include "afxdialogex.h"


// CEdgeBarDialog dialog

IMPLEMENT_DYNAMIC(CEdgeBarDialog, CDialogEx)

CEdgeBarDialog::CEdgeBarDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEdgeBarDialog::IDD, pParent)
{

}

CEdgeBarDialog::~CEdgeBarDialog()
{
}

void CEdgeBarDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listView);
}


BEGIN_MESSAGE_MAP(CEdgeBarDialog, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CEdgeBarDialog message handlers


void CEdgeBarDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	RECT rect;
	this->GetClientRect(&rect);

	// Resize CListCtrl.
	if (m_listView.m_hWnd != NULL)
	{
		m_listView.MoveWindow(&rect, TRUE);
	}
}
