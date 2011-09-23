#include "World.h"

nameEntryManager SelectionNames;

nameEntry* getSelect(unsigned int pick)
{
	return SelectionNames.findEntry(pick);
}

unsigned int addNameModel(ModelInstance* model)
{
	return SelectionNames.add(model);
}

unsigned int addNameWMO(WMOInstance* wmo)
{
	return SelectionNames.add(wmo);
}

unsigned int addNameMapChunk(MapChunk* chunk)
{
	return SelectionNames.add(chunk);
}

void deleteName(unsigned int Name)
{
	SelectionNames.del(Name);
}

extern nameEntry* Selection;
extern nameEntry* CurSelection;

void nameEntryManager::del(unsigned int Ref)
{
	if(items[Ref] != 0)
	{
		if(Selection == items[Ref])
			Selection = 0;
		if(CurSelection == items[Ref])
			CurSelection = 0;
		delete (items[Ref]);
	}
	items[Ref] = 0;
}