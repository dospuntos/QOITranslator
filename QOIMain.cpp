/*
 * QOIMain.cpp – QOI image translator for Haiku
 * Version 0.2.0 (May 2026)
 *
 * Copyright (c) 2026 Johan Wagenheim <johan@dospuntos.no>
 *
 * Distributed under the terms of the MIT License.
 * See the accompanying LICENSE file for details.
 */

#include <Application.h>
#include <Catalog.h>

#include "QOITranslator.h"
#include "shared/TranslatorWindow.h"

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "main"


int
main()
{
	BApplication app("application/x-vnd.Haiku-QOITranslator");
	if (LaunchTranslatorWindow(new QOITranslator, B_TRANSLATE("QOI Settings")) != B_OK)
		return 1;

	app.Run();
	return 0;
}
