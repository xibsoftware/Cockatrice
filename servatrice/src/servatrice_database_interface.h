#ifndef SERVATRICE_DATABASE_INTERFACE_H
#define SERVATRICE_DATABASE_INTERFACE_H

#include <QObject>
#include <QSqlDatabase>

#include "server.h"
#include "server_database_interface.h"

class Servatrice;

class Servatrice_DatabaseInterface : public Server_DatabaseInterface {
	Q_OBJECT
private:
	int instanceId;
	QSqlDatabase sqlDatabase;
	Servatrice *server;
	ServerInfo_User evalUserQueryResult(const QSqlQuery &query, bool complete, bool withId = false);
protected:
	AuthenticationResult checkUserPassword(Server_ProtocolHandler *handler, const QString &user, const QString &password, QString &reasonStr, int &secondsLeft);
public:
	Servatrice_DatabaseInterface(int _instanceId, Servatrice *_server);
	void initDatabase(const QSqlDatabase &_sqlDatabase);
	bool openDatabase();
	bool checkSql();
	bool execSqlQuery(QSqlQuery &query);

	bool userExists(const QString &user);
	int getUserIdInDB(const QString &name);
	QMap<QString, ServerInfo_User> getBuddyList(const QString &name);
	QMap<QString, ServerInfo_User> getIgnoreList(const QString &name);
	bool isInBuddyList(const QString &whoseList, const QString &who);
	bool isInIgnoreList(const QString &whoseList, const QString &who);
	ServerInfo_User getUserData(const QString &name, bool withId = false);
	
	int getNextGameId();
	int getNextReplayId();
	
	qint64 startSession(const QString &userName, const QString &address);
	void endSession(qint64 sessionId);
	
	void clearSessionTables();
	void lockSessionTables();
	void unlockSessionTables();
	bool userSessionExists(const QString &userName);
	
};

#endif
