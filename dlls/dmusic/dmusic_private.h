/*
 * DirectMusic Private Include
 *
 * Copyright (C) 2003-2004 Rok Mandeljc
 * Copyright (C) 2012 Christian Costa
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef __WINE_DMUSIC_PRIVATE_H
#define __WINE_DMUSIC_PRIVATE_H

#include <stdarg.h>

#define COBJMACROS
#include "windef.h"
#include "winbase.h"
#include "winnt.h"
#include "wingdi.h"
#include "winuser.h"

#include "wine/debug.h"
#include "wine/list.h"
#include "winreg.h"
#include "objbase.h"

#include "dmusici.h"
#include "dmusicf.h"
#include "dmusics.h"
#include "dmksctrl.h"

/*****************************************************************************
 * Interfaces
 */
typedef struct IDirectMusic8Impl IDirectMusic8Impl;
typedef struct IDirectMusicBufferImpl IDirectMusicBufferImpl;
typedef struct IDirectMusicDownloadedInstrumentImpl IDirectMusicDownloadedInstrumentImpl;
typedef struct IReferenceClockImpl IReferenceClockImpl;

/*****************************************************************************
 * Some stuff to make my life easier :=)
 */
 
/* some sort of aux. midi channel: big fake at the moment; accepts only priority
   changes... more coming soon */
typedef struct DMUSIC_PRIVATE_MCHANNEL_ {
	DWORD priority;
} DMUSIC_PRIVATE_MCHANNEL, *LPDMUSIC_PRIVATE_MCHANNEL;

/* some sort of aux. channel group: collection of 16 midi channels */
typedef struct DMUSIC_PRIVATE_CHANNEL_GROUP_ {
	DMUSIC_PRIVATE_MCHANNEL channel[16]; /* 16 channels in a group */
} DMUSIC_PRIVATE_CHANNEL_GROUP, *LPDMUSIC_PRIVATE_CHANNEL_GROUP;

typedef struct port_info {
    DMUS_PORTCAPS caps;
    HRESULT (*create)(IDirectMusic8Impl *parent, DMUS_PORTPARAMS *port_params,
            DMUS_PORTCAPS *port_caps, IDirectMusicPort **port);
    ULONG device;
} port_info;

struct region
{
    struct list entry;
    RGNHEADER header;
    WAVELINK wave_link;
    WSMPL wave_sample;
    WLOOP wave_loop;
    BOOL loop_present;
};

/*****************************************************************************
 * ClassFactory
 */

/* CLSID */
extern HRESULT music_create(IUnknown **ret_iface);
extern HRESULT collection_create(IUnknown **ret_iface);

/* Internal */
extern HRESULT DMUSIC_CreateDirectMusicBufferImpl(LPDMUS_BUFFERDESC desc, LPVOID* ret_iface);
extern HRESULT DMUSIC_CreateReferenceClockImpl (LPCGUID lpcGUID, LPVOID* ppobj, LPUNKNOWN pUnkOuter);

extern HRESULT download_create(DWORD size, IDirectMusicDownload **ret_iface);

extern HRESULT instrument_create_from_stream(IStream *stream, IDirectMusicInstrument **ret_iface);

/*****************************************************************************
 * IDirectMusic8Impl implementation structure
 */
struct IDirectMusic8Impl {
    IDirectMusic8 IDirectMusic8_iface;
    LONG ref;
    IDirectSound *dsound;
    IReferenceClock *master_clock;
    IDirectMusicPort **ports;
    int num_ports;
    port_info *system_ports;
    int num_system_ports;
};

/*****************************************************************************
 * IDirectMusicBufferImpl implementation structure
 */
struct IDirectMusicBufferImpl {
    /* IUnknown fields */
    IDirectMusicBuffer IDirectMusicBuffer_iface;
    LONG ref;

    /* IDirectMusicBufferImpl fields */
    GUID format;
    DWORD size;
    LPBYTE data;
    DWORD write_pos;
    REFERENCE_TIME start_time;
};

/*****************************************************************************
 * IDirectMusicDownloadedInstrumentImpl implementation structure
 */
struct IDirectMusicDownloadedInstrumentImpl {
    /* IUnknown fields */
    IDirectMusicDownloadedInstrument IDirectMusicDownloadedInstrument_iface;
    LONG ref;

    /* IDirectMusicDownloadedInstrumentImpl fields */
    BOOL downloaded;
    void *data;
};

/** Internal factory */
extern HRESULT synth_port_create(IDirectMusic8Impl *parent, DMUS_PORTPARAMS *port_params,
        DMUS_PORTCAPS *port_caps, IDirectMusicPort **port);
extern HRESULT midi_out_port_create(IDirectMusic8Impl *parent, DMUS_PORTPARAMS *port_params,
        DMUS_PORTCAPS *port_caps, IDirectMusicPort **port);
extern HRESULT midi_in_port_create(IDirectMusic8Impl *parent, DMUS_PORTPARAMS *port_params,
        DMUS_PORTCAPS *port_caps, IDirectMusicPort **port);

/*****************************************************************************
 * IReferenceClockImpl implementation structure
 */
struct IReferenceClockImpl {
    /* IUnknown fields */
    IReferenceClock IReferenceClock_iface;
    LONG ref;

    /* IReferenceClockImpl fields */
    REFERENCE_TIME rtTime;
    DMUS_CLOCKINFO pClockInfo;
};

typedef struct _DMUS_PRIVATE_POOLCUE {
	struct list entry; /* for listing elements */
} DMUS_PRIVATE_POOLCUE, *LPDMUS_PRIVATE_POOLCUE;

struct instrument
{
    IDirectMusicInstrument IDirectMusicInstrument_iface;
    LONG ref;

    GUID id;
    INSTHEADER header;
    WCHAR wszName[DMUS_MAX_NAME];
    /* instrument data */

    struct list articulations;
    struct list regions;
};

static inline struct instrument *impl_from_IDirectMusicInstrument(IDirectMusicInstrument *iface)
{
    return CONTAINING_RECORD(iface, struct instrument, IDirectMusicInstrument_iface);
}

/*****************************************************************************
 * Misc.
 */
void dmusic_remove_port(IDirectMusic8Impl *dmusic, IDirectMusicPort *port);

/* for simpler reading */
typedef struct _DMUS_PRIVATE_CHUNK {
	FOURCC fccID; /* FOURCC ID of the chunk */
	DWORD dwSize; /* size of the chunk */
} DMUS_PRIVATE_CHUNK, *LPDMUS_PRIVATE_CHUNK;

/* used for generic dumping (copied from ddraw) */
typedef struct {
    DWORD val;
    const char* name;
} flag_info;

#define FE(x) { x, #x }	

/* dwPatch from MIDILOCALE */
extern DWORD MIDILOCALE2Patch (const MIDILOCALE *pLocale);
/* MIDILOCALE from dwPatch */
extern void Patch2MIDILOCALE (DWORD dwPatch, LPMIDILOCALE pLocale);

/* check whether the given DWORD is even (return 0) or odd (return 1) */
extern int even_or_odd (DWORD number);
/* Dump whole DMUS_PORTPARAMS struct */
extern void dump_DMUS_PORTPARAMS(LPDMUS_PORTPARAMS params);

#endif /* __WINE_DMUSIC_PRIVATE_H */
