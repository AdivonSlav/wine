/*
 * Copyright (c) 2013 André Hentschel
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

#include "windef.h"
#include "winbase.h"
#include "difxapi.h"
#include "wine/debug.h"

WINE_DEFAULT_DEBUG_CHANNEL(difxapi);

DWORD WINAPI DriverPackagePreinstallA(LPCSTR inf, DWORD flags)
{
    FIXME("(%s, %u) stub\n", wine_dbgstr_a(inf), flags);
    return ERROR_SUCCESS;
}

DWORD WINAPI DriverPackagePreinstallW(LPCWSTR inf, DWORD flags)
{
    FIXME("(%s, %u) stub\n", wine_dbgstr_w(inf), flags);
    return ERROR_SUCCESS;
}

DWORD WINAPI DriverPackageInstallA(LPCSTR inf, DWORD flags, PCINSTALLERINFO_A info, BOOL *reboot)
{
    FIXME("(%s, %u, %p, %p) stub\n", wine_dbgstr_a(inf), flags, info, reboot);
    if (reboot) *reboot = FALSE;
    return ERROR_SUCCESS;
}

DWORD WINAPI DriverPackageInstallW(LPCWSTR inf, DWORD flags, PCINSTALLERINFO_W info, BOOL *reboot)
{
    FIXME("(%s, %u, %p, %p) stub\n", wine_dbgstr_w(inf), flags, info, reboot);
    if (reboot) *reboot = FALSE;
    return ERROR_SUCCESS;
}
