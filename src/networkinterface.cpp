//
// Configuration
//

// Local includes
#include "networkinterface.h"
#include "mainapplication.h"

// Library includes
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtNetwork/QHostInfo>
#include <qxmpp/QXmppConfiguration.h>
#include <qxmpp/QXmppRosterManager.h>


//
// Construction and destruction
//

FlatTurtle::NetworkInterface::NetworkInterface(QObject *iParent) throw(QException)
    : QXmppClient(iParent) {
    // Load settings
    mSettings = new QSettings(this);
    mSettings->beginGroup(metaObject()->className());

    // Connect slots
    connect(this, SIGNAL(messageReceived(const QXmppMessage&)), SLOT(messageReceived(const QXmppMessage&)));
    connect(this, SIGNAL(disconnected()), SLOT(disconnected()));

    // Construct the configuration parameters
    QXmppConfiguration tConfiguration;
    tConfiguration.setHost("botmaster.corp.flatturtle.com");
#ifdef DEVEL
    qWarning() << "WARNING: using development XMPP configuration";
    tConfiguration.setResource("botnet.corp.flatturtle.com");
    tConfiguration.setJid("testclient@botnet.corp.flatturtle.com");
#else
    QHostInfo tHostInfo;
    tConfiguration.setResource(tHostInfo.localDomainName());
    tConfiguration.setJid(tHostInfo.localHostName() + "@" + tHostInfo.localDomainName());
#endif
    tConfiguration.setAutoAcceptSubscriptions(true);
    tConfiguration.setAutoReconnectionEnabled(true);

    // Connect
    connectToServer(tConfiguration);
}


//
// XMPP events
//

void FlatTurtle::NetworkInterface::messageReceived(const QXmppMessage& iMessage) {
    qDebug() << "Received command from" << iMessage.from() << ":" << iMessage.body();
    QVariant tOutput = MainApplication::instance()->userInterface()->execute(iMessage.body());
    sendMessage(iMessage.from(), tOutput.toString());
}

void FlatTurtle::NetworkInterface::disconnected() {
    if (!isConnected()) {
        qWarning() << "WARNING: attempting to reconnect";
        connectToServer(configuration(), clientPresence());
        QTimer::singleShot(mSettings->value("reconnection/interval", 60000).toInt(), this, SLOT(disconnected()));
    }
}