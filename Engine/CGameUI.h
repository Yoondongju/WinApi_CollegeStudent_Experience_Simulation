#pragma once

class CGameUI
{
public:
    ID2D1Factory* m_pFactory;
    HRESULT       m_hr;
    ID2D1HwndRenderTarget* m_pRT;

public:


    CGameUI();
    ~CGameUI();
 
};


