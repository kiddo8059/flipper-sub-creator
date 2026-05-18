#include "../sub_creator_i.h"
#include "../helpers/sub_creator_custom_event.h"

static void sub_creator_scene_save_success_popup_callback(void* context) {
    furi_assert(context);

    SubCreator* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, SubCreatorCustomEventSceneSaveSuccess);
}

void sub_creator_scene_save_success_on_enter(void* context) {
    furi_assert(context);

    SubCreator* app = context;

    Popup* popup = app->popup;
    popup_set_icon(popup, 36, 5, &I_DolphinSaved_92x58);
    popup_set_header(popup, "Saved", 15, 19, AlignLeft, AlignBottom);
    popup_set_timeout(popup, 1500);
    popup_set_context(popup, app);
    popup_set_callback(popup, sub_creator_scene_save_success_popup_callback);
    popup_enable_timeout(popup);
    view_dispatcher_switch_to_view(app->view_dispatcher, SubCreatorViewPopup);
}

bool sub_creator_scene_save_success_on_event(void* context, SceneManagerEvent event) {
    furi_assert(context);

    SubCreator* app = context;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == SubCreatorCustomEventSceneSaveSuccess) {
            scene_manager_search_and_switch_to_previous_scene(app->scene_manager, SubCreatorSceneStart);

            return true;
        }
    }
    return false;
}

void sub_creator_scene_save_success_on_exit(void* context) {
    furi_assert(context);

    SubCreator* app = context;

    popup_reset(app->popup);
}
