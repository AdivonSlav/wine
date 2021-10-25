/*
 * Copyright 2019 Alistair Leslie-Hughes
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#define COBJMACROS

#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "objbase.h"
#include "rpcproxy.h"
#include "wine/debug.h"

#include "initguid.h"
#include "msdasql.h"

WINE_DEFAULT_DEBUG_CHANNEL(msdasql);

static HRESULT WINAPI ClassFactory_QueryInterface(IClassFactory *iface, REFIID riid, void **ppv)
{
    *ppv = NULL;

    if(IsEqualGUID(&IID_IUnknown, riid)) {
        TRACE("(%p)->(IID_IUnknown %p)\n", iface, ppv);
        *ppv = iface;
    }else if(IsEqualGUID(&IID_IClassFactory, riid)) {
        TRACE("(%p)->(IID_IClassFactory %p)\n", iface, ppv);
        *ppv = iface;
    }

    if(*ppv) {
        IUnknown_AddRef((IUnknown*)*ppv);
        return S_OK;
    }

    WARN("(%p)->(%s %p)\n", iface, debugstr_guid(riid), ppv);
    return E_NOINTERFACE;
}

static ULONG WINAPI ClassFactory_AddRef(IClassFactory *iface)
{
    TRACE("(%p)\n", iface);
    return 2;
}

static ULONG WINAPI ClassFactory_Release(IClassFactory *iface)
{
    TRACE("(%p)\n", iface);
    return 1;
}

static HRESULT create_msdasql_provider(REFIID riid, void **ppv);

HRESULT WINAPI msdasql_CreateInstance(IClassFactory *iface, IUnknown *outer, REFIID riid, void **ppv)
{
    TRACE("(%p %s %p)\n", outer, debugstr_guid(riid), ppv);

    return create_msdasql_provider(riid, ppv);
}

static HRESULT WINAPI ClassFactory_LockServer(IClassFactory *iface, BOOL fLock)
{
    TRACE("(%p)->(%x)\n", iface, fLock);
    return S_OK;
}

static const IClassFactoryVtbl cfmsdasqlVtbl = {
    ClassFactory_QueryInterface,
    ClassFactory_AddRef,
    ClassFactory_Release,
    msdasql_CreateInstance,
    ClassFactory_LockServer
};

static IClassFactory cfmsdasql = { &cfmsdasqlVtbl };

HRESULT WINAPI DllGetClassObject( REFCLSID rclsid, REFIID riid, void **ppv )
{
    TRACE("%s %s %p\n", debugstr_guid(rclsid), debugstr_guid(riid), ppv);

    if (IsEqualGUID(&CLSID_MSDASQL, rclsid))
        return IClassFactory_QueryInterface(&cfmsdasql, riid, ppv);

    return CLASS_E_CLASSNOTAVAILABLE;
}

struct msdasql
{
    IUnknown         MSDASQL_iface;

    LONG     ref;
};

static inline struct msdasql *impl_from_IUnknown(IUnknown *iface)
{
    return CONTAINING_RECORD(iface, struct msdasql, MSDASQL_iface);
}

static HRESULT WINAPI msdsql_QueryInterface(IUnknown *iface, REFIID riid, void **out)
{
    struct msdasql *provider = impl_from_IUnknown(iface);

    TRACE("(%p)->(%s %p)\n", provider, debugstr_guid(riid), out);

    if(IsEqualGUID(riid, &IID_IUnknown) ||
       IsEqualGUID(riid, &CLSID_MSDASQL))
    {
        *out = &provider->MSDASQL_iface;
    }
    else
    {
        FIXME("(%s, %p)\n", debugstr_guid(riid), out);
        *out = NULL;
        return E_NOINTERFACE;
    }

    IUnknown_AddRef((IUnknown*)*out);
    return S_OK;
}

static ULONG WINAPI msdsql_AddRef(IUnknown *iface)
{
    struct msdasql *provider = impl_from_IUnknown(iface);
    ULONG ref = InterlockedIncrement(&provider->ref);

    TRACE("(%p) ref=%u\n", provider, ref);

    return ref;
}

static ULONG  WINAPI msdsql_Release(IUnknown *iface)
{
    struct msdasql *provider = impl_from_IUnknown(iface);
    ULONG ref = InterlockedDecrement(&provider->ref);

    TRACE("(%p) ref=%u\n", provider, ref);

    if (!ref)
    {
        free(provider);
    }

    return ref;
}

static const IUnknownVtbl msdsql_vtbl =
{
    msdsql_QueryInterface,
    msdsql_AddRef,
    msdsql_Release
};

static HRESULT create_msdasql_provider(REFIID riid, void **ppv)
{
    struct msdasql *provider;
    HRESULT hr;

    provider = malloc(sizeof(struct msdasql));
    if (!provider)
        return E_OUTOFMEMORY;

    provider->MSDASQL_iface.lpVtbl = &msdsql_vtbl;
    provider->ref = 1;

    hr = IUnknown_QueryInterface(&provider->MSDASQL_iface, riid, ppv);
    IUnknown_Release(&provider->MSDASQL_iface);
    return hr;
}
