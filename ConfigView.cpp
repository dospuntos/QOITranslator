/*
 * ConfigView.cpp – QOI image translator for Haiku
 *
 * Copyright (c) 2026 Johan Wagenheim <johan@dospuntos.no>
 *
 * Distributed under the terms of the MIT License.
 */


#include "ConfigView.h"
#include "QOITranslator.h"
#include <Catalog.h>
#include <LayoutBuilder.h>
#include <StringView.h>
#include <cstdio>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "ConfigView"


ConfigView::ConfigView(uint32 flags)
	:
	BView("QOITranslator Settings", flags)
{
	SetViewUIColor(B_PANEL_BACKGROUND_COLOR);

	BStringView* titleView = new BStringView("title", B_TRANSLATE("QOI image translator"));
	titleView->SetFont(be_bold_font);

	char version[256];
	sprintf(version, B_TRANSLATE("Version %d.%d.%d, %s"),
		int(B_TRANSLATION_MAJOR_VERSION(QOI_TRANSLATOR_VERSION)),
		int(B_TRANSLATION_MINOR_VERSION(QOI_TRANSLATOR_VERSION)),
		int(B_TRANSLATION_REVISION_VERSION(QOI_TRANSLATOR_VERSION)), __DATE__);
	BStringView* versionView = new BStringView("version", version);

	BStringView* copyrightView
		= new BStringView("copyright ", B_UTF8_COPYRIGHT " 2026 Johan Wagenheim");

	BString qoiInfo = B_TRANSLATE("Based on QOI (Quite OK Image) implementation");
	BStringView* copyrightView2 = new BStringView("copyright2", qoiInfo.String());

	BStringView* copyrightView3
		= new BStringView("copyright3", "QOI format by Dominic Szablewski:");

	BStringView* copyrightView4 = new BStringView("copyright4", "https://qoiformat.org/");

	// Build the layout
	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.SetInsets(B_USE_DEFAULT_SPACING)
		.Add(titleView)
		.Add(versionView)
		.Add(copyrightView)
		.AddGlue()
		.Add(copyrightView2)
		.Add(copyrightView3)
		.Add(copyrightView4);
}


ConfigView::~ConfigView()
{
}
