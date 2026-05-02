/*
 * QOITranslator.h – QOI image translator for Haiku
 *
 * Copyright (c) 2026 Johan Wagenheim <johan@dospuntos.no>
 *
 * Distributed under the terms of the MIT License.
 */

#ifndef QOITRANSLATOR_H
#define QOITRANSLATOR_H

#include "shared/BaseTranslator.h"
#include "shared/TranslatorSettings.h"
#include <DataIO.h>
#include <Message.h>
#include <SupportDefs.h>
#include <TranslationDefs.h>

#define QOI_TRANSLATOR_VERSION B_TRANSLATION_MAKE_VERSION(0, 2, 0)
#define QOI_IMAGE_FORMAT 'qoif'

class QOITranslator : public BaseTranslator {
public:
				QOITranslator();
				virtual ~QOITranslator();

				virtual status_t DerivedIdentify(BPositionIO *inSource,
					const translation_format *inFormat, BMessage *ioExtension,
					translator_info *outInfo, uint32 outType);

				virtual status_t DerivedTranslate(BPositionIO *inSource,
					const translator_info *inInfo, BMessage *ioExtension,
					uint32 outType, BPositionIO *outDestination, int32 baseType);

				virtual BView *NewConfigView(TranslatorSettings *settings);

private:
};

#endif // QOITRANSLATOR_H
