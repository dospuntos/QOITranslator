/*
 * QOITranslator.cpp – QOI image translator for Haiku
 *
 * Copyright (c) 2026 Johan Wagenheim <johan@dospuntos.no>
 *
 * Distributed under the terms of the MIT License.
 */

#define QOI_IMPLEMENTATION

// Includes
#include "QOITranslator.h"
#include "ConfigView.h"
#include "qoi.h"
#include <Bitmap.h>
#include <Catalog.h>
#include <DataIO.h>
#include <File.h>
#include <FindDirectory.h>
#include <Path.h>
#include <Screen.h>
#include <SupportKit.h>
#include <TranslatorAddOn.h>
#include <TranslatorFormats.h>
#include <string.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "QOITranslator"


// The input formats that this translator supports.
static const translation_format sInputFormats[] = {
	{
		QOI_IMAGE_FORMAT,
		B_TRANSLATOR_BITMAP,
		1,
		1,
		"image/qoi",
		"QOI"
	}
};

// The output formats that this translator supports.
static const translation_format sOutputFormats[] = {
	{
		B_TRANSLATOR_BITMAP,
		B_TRANSLATOR_BITMAP,
		1,
		1,
		"image/x-be-bitmap",
		"Be Bitmap Format (QOITranslator)"
	},
};

// Default settings for the Translator
static const TranSetting sDefaultSettings[] = {
	{B_TRANSLATOR_EXT_HEADER_ONLY, TRAN_SETTING_BOOL, false},
	{B_TRANSLATOR_EXT_DATA_ONLY, TRAN_SETTING_BOOL, false}
};

const uint32 kNumInputFormats = sizeof(sInputFormats) / sizeof(translation_format);
const uint32 kNumOutputFormats = sizeof(sOutputFormats) / sizeof(translation_format);
const uint32 kNumDefaultSettings = sizeof(sDefaultSettings) / sizeof(TranSetting);

QOITranslator::QOITranslator()
		: BaseTranslator(B_TRANSLATE("QOI images"),
				B_TRANSLATE("QOI image translator"),
				QOI_TRANSLATOR_VERSION,
				sInputFormats, kNumInputFormats,
				sOutputFormats, kNumOutputFormats,
				"QOITranslator_Settings",
				sDefaultSettings, kNumDefaultSettings,
				B_TRANSLATOR_BITMAP, QOI_IMAGE_FORMAT)
{
}


QOITranslator::~QOITranslator()
{
}


status_t
QOITranslator::DerivedIdentify(
	BPositionIO* inSource,
	const translation_format* inFormat,
	BMessage* /*ioExtension*/,
	translator_info* outInfo,
	uint32 outType)
{

	if (outType && (outType != B_TRANSLATOR_BITMAP))
		return B_NO_TRANSLATOR;

	// Check for QOI magic bytes ("qoif" at offset 0)
	char magic[4];
	ssize_t readSize = inSource->Read(magic, 4);
	if (readSize < 4)
		return B_NO_TRANSLATOR;

	if (memcmp(magic, "qoif", 4) == 0) {
		inFormat = &sInputFormats[0];
		outInfo->type = inFormat->type;
		outInfo->group = inFormat->group;
		outInfo->quality = inFormat->quality;
		outInfo->capability = inFormat->capability;
		strcpy(outInfo->name, inFormat->name);
		strcpy(outInfo->MIME, inFormat->MIME);

		return B_OK;
	}

	return B_NO_TRANSLATOR;
}


status_t
QOITranslator::DerivedTranslate (
	BPositionIO* source,
	const translator_info* info,
	BMessage* /*ioExtension*/,
	uint32 outType,
	BPositionIO* target, int32 baseType)
{
	status_t ret_val = B_OK;

	// Check types
	if (info->type != QOI_IMAGE_FORMAT)
		return B_NO_TRANSLATOR;

	if (outType == 0)
		outType = B_TRANSLATOR_BITMAP;

	if (outType != B_TRANSLATOR_BITMAP)
		return B_NO_TRANSLATOR;

	// Read entire file into memory
	off_t fileSize = source->Seek(0, SEEK_END);
	source->Seek(0, SEEK_SET);

	uint8_t* buffer = new uint8_t[fileSize];
	if (source->Read(buffer, fileSize) != fileSize) {
		delete[] buffer;
		return B_ERROR;
	}

	// Decode QOI
	qoi_desc desc;
	void* pixels = qoi_decode(buffer, fileSize, &desc, 4); // force RGBA
	delete[] buffer;

	if (!pixels)
		return B_ERROR;

	int width = desc.width;
	int height = desc.height;

	// Create Haiku bitmap
	BBitmap* bitmap = new BBitmap(BRect(0, 0, width - 1, height - 1), B_RGBA32);
	if (!bitmap->IsValid()) {
		free(pixels);
		delete bitmap;
		return B_ERROR;
	}

	uint8* dest = (uint8*)bitmap->Bits();
	uint8* src = (uint8*)pixels;

	for (int i = 0; i < width * height; i++) {
		uint8 r = *src++;
		uint8 g = *src++;
		uint8 b = *src++;
		uint8 a = *src++;

		*dest++ = b;
		*dest++ = g;
		*dest++ = r;
		*dest++ = a;
	}

	free(pixels);

	// Prepare TranslatorBitmap header
	TranslatorBitmap bmp;
	bmp.magic = B_TRANSLATOR_BITMAP;
	bmp.bounds = bitmap->Bounds();
	bmp.rowBytes = bitmap->BytesPerRow();
	bmp.colors = B_RGBA32;
	bmp.dataSize = bitmap->BitsLength();

	// Convert header to correct endianness
	swap_data(B_UINT32_TYPE, &(bmp.magic), sizeof(uint32), B_SWAP_HOST_TO_BENDIAN);
	swap_data(B_RECT_TYPE, &(bmp.bounds), sizeof(BRect), B_SWAP_HOST_TO_BENDIAN);
	swap_data(B_UINT32_TYPE, &(bmp.rowBytes), sizeof(uint32), B_SWAP_HOST_TO_BENDIAN);
	swap_data(B_UINT32_TYPE, &(bmp.colors), sizeof(color_space), B_SWAP_HOST_TO_BENDIAN);
	swap_data(B_UINT32_TYPE, &(bmp.dataSize), sizeof(uint32), B_SWAP_HOST_TO_BENDIAN);

	// Write output
	if (target->Write(&bmp, sizeof(TranslatorBitmap)) != sizeof(TranslatorBitmap)
		|| target->Write(bitmap->Bits(), bitmap->BitsLength()) != bitmap->BitsLength()) {
		ret_val = B_ERROR;
	}

	delete bitmap;
	return ret_val;
}


BView*
QOITranslator::NewConfigView(TranslatorSettings* settings)
{
	return new ConfigView();
}


BTranslator*
make_nth_translator(int32 n, image_id you, uint32 flags, ...)
{
	if (n != 0)
		return NULL;

	return new QOITranslator();
}
