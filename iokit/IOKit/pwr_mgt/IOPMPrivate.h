/*
 * Copyright (c) 2002 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * The contents of this file constitute Original Code as defined in and
 * are subject to the Apple Public Source License Version 1.1 (the
 * "License").  You may not use this file except in compliance with the
 * License.  Please obtain a copy of the License at
 * http://www.apple.com/publicsource and read it before using this file.
 * 
 * This Original Code and all software distributed under the License are
 * distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
#ifndef _IOKIT_IOPMPRIVATE_H
#define _IOKIT_IOPMPRIVATE_H

#include <IOKit/pwr_mgt/IOPM.h>

// Private power commands issued to root domain
// bits 0-7 in IOPM.h

enum {
    kIOPMSetValue		= (1<<16),
    // don't sleep on clamshell closure on a portable with AC connected
    kIOPMSetDesktopMode		= (1<<17),
    // set state of AC adaptor connected
    kIOPMSetACAdaptorConnected	= (1<<18)
};

#endif /* ! _IOKIT_IOPMPRIVATE_H */

