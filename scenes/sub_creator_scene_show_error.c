#include "../sub_creator_i.h"
#include "../helpers/sub_creator_custom_event.h"

static const NotificationSequence subghz_sequence_sd_error = {
    &message_red_255,
    &message_green_255,
    &message_do_not_reset,
    NULL,
};

static void sub_creator_scene_show_error_callback(GuiButtonType result, InputType type, void* context) {
    furi_assert(context);

    SubCreator* app = context;

    if((result == GuiButtonTypeRight) && (type == InputTypeShort)) {
        view_dispatcher_send_custom_event(app->view_dispatcher, SubCreatorCustomEventSceneShowErrorOk);
    } else if((result == GuiButtonTypeLeft) && (type == InputTypeShort)) {
        view_dispatcher_send_custom_event(app->view_dispatcher, SubCreatorCustomEventSceneShowErrorBack);
    }
}

void sub_creator_scene_show_error_on_enter(void* context) {
    furi_assert(context);

    SubCreator* app = context;

    widget_add_icon_element(app->widget, 0, 0, &I_SDQuestion_35x43);

    widget_add_string_multiline_element(
        app->widget, 81, 24, AlignCenter, AlignCenter, FontSecondary, furi_string_get_cstr(app->error_str));
    if(scene_manager_get_scene_state(app->scene_manager, SubCreatorSceneShowError) == SubCreatorCustomEventManagerSet) {
        widget_add_button_element(app->widget, GuiButtonTypeRight, "OK", sub_creator_scene_show_error_callback, app);
    } else {
        notification_message(app->notifications, &subghz_sequence_sd_error);
    }

    widget_add_button_element(app->widget, GuiButtonTypeLeft, "Back", sub_creator_scene_show_error_callback, app);

    view_dispatcher_switch_to_view(app->view_dispatcher, SubCreatorViewWidget);
}

bool sub_creator_scene_show_error_on_event(void* context, SceneManagerEvent event) {
    furi_assert(context);

    SubCreator* app = context;
    SubCreatorCustomEvent scene_state = scene_manager_get_scene_state(app->scene_manager, SubCreatorSceneShowError);
    if(event.type == SceneManagerEventTypeBack) {
        if(scene_state == SubCreatorCustomEventManagerSet) {
            return false;
        } else {
            scene_manager_search_and_switch_to_previous_scene(app->scene_manager, SubCreatorSceneStart);
        }
        return true;
    } else if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == SubCreatorCustomEventSceneShowErrorOk) {
            if(scene_state == SubCreatorCustomEventManagerSet) {
                scene_manager_next_scene(app->scene_manager, SubCreatorSceneStart);
            }
            return true;
        } else if(event.event == SubCreatorCustomEventSceneShowErrorBack) {
            if(scene_state == SubCreatorCustomEventManagerSet) {
                if(!scene_manager_previous_scene(app->scene_manager)) {
                    scene_manager_stop(app->scene_manager);
                    view_dispatcher_stop(app->view_dispatcher);
                }
            } else {
                scene_manager_search_and_switch_to_previous_scene(app->scene_manager, SubCreatorSceneStart);
            }
            return true;
        }
    }
    return false;
}

void sub_creator_scene_show_error_on_exit(void* context) {
    furi_assert(context);

    SubCreator* app = context;
    scene_manager_set_scene_state(app->scene_manager, SubCreatorSceneShowError, SubCreatorCustomEventManagerNoSet);
    widget_reset(app->widget);
    furi_string_reset(app->error_str);
    notification_message(app->notifications, &sequence_reset_rgb);
}
