#include "abstractclient.h"
#include "protocol.h"
#include "protocol_items.h"
#include <QDebug>

AbstractClient::AbstractClient(QObject *parent)
	: QObject(parent), status(StatusDisconnected)
{
}

AbstractClient::~AbstractClient()
{
}

void AbstractClient::processProtocolItem(ProtocolItem *item)
{
	ProtocolResponse *response = qobject_cast<ProtocolResponse *>(item);
	if (response) {
		CommandContainer *cmdCont = pendingCommands.value(response->getCmdId(), 0);
		if (!cmdCont)
			return;
		
		pendingCommands.remove(cmdCont->getCmdId());
		cmdCont->processResponse(response);
		if (response->getReceiverMayDelete())
			delete response;
		cmdCont->deleteLater();
		
		return;
	}
	
	GenericEvent *genericEvent = qobject_cast<GenericEvent *>(item);
	if (genericEvent) {
		switch (genericEvent->getItemId()) {
			case ItemId_Event_ListGames: emit listGamesEventReceived(qobject_cast<Event_ListGames *>(item)); break;
			case ItemId_Event_ServerMessage: emit serverMessageEventReceived(qobject_cast<Event_ServerMessage *>(item)); break;
			case ItemId_Event_ListChatChannels: emit listChatChannelsEventReceived(qobject_cast<Event_ListChatChannels *>(item)); break;
			case ItemId_Event_GameJoined: emit gameJoinedEventReceived(qobject_cast<Event_GameJoined *>(item)); break;
		}
		if (genericEvent->getReceiverMayDelete())
			delete genericEvent;
		return;
	}

	GameEventContainer *gameEventContainer = qobject_cast<GameEventContainer *>(item);
	if (gameEventContainer) {
		emit gameEventContainerReceived(gameEventContainer);
		if (gameEventContainer->getReceiverMayDelete())
			delete gameEventContainer;
		return;
	}

	ChatEvent *chatEvent = qobject_cast<ChatEvent *>(item);
	if (chatEvent) {
		emit chatEventReceived(chatEvent);
		if (chatEvent->getReceiverMayDelete())
			delete chatEvent;
		return;
	}
}


void AbstractClient::setStatus(const ClientStatus _status)
{
	if (_status != status) {
		status = _status;
		emit statusChanged(_status);
	}
}

void AbstractClient::sendCommand(Command *cmd)
{
	sendCommandContainer(new CommandContainer(QList<Command *>() << cmd));
}

