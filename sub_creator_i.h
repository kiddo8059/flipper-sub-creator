#pragma once

#include "scenes/sub_creator_scene.h"
#include "helpers/sub_creator_types.h"
#include "helpers/sub_creator_gen_info.h"
#include "helpers/sub_creator_txrx.h"

#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>

#include <dialogs/dialogs.h>
#include <notification/notification_messages.h>

#include <lib/toolbox/path.h>

#include <gui/modules/submenu.h>
#include <gui/modules/popup.h>
#include <gui/modules/byte_input.h>
#include <gui/modules/text_input.h>
#include <gui/modules/widget.h>

#include <lib/subghz/types.h>

#include <sub_creator_icons.h>

#define SUB_CREATOR_MAX_LEN_NAME 64

typedef struct {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    SceneManager* scene_manager;
    DialogsApp* dialogs;
    NotificationApp* notifications;

    Submenu* submenu;
    Popup* popup;
    ByteInput* byte_input;
    TextInput* text_input;
    Widget* widget;

    SubCreatorTxRx* txrx;
    GenInfo* gen_info;

    bool save_datetime_set;
    DateTime save_datetime;

    FuriString* error_str;

    FuriString* file_path;
    FuriString* file_path_tmp;
    char file_name_tmp[SUB_CREATOR_MAX_LEN_NAME];
} SubCreator;

#ifdef __cplusplus
extern "C" {
#endif

void sub_creator_file_name_clear(SubCreator* app);
bool sub_creator_path_is_file(FuriString* path);
bool sub_creator_rename_file(SubCreator* app);
bool sub_creator_save_protocol_to_file(SubCreator* app, FlipperFormat* flipper_format, const char* dev_file_name);

#ifdef __cplusplus
}
#endif
