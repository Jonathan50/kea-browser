/*
  Kea Browser
  Copyright (C) 2015 Kea Browser

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//
// protocols.c
// code for pseudo-protocols

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>

#include "../config.h"
#include "protocols.h"

static void
about_protocol_request(WebKitURISchemeRequest *request, gpointer data)
{
  g_warning("yey it works:DD");
  GInputStream *stream;
  size_t length;
  const char *path;
  char *contents;
  char filename[64];

  g_object_ref(request);

  path = webkit_uri_scheme_request_get_path(request);
  g_strlcpy(filename, DATA_DIR "/", 64);
  g_strlcat(filename, path, 64);
  g_warning("%s, %s", filename, path);

  GError *err = NULL;
  if(!g_file_get_contents(filename, &contents, &length, &err)) {
    g_warning("%s", err->message);
    webkit_uri_scheme_request_finish_error(request, err);
    g_error_free(err);
    return;
  }
  stream = g_memory_input_stream_new_from_data(contents, length, g_free);

  webkit_uri_scheme_request_finish(request, stream, length, "text/html");
  g_object_unref(G_OBJECT(stream));
}

void
register_schemes(WebKitWebContext *context)
{
  webkit_web_context_register_uri_scheme(context, "about",
                                         about_protocol_request,
                                         NULL, NULL);
  g_warning("plz work plz work :O");
}
