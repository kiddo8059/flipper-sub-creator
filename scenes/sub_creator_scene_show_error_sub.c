#include "../sub_creator_i.h"
#include "../helpers/sub_creator_custom_event.h"

static void sub_creator_scene_show_error_sub_popup_callback(void* context) {
    furi_assert(context);

    SubCreator* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, SubCreatorCustomEventSceneShowErrorSub);
}

void sub_creator_scene_show_error_sub_on_enter(void* context) {
    furi_assert(context);

    SubCreator* app = context;

    popup_set_icon(app->popup, 83, 22, &I_WarningDolphinFlip_45x42);
    popup_set_header(app->popup, furi_string_get_cstr(app->error_str), 14, 15, AlignLeft, AlignTop);
    popup_set_timeout(app->popup, 1500);
    popup_set_context(app->popup, app);
    popup_set_callback(app->popup, sub_creator_scene_show_error_sub_popup_callback);
    popup_enable_timeout(app->popup);
    view_dispatcher_switch_to_view(app->view_dispatcher, SubCreatorViewPopup);

    notification_message(app->notifications, &sequence_set_red_255);
}

bool sub_creator_scene_show_error_sub_on_event(void* context, SceneManagerEvent event) {
    furi_assert(context);

    SubCreator* app = context;
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == SubCreatorCustomEventSceneShowErrorSub) {
            scene_manager_search_and_switch_to_previous_scene(app->scene_manager, SubCreatorSceneStart);
            return true;
        }
    }
    return false;
}

void sub_creator_scene_show_error_sub_on_exit(void* context) {
    furi_assert(context);

    SubCreator* app = context;
    popup_reset(app->popup);
    furi_string_reset(app->error_str);
    notification_message(app->notifications, &sequence_reset_rgb);
}
