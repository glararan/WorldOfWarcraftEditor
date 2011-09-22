#define		ENTRY_FAKE		0
#define		ENTRY_MODEL		1
#define		ENTRY_WMO		2
#define		ENTRY_MAPCHUNK	3

using namespace std;

class nameEntry
{
private:
	string	Name;
public:
	int type;
	union
	{
		ModelInstance* model;
		WMOInstance*   wmo;
		MapChunk*      mapchunk;
	} data;

	nameEntry(ModelInstance* mod)
	{
		type = ENTRY_MODEL;
		data.model = mod;
		Name = data.model->model->name.c_str();
	};

	nameEntry(WMOInstance* wmo)
	{
		type = ENTRY_WMO;
		data.wmo = wmo;
		Name = data.wmo->wmo->name.c_str();
	};

	nameEntry(MapChunk* chunk)
	{
		char Temp[255];
		type = ENTRY_MAPCHUNK;
		data.mapchunk = chunk;
		sprintf_s(Temp, "Map Chunk (%d, %d)", chunk->px, chunk->py);
		Name = Temp;
	};

	nameEntry()
	{
		type = ENTRY_FAKE;
		Name = "Fake";
	}

	~nameEntry(){};
	
	const char* returnName()
	{
		return Name.c_str();
	};
};

class nameEntryManager
{
private:
	unsigned int NextName;
	vector<nameEntry*> items;
public:
	unsigned int add(ModelInstance* mod)
	{
		items.push_back(new nameEntry(mod));
		NextName++;
		return NextName - 1;
	};

	unsigned int add(WMOInstance* wmo)
	{
		items.push_back(new nameEntry(wmo));
		NextName++;
		return NextName - 1;
	};

	unsigned int add(MapChunk* chunk)
	{
		items.push_back(new nameEntry(chunk));
		NextName++;
		return NextName - 1;
	};

	void addFake()
	{
		items.push_back(new nameEntry);
		NextName++;
	}

	void del(unsigned int Ref);
	nameEntry* findEntry(unsigned int ref)
	{
		return items[ref];
	};

	nameEntryManager(){NextName = 0; addFake(); }
	~nameEntryManager(){};
};

nameEntry* getSelect(unsigned int pick);
unsigned int addNameModel(ModelInstance* model);
unsigned int addNameWMO(WMOInstance* wmo);
unsigned int addNameMapChunk(MapChunk* chunk);
void deleteName(unsigned int Name);