/* -*- Mode: c++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*-
**
** Copyright (C) 1995-2003 Opera Software AS.  All rights reserved.
**
** This file is part of the Opera web browser.  It may not be distributed
** under any circumstances.
**
*/

#ifndef DECODERFACTORYXBM_H
#define DECODERFACTORYXBM_H

#include "modules/img/imagedecoderfactory.h"

class DecoderFactoryXbm : public ImageDecoderFactory
{
public:

#if defined(INTERNAL_XBM_SUPPORT) || defined(ASYNC_IMAGE_DECODERS_EMULATION)
	ImageDecoder* CreateImageDecoder(ImageDecoderListener* listener);
#endif // INTERNAL_XBM_SUPPORT

	BOOL3 CheckSize(const UCHAR* data, INT32 len, INT32& width, INT32& height);
	BOOL3 CheckType(const UCHAR* data, INT32 len);
};

#endif // !DECODERFACTORYXBM_H
