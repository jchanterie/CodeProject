#include "stdafx.h"
#include "EdgeBarController.h"

CEdgeBarController::CEdgeBarController()
{
	m_pAddInEx = NULL;
	m_pAddInEdgeBarEventsObj = NULL;
}

CEdgeBarController::~CEdgeBarController()
{
	m_pAddInEdgeBarEventsObj = NULL;
	m_pAddInEx = NULL;
}

HRESULT CEdgeBarController::SetAddInEx(ISEAddInEx* pAddInEx)
{
	HRESULT hr = S_OK;

	// If SetAddInEx() has been previously called, disconnect from AddInEdgeBarEvents.
	if (m_pAddInEx != NULL)
	{
		if (m_pAddInEdgeBarEventsObj != NULL)
		{
			hr = m_pAddInEdgeBarEventsObj->Disconnect(m_pAddInEx->AddInEdgeBarEvents);
			m_pAddInEdgeBarEventsObj->Release();
			m_pAddInEdgeBarEventsObj = NULL;
		}
	}

	// Assign local variable.
	m_pAddInEx = pAddInEx;

	// If m_pAddInEx is not NULL, connect to AddInEdgeBarEvents.
	if (m_pAddInEx != NULL)
	{
		XAddInEdgeBarEventsObj::CreateInstance(&m_pAddInEdgeBarEventsObj);

		if (m_pAddInEdgeBarEventsObj != NULL)
		{
			m_pAddInEdgeBarEventsObj->AddRef();
			hr = m_pAddInEdgeBarEventsObj->Connect(m_pAddInEx->AddInEdgeBarEvents);
			m_pAddInEdgeBarEventsObj->m_pController = this;
		}
	}

	return S_OK;
}

HRESULT CEdgeBarController::AddEdgeBarPage(SolidEdgeDocument* pDocument)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HRESULT hr = S_OK;
	CEdgeBarDocumentObj* pEdgeBarDocument = NULL;
	ISolidEdgeBarExPtr pEdgeBarEx = NULL;
	
	BOOL bFound = m_pMap.Lookup(pDocument, pEdgeBarDocument);

	if (bFound == FALSE)
	{
		// Get a pointer to the ISolidEdgeBarEx interface.
		pEdgeBarEx = m_pAddInEx;

		CEdgeBarDocumentObj::CreateInstance(&pEdgeBarDocument);
		pEdgeBarDocument->AddRef();
		hr = pEdgeBarDocument->CreateEdgeBarPage(pEdgeBarEx, pDocument);

		m_pMap.SetAt(pDocument, pEdgeBarDocument);
	}

	pEdgeBarEx = NULL;
	return S_OK;
}

HRESULT CEdgeBarController::RemoveEdgeBarPage(SolidEdgeDocument* pDocument)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HRESULT hr = S_OK;
	CEdgeBarDocumentObj* pEdgeBarDocument = NULL;
	ISolidEdgeBarExPtr pEdgeBarEx = NULL;
	ULONG count = 0;

	BOOL bFound = m_pMap.Lookup(pDocument, pEdgeBarDocument);

	if ((bFound == TRUE) && (pEdgeBarDocument != NULL))
	{
		m_pMap.RemoveKey(pDocument);

		// Get a pointer to the ISolidEdgeBarEx interface.
		pEdgeBarEx = m_pAddInEx;

		pEdgeBarDocument->DeleteEdgeBarPage(pEdgeBarEx, pDocument);
		count = pEdgeBarDocument->Release();
	}

	pEdgeBarEx = NULL;
	return hr;
}

HRESULT CEdgeBarController::XAddInEdgeBarEvents::raw_AddPage(LPDISPATCH theDocument)
{
	HRESULT hr = S_OK;
	SolidEdgeDocumentPtr pDocument = theDocument;

	if (pDocument != NULL)
	{
		// Forward call to controller.
		hr = m_pController->AddEdgeBarPage(pDocument);
	}
	else
	{
		hr = E_FAIL;
	}

	pDocument = NULL;
	return hr;
}

HRESULT CEdgeBarController::XAddInEdgeBarEvents::raw_RemovePage(LPDISPATCH theDocument)
{
	HRESULT hr = S_OK;
	SolidEdgeDocumentPtr pDocument = theDocument;

	if (pDocument != NULL)
	{
		// Forward call to controller.
		hr = m_pController->RemoveEdgeBarPage(pDocument);
	}
	else
	{
		hr = E_FAIL;
	}

	pDocument = NULL;
	return hr;
}

HRESULT CEdgeBarController::XAddInEdgeBarEvents::raw_IsPageDisplayable(LPDISPATCH theDocument, BSTR EnvironmentCatID, VARIANT_BOOL * vbIsPageDisplayable)
{
	*vbIsPageDisplayable = VARIANT_TRUE;
	return S_OK;
}