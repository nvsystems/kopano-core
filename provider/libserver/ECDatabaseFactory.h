/*
 * SPDX-License-Identifier: AGPL-3.0-only
 * Copyright 2005 - 2016 Zarafa and its licensors
 */

#ifndef ECDATABASEFACTORY_H
#define ECDATABASEFACTORY_H

#include <memory>
#include <kopano/zcdefs.h>
#include "ECDatabase.h"
#include <kopano/ECConfig.h>
#include <kopano/ECLogger.h>

namespace KC {

// The ECDatabaseFactory creates database objects connected to the server database. Which
// database is returned is chosen by the database_engine configuration setting.

class _kc_export ECDatabaseFactory _kc_final {
public:
	ECDatabaseFactory(std::shared_ptr<ECConfig>);
	ECRESULT		CreateDatabaseObject(ECDatabase **lppDatabase, std::string &ConnectError);
	ECRESULT		CreateDatabase();
	ECRESULT		UpdateDatabase(bool bForceUpdate, std::string &strError);

private:
	_kc_hidden ECRESULT GetDatabaseFactory(ECDatabase **);

	std::shared_ptr<ECConfig> m_lpConfig;
};

ECRESULT	GetThreadLocalDatabase(ECDatabaseFactory *lpFactory, ECDatabase **lppDatabase);

} /* namespace */

#endif // ECDATABASEFACTORY_H
