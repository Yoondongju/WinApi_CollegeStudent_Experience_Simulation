#pragma once
#include "CItem.h"
class CMTItem : public CItem
{

public:
	virtual void update();
	virtual void render(HDC _dc);



public:
	void ItemClickSpeech();



public:
	CLONE(CMTItem)

public:
	CMTItem();
	~CMTItem();
	 
};

