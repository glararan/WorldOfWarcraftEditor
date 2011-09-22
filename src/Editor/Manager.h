#ifndef MANAGER_H
#define MANAGER_H

#include <string>
#include <map>

using namespace std;
// base class for manager objects

class ManagedItem
{
	int refcount;
public:
	string name;
	ManagedItem(string n) : name(n), refcount(0) {}
	virtual ~ManagedItem() {}

	void addref()
	{
		++refcount;
	}

	bool delref()
	{
		return --refcount==0;
	}
};

template <class IDTYPE>
class Manager
{
public:
	map<string, IDTYPE> names;
	map<IDTYPE, ManagedItem*> items;

	Manager()
	{
	}

	virtual IDTYPE add(string name) = 0;

	virtual void del(IDTYPE id)
	{
		if (items[id]->delref())
		{
			ManagedItem* i = items[id];
			doDelete(id);
			names.erase(names.find(i->name));
			items.erase(items.find(id));
			delete i;
		}
	}

	void delbyname(string name)
	{
		if(has(name))
			del(get(name));
	}

	virtual void doDelete(IDTYPE id) {}

	bool has(string name)
	{
		return (names.find(name) != names.end());
	}

	IDTYPE get(string name)
	{
		return names[name];
	}

protected:
	void do_add(string name, IDTYPE id, ManagedItem* item)
	{
		names[name] = id;
		item->addref();
		items[id] = item;
	}
};

class SimpleManager : public Manager<int>
{
	int baseid;
public:
	SimpleManager() : baseid(0)
	{
	}

protected:
	int nextID()
	{
		return baseid++;
	}
};

#endif

