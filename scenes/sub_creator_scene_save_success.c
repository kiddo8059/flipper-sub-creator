#include "../sub_creator_i.h"
#include "../helpers/sub_creator_custom_event.h"

#include <loader/loader.h>

#define SUBGHZ_APP_PATH (EXT_PATH("apps/NFC/mfkey.fap"))

static void sub_creator_scene_save_success_widget_callback(GuiButtonType result, InputType type, void* context) {
    furi_assert(context);

    SubCreator* app = context;

    if(type == InputTypeShort && result == GuiButtonTypeLeft) {
        view_dispatcher_send_custom_event(app->view_dispatcher, SubCreatorCustomEventSceneSaveSuccess);
    }

    if(type == InputTypeShort && result == GuiButtonTypeRight) {
        view_dispatcher_send_custom_event(app->view_dispatcher, SubCreatorCustomEventSceneSaveOpenSub);
    }
}

void sub_creator_scene_save_success_on_enter(void* context) {
    furi_assert(context);

    SubCreator* app = context;

    notification_message(app->notifications, &sequence_success);

    widget_add_string_element(app->widget, 64, 10, AlignCenter, AlignTop, FontPrimary, "Saved successfully!");

    widget_add_string_multiline_element(
        app->widget, 64, 28, AlignCenter, AlignTop, FontSecondary, "Do you want to\nopen Sub-GHz app?");

    widget_add_button_element(
        app->widget, GuiButtonTypeLeft, "No", sub_creator_scene_save_success_widget_callback, app);
    widget_add_button_element(
        app->widget, GuiButtonTypeRight, "Yes", sub_creator_scene_save_success_widget_callback, app);

    view_dispatcher_switch_to_view(app->view_dispatcher, SubCreatorViewWidget);
}

bool sub_creator_scene_save_success_on_event(void* context, SceneManagerEvent event) {
    furi_assert(context);

    SubCreator* app = context;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == SubCreatorCustomEventSceneSaveSuccess) {
            scene_manager_search_and_switch_to_previous_scene(app->scene_manager, SubCreatorSceneStart);
            return true;
        } else if(event.event == SubCreatorCustomEventSceneSaveOpenSub) {
            Loader* loader = furi_record_open(RECORD_LOADER);
            loader_enqueue_launch(loader, "Sub-GHz", NULL, LoaderDeferredLaunchFlagNone);
            furi_record_close(RECORD_LOADER);

            scene_manager_stop(app->scene_manager);
            view_dispatcher_stop(app->view_dispatcher);

            return true;
        }
    }

    return false;
}

void sub_creator_scene_save_success_on_exit(void* context) {
    furi_assert(context);

    SubCreator* app = context;

    widget_reset(app->widget);
}
