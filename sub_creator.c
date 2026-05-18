#include "sub_creator_i.h"

static bool sub_creator_custom_event_callback(void* context, uint32_t event) {
    furi_assert(context);
    SubCreator* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, event);
}

static bool sub_creator_back_event_callback(void* context) {
    furi_assert(context);
    SubCreator* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

SubCreator* sub_creator_alloc() {
    SubCreator* app = malloc(sizeof(SubCreator));

    // View Dispatcher
    app->view_dispatcher = view_dispatcher_alloc();
    app->scene_manager = scene_manager_alloc(&sub_creator_scene_handlers, app);

    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, sub_creator_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, sub_creator_back_event_callback);

    // Open GUI Record
    app->gui = furi_record_open(RECORD_GUI);
    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    // Open Notifications Record
    app->notifications = furi_record_open(RECORD_NOTIFICATION);

    // Open Dialogs Record
    app->dialogs = furi_record_open(RECORD_DIALOGS);

    // SubMenu
    app->submenu = submenu_alloc();
    view_dispatcher_add_view(app->view_dispatcher, SubCreatorViewSubmenu, submenu_get_view(app->submenu));

    // Popup
    app->popup = popup_alloc();
    view_dispatcher_add_view(app->view_dispatcher, SubCreatorViewPopup, popup_get_view(app->popup));

    // Byte Input
    app->byte_input = byte_input_alloc();
    view_dispatcher_add_view(app->view_dispatcher, SubCreatorViewByteInput, byte_input_get_view(app->byte_input));

    // Text Input
    app->text_input = text_input_alloc();
    view_dispatcher_add_view(app->view_dispatcher, SubCreatorViewTextInput, text_input_get_view(app->text_input));

    // Custom Widget
    app->widget = widget_alloc();
    view_dispatcher_add_view(app->view_dispatcher, SubCreatorViewWidget, widget_get_view(app->widget));

    // TxRx
    app->txrx = sub_creator_txrx_alloc();

    app->gen_info = malloc(sizeof(GenInfo));

    app->file_path = furi_string_alloc();
    furi_string_set(app->file_path, SUBGHZ_APP_FOLDER);

    app->file_path_tmp = furi_string_alloc();
    app->error_str = furi_string_alloc();

    return app;
}

void sub_creator_free(SubCreator* app) {
    furi_assert(app);

    // Submenu
    view_dispatcher_remove_view(app->view_dispatcher, SubCreatorViewSubmenu);
    submenu_free(app->submenu);

    // Popup
    view_dispatcher_remove_view(app->view_dispatcher, SubCreatorViewPopup);
    popup_free(app->popup);

    // ByteInput
    view_dispatcher_remove_view(app->view_dispatcher, SubCreatorViewByteInput);
    byte_input_free(app->byte_input);

    // TextInput
    view_dispatcher_remove_view(app->view_dispatcher, SubCreatorViewTextInput);
    text_input_free(app->text_input);

    // Custom Widget
    view_dispatcher_remove_view(app->view_dispatcher, SubCreatorViewWidget);
    widget_free(app->widget);

    // Scene manager
    scene_manager_free(app->scene_manager);

    // View Dispatcher
    view_dispatcher_free(app->view_dispatcher);

    // Close GUI Record
    furi_record_close(RECORD_GUI);
    app->gui = NULL;

    // Close Notifications Record
    furi_record_close(RECORD_NOTIFICATION);
    app->notifications = NULL;

    // Close Dialogs Record
    furi_record_close(RECORD_DIALOGS);
    app->dialogs = NULL;

    // TxRx
    sub_creator_txrx_free(app->txrx);

    free(app->gen_info);

    furi_string_free(app->error_str);
    furi_string_free(app->file_path);
    furi_string_free(app->file_path_tmp);

    free(app);
}

int32_t sub_creator_app(void* p) {
    UNUSED(p);

    SubCreator* app = sub_creator_alloc();

    scene_manager_next_scene(app->scene_manager, SubCreatorSceneStart);

    view_dispatcher_run(app->view_dispatcher);

    sub_creator_free(app);

    return 0;
}
