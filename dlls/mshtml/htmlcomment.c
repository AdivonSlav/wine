/*
 * Copyright 2008 Jacek Caban for CodeWeavers
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


#include <stdarg.h>

#define COBJMACROS

#include "windef.h"
#include "winbase.h"
#include "winuser.h"
#include "ole2.h"

#include "mshtml_private.h"
#include "htmlevent.h"

#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(mshtml);

struct HTMLCommentElement {
    HTMLElement element;
    IHTMLCommentElement IHTMLCommentElement_iface;
};

static inline HTMLCommentElement *impl_from_IHTMLCommentElement(IHTMLCommentElement *iface)
{
    return CONTAINING_RECORD(iface, HTMLCommentElement, IHTMLCommentElement_iface);
}

static HRESULT WINAPI HTMLCommentElement_QueryInterface(IHTMLCommentElement *iface,
        REFIID riid, void **ppv)
{
    HTMLCommentElement *This = impl_from_IHTMLCommentElement(iface);

    return IHTMLDOMNode_QueryInterface(&This->element.node.IHTMLDOMNode_iface, riid, ppv);
}

static ULONG WINAPI HTMLCommentElement_AddRef(IHTMLCommentElement *iface)
{
    HTMLCommentElement *This = impl_from_IHTMLCommentElement(iface);

    return IHTMLDOMNode_AddRef(&This->element.node.IHTMLDOMNode_iface);
}

static ULONG WINAPI HTMLCommentElement_Release(IHTMLCommentElement *iface)
{
    HTMLCommentElement *This = impl_from_IHTMLCommentElement(iface);

    return IHTMLDOMNode_Release(&This->element.node.IHTMLDOMNode_iface);
}

static HRESULT WINAPI HTMLCommentElement_GetTypeInfoCount(IHTMLCommentElement *iface, UINT *pctinfo)
{
    HTMLCommentElement *This = impl_from_IHTMLCommentElement(iface);
    return IDispatchEx_GetTypeInfoCount(&This->element.node.event_target.dispex.IDispatchEx_iface, pctinfo);
}

static HRESULT WINAPI HTMLCommentElement_GetTypeInfo(IHTMLCommentElement *iface, UINT iTInfo,
        LCID lcid, ITypeInfo **ppTInfo)
{
    HTMLCommentElement *This = impl_from_IHTMLCommentElement(iface);
    return IDispatchEx_GetTypeInfo(&This->element.node.event_target.dispex.IDispatchEx_iface, iTInfo, lcid,
            ppTInfo);
}

static HRESULT WINAPI HTMLCommentElement_GetIDsOfNames(IHTMLCommentElement *iface, REFIID riid,
                                                LPOLESTR *rgszNames, UINT cNames,
                                                LCID lcid, DISPID *rgDispId)
{
    HTMLCommentElement *This = impl_from_IHTMLCommentElement(iface);
    return IDispatchEx_GetIDsOfNames(&This->element.node.event_target.dispex.IDispatchEx_iface, riid, rgszNames,
            cNames, lcid, rgDispId);
}

static HRESULT WINAPI HTMLCommentElement_Invoke(IHTMLCommentElement *iface, DISPID dispIdMember,
                            REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams,
                            VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
    HTMLCommentElement *This = impl_from_IHTMLCommentElement(iface);
    return IDispatchEx_Invoke(&This->element.node.event_target.dispex.IDispatchEx_iface, dispIdMember, riid,
            lcid, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
}

static HRESULT WINAPI HTMLCommentElement_put_text(IHTMLCommentElement *iface, BSTR v)
{
    HTMLCommentElement *This = impl_from_IHTMLCommentElement(iface);
    FIXME("(%p)->(%s)\n", This, debugstr_w(v));
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLCommentElement_get_text(IHTMLCommentElement *iface, BSTR *p)
{
    HTMLCommentElement *This = impl_from_IHTMLCommentElement(iface);

    TRACE("(%p)->(%p)\n", This, p);

    return IHTMLElement_get_outerHTML(&This->element.IHTMLElement_iface, p);
}

static HRESULT WINAPI HTMLCommentElement_put_atomic(IHTMLCommentElement *iface, LONG v)
{
    HTMLCommentElement *This = impl_from_IHTMLCommentElement(iface);
    FIXME("(%p)->(%ld)\n", This, v);
    return E_NOTIMPL;
}

static HRESULT WINAPI HTMLCommentElement_get_atomic(IHTMLCommentElement *iface, LONG *p)
{
    HTMLCommentElement *This = impl_from_IHTMLCommentElement(iface);
    FIXME("(%p)->(%p)\n", This, p);
    return E_NOTIMPL;
}

static const IHTMLCommentElementVtbl HTMLCommentElementVtbl = {
    HTMLCommentElement_QueryInterface,
    HTMLCommentElement_AddRef,
    HTMLCommentElement_Release,
    HTMLCommentElement_GetTypeInfoCount,
    HTMLCommentElement_GetTypeInfo,
    HTMLCommentElement_GetIDsOfNames,
    HTMLCommentElement_Invoke,
    HTMLCommentElement_put_text,
    HTMLCommentElement_get_text,
    HTMLCommentElement_put_atomic,
    HTMLCommentElement_get_atomic
};

static inline HTMLCommentElement *impl_from_HTMLDOMNode(HTMLDOMNode *iface)
{
    return CONTAINING_RECORD(iface, HTMLCommentElement, element.node);
}

static void *HTMLCommentElement_QI(HTMLDOMNode *iface, REFIID riid)
{
    HTMLCommentElement *This = impl_from_HTMLDOMNode(iface);

    if(IsEqualGUID(&IID_IHTMLCommentElement, riid))
        return &This->IHTMLCommentElement_iface;

    return HTMLElement_QI(&This->element.node, riid);
}

static void HTMLCommentElement_destructor(HTMLDOMNode *iface)
{
    HTMLCommentElement *This = impl_from_HTMLDOMNode(iface);

    HTMLElement_destructor(&This->element.node);
}

static HRESULT HTMLCommentElement_clone(HTMLDOMNode *iface, nsIDOMNode *nsnode, HTMLDOMNode **ret)
{
    HTMLCommentElement *This = impl_from_HTMLDOMNode(iface);
    HTMLElement *new_elem;
    HRESULT hres;

    hres = HTMLCommentElement_Create(This->element.node.doc, nsnode, &new_elem);
    if(FAILED(hres))
        return hres;

    *ret = &new_elem->node;
    return S_OK;
}

static const NodeImplVtbl HTMLCommentElementImplVtbl = {
    .clsid                 = &CLSID_HTMLCommentElement,
    .qi                    = HTMLCommentElement_QI,
    .destructor            = HTMLCommentElement_destructor,
    .cpc_entries           = HTMLElement_cpc,
    .clone                 = HTMLCommentElement_clone,
    .handle_event          = HTMLElement_handle_event,
    .get_attr_col          = HTMLElement_get_attr_col
};

static const tid_t HTMLCommentElement_iface_tids[] = {
    HTMLELEMENT_TIDS,
    IHTMLCommentElement_tid,
    0
};
static dispex_static_data_t HTMLCommentElement_dispex = {
    "Comment",
    &HTMLElement_event_target_vtbl.dispex_vtbl,
    DispHTMLCommentElement_tid,
    HTMLCommentElement_iface_tids,
    HTMLElement_init_dispex_info
};

HRESULT HTMLCommentElement_Create(HTMLDocumentNode *doc, nsIDOMNode *nsnode, HTMLElement **elem)
{
    HTMLCommentElement *ret;

    ret = calloc(1, sizeof(*ret));
    if(!ret)
        return E_OUTOFMEMORY;

    ret->element.node.vtbl = &HTMLCommentElementImplVtbl;
    ret->IHTMLCommentElement_iface.lpVtbl = &HTMLCommentElementVtbl;

    HTMLElement_Init(&ret->element, doc, NULL, &HTMLCommentElement_dispex);
    HTMLDOMNode_Init(doc, &ret->element.node, nsnode, &HTMLCommentElement_dispex);

    *elem = &ret->element;
    return S_OK;
}
