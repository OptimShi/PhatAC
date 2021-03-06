#pragma once

#define MYSQL_CONNECT_TIMEOUT 3

typedef char** SQLResultRow_t;

class CSQLResult
{
public:
	CSQLResult() { }
	virtual ~CSQLResult() { }

	virtual SQLResultRow_t FetchRow() = 0;
	virtual uint64_t ResultRows() = 0;
};

class CMYSQLResult : public CSQLResult
{
public:
	CMYSQLResult(MYSQL_RES *);
	virtual ~CMYSQLResult();
	virtual void Free();

	virtual SQLResultRow_t FetchRow() override;
	virtual uint64_t ResultRows() override;

private:
	MYSQL_RES *m_Result;
};

class CSQLConnection
{
public:
	virtual ~CSQLConnection() { }
	virtual void Close() = 0;
	virtual bool Query(const char *query) = 0;
	virtual unsigned int LastError() = 0;
	virtual CSQLResult *GetResult() = 0;

	static int s_NumConnectAttempts;
};

class CMYSQLConnection : public CSQLConnection
{
public:
	virtual ~CMYSQLConnection();
	static CMYSQLConnection *Create(const char *host, unsigned int port, const char *user, const char *password, const char *defaultdatabase);
	virtual void Close() override;
	virtual bool Query(const char *query);
	virtual unsigned int LastError();
	virtual CSQLResult *GetResult();

private:
	CMYSQLConnection(MYSQL *connection);

	MYSQL *m_InternalConnection;
};

class CDatabase2
{
public:
	CDatabase2();
	virtual ~CDatabase2();

	virtual bool Query(const char *format, ...);
	virtual void Tick();
	virtual CSQLResult *GetResult();

protected:
	CSQLConnection *m_pConnection;
};

class CMYSQLDatabase : public CDatabase2
{
public:
	CMYSQLDatabase(const char *host, unsigned int port, const char *user, const char *password, const char *defaultdatabasename);
	virtual ~CMYSQLDatabase();

protected:
	virtual void Tick() override;

private:
	void RefreshConnection();

	std::string m_DatabaseHost;
	unsigned int m_DatabasePort;
	std::string m_DatabaseUser;
	std::string m_DatabasePassword;
	std::string m_DatabaseName;

	double m_fLastRefresh;
	bool m_bDisabled;
};

#include "ModelInfo.h"
#include "PhysicsDesc.h"
#include "PublicWeenieDesc.h"

class CCapturedWorldObjectInfo
{
public:
	CCapturedWorldObjectInfo() {
		m_ObjData = NULL;
		m_ObjDataLen = 0;
	}
	~CCapturedWorldObjectInfo() {
		Free();
	}
	void Free() {
		if (m_ObjData)
		{
			delete[] m_ObjData;
			m_ObjData = NULL;
		}
	}

	std::string m_ObjName;
	BYTE *m_ObjData;
	unsigned m_ObjDataLen;

	DWORD guid;
	ModelInfo appearance;
	PhysicsDesc physics;
	PublicWeenieDesc weenie;
	std::map<DWORD, DWORD> dwordProperties;
	std::map<DWORD, UINT64> qwordProperties;
	std::map<DWORD, DWORD> boolProperties;
	std::map<DWORD, double> floatProperties;
	std::map<DWORD, std::string> stringProperties;
	std::map<DWORD, DWORD> dataIDProperties;
	ModelInfo wornAppearance;
};

class CGameDatabase
{
public:
	CGameDatabase();
	virtual ~CGameDatabase();

	void Init();

	bool LoadedPortals() { return m_bLoadedPortals; }
	CCapturedWorldObjectInfo *GetCapturedMonsterData(const char *name);
	CCapturedWorldObjectInfo *GetCapturedItemData(const char *name);
	CCapturedWorldObjectInfo *GetCapturedArmorData(const char *name);
	CCapturedWorldObjectInfo *GetRandomCapturedMonsterData();
	CCapturedWorldObjectInfo *GetRandomCapturedItemData();
	CCapturedWorldObjectInfo *GetRandomCapturedArmorData();

	void SpawnAerfalle();

	class CPhysicsObj *CreateFromCapturedData(CCapturedWorldObjectInfo *pObjectInfo);

protected:

	std::string ConvertNameForLookup(std::string name);

	void LoadPortals();
	void LoadAerfalle();	
	void LoadCapturedMonsterData();	
	void LoadCapturedItemData();
	void LoadCapturedArmorData();
	void LoadMonsterTemplates();
	void LoadTeleTownList();
	void LoadStaticsData();

	bool m_bLoadedPortals;

	std::list<CCapturedWorldObjectInfo *> m_CapturedStaticsData;
	std::list<CCapturedWorldObjectInfo *> m_CapturedAerfalleData;
	std::map<std::string, CCapturedWorldObjectInfo *> m_CapturedMonsterData;
	std::vector<CCapturedWorldObjectInfo *> m_CapturedMonsterDataList;
	std::map<std::string, CCapturedWorldObjectInfo *> m_CapturedItemData;
	std::vector<CCapturedWorldObjectInfo *> m_CapturedItemDataList;
	std::map<std::string, CCapturedWorldObjectInfo *> m_CapturedArmorData;
	std::vector<CCapturedWorldObjectInfo *> m_CapturedArmorDataList;
};

extern CGameDatabase *g_pGameDatabase;

