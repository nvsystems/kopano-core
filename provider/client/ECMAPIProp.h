/*
 * SPDX-License-Identifier: AGPL-3.0-only
 * Copyright 2005 - 2016 Zarafa and its licensors
 */

#ifndef ECMAPIPROP_H
#define ECMAPIPROP_H

#include <kopano/memory.hpp>
#include <kopano/zcdefs.h>
#include "kcore.hpp"
#include <kopano/IECInterfaces.hpp>
#include "ECGenericProp.h"

// For HrSetFlags
#define SET         1
#define UNSET       2

using namespace KC;
class ECMsgStore;

class ECMAPIProp : public ECGenericProp, public IECSecurity {
protected:
	ECMAPIProp(ECMsgStore *prov, ULONG obj_type, BOOL modify, const ECMAPIProp *root, const char *cls = nullptr);
	virtual ~ECMAPIProp() = default;

public:
	/**
	 * \brief Obtain a different interface to this object.
	 *
	 * See ECUnkown::QueryInterface.
	 */
	virtual HRESULT QueryInterface(REFIID refiid, void **lppInterface) _kc_override;
	static HRESULT TableRowGetProp(void *prov, const struct propVal *src, SPropValue *dst, void **base, ULONG type);

	// Callback for Commit() on streams
	static HRESULT	HrStreamCommit(IStream *lpStream, void *lpData);
	// Callback for ECMemStream delete local data
	static HRESULT	HrStreamCleanup(void *lpData);

	static HRESULT	DefaultMAPIGetProp(ULONG ulPropTag, void* lpProvider, ULONG ulFlags, LPSPropValue lpsPropValue, void *lpParam, void *lpBase);
	static HRESULT SetPropHandler(ULONG ulPropTag, void *lpProvider, const SPropValue *lpsPropValue, void *lpParam);

	// IMAPIProp override
	virtual HRESULT OpenProperty(ULONG tag, const IID *intf, ULONG iface_opts, ULONG flags, IUnknown **) override;
	virtual HRESULT SaveChanges(ULONG flags) override;
	virtual HRESULT CopyTo(ULONG nexcl, const IID *excl, const SPropTagArray *exclprop, ULONG ui_param, IMAPIProgress *, const IID *intf, void *dest, ULONG flags, SPropProblemArray **) override;
	virtual HRESULT CopyProps(const SPropTagArray *inclprop, ULONG ui_param, IMAPIProgress *, const IID *intf, void *dest, ULONG flags, SPropProblemArray **) override;
	virtual HRESULT GetNamesFromIDs(SPropTagArray **tags, const GUID *, ULONG flags, ULONG *nvals, MAPINAMEID ***names) override;
	virtual HRESULT GetIDsFromNames(ULONG n, MAPINAMEID **, ULONG flags, SPropTagArray **) override;

	virtual HRESULT HrSetSyncId(ULONG ulSyncId);
	virtual HRESULT SetParentID(ULONG eid, const ENTRYID *eid_size);
	virtual HRESULT SetICSObject(BOOL bICSObject);
	virtual BOOL IsICSObject();

protected:
	HRESULT HrLoadProps() override;
	virtual HRESULT HrSaveChild(ULONG flags, MAPIOBJECT *) override;
	HRESULT GetSerializedACLData(LPVOID lpBase, LPSPropValue lpsPropValue);
	HRESULT SetSerializedACLData(const SPropValue *lpsPropValue);
	HRESULT	UpdateACLs(ULONG cNewPerms, ECPERMISSION *lpNewPerms);

	// IECServiceAdmin and IECSecurity
	virtual HRESULT GetUserList(ULONG eid_size, const ENTRYID *comp_eid, ULONG flags, ULONG *nusers, ECUSER **) override;
	virtual HRESULT GetGroupList(ULONG eid_size, const ENTRYID *comp_eid, ULONG flags, ULONG *ngrps, ECGROUP **) override;
	virtual HRESULT GetCompanyList(ULONG flags, ULONG *ncomp, ECCOMPANY **) override;
	// IECSecurity
	virtual HRESULT GetOwner(ULONG *id_size, ENTRYID **) override;
	virtual HRESULT GetPermissionRules(int type, ULONG *nperm, ECPERMISSION **) override;
	virtual HRESULT SetPermissionRules(ULONG n, const ECPERMISSION *) override;

public:
	ECMsgStore *GetMsgStore() const { return static_cast<ECMsgStore *>(lpProvider); }

private:
	BOOL m_bICSObject = false; // coming from the ICS system
	ULONG m_ulSyncId = 0, m_cbParentID = 0;
	KC::memory_ptr<ENTRYID> m_lpParentID; /* Overrides the parentid from the server */

public:
	const ECMAPIProp *m_lpRoot; // Points to the 'root' object that was opened by OpenEntry; normally points to 'this' except for Attachments and Submessages
};
IID_OF(ECMAPIProp)

#endif // ECMAPIPROP_H
