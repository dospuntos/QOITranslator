/*
 * ConfigView.h – QOI image translator for Haiku
 *
 * Copyright (c) 2026 Johan Wagenheim <johan@dospuntos.no>
 *
 * Distributed under the terms of the MIT License.
 */

#ifndef CONFIGVIEW_H
#define CONFIGVIEW_H


#include <View.h>


class ConfigView : public BView {
public:
			ConfigView(uint32 flags = B_WILL_DRAW);
			virtual ~ConfigView();
};

#endif // CONFIGVIEW_H
