#include "../sub_creator_i.h"
#include "sub_creator_scene_start.h"

static void sub_creator_scene_start_submenu_callback(void* context, uint32_t index) {
    furi_assert(context);

    SubCreator* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void sub_creator_scene_start_on_enter(void* context) {
    furi_assert(context);

    SubCreator* app = context;

    submenu_set_header(app->submenu, "Sub Creator");

    submenu_add_item(app->submenu, "Create", SubmenuIndexCreate, sub_creator_scene_start_submenu_callback, app);
    submenu_add_item(
        app->submenu, "Create [Advanced]", SubmenuIndexCreateAdvanced, sub_creator_scene_start_submenu_callback, app);
    submenu_set_selected_item(app->submenu, scene_manager_get_scene_state(app->scene_manager, SubCreatorSceneStart));

    view_dispatcher_switch_to_view(app->view_dispatcher, SubCreatorViewSubmenu);
}

bool sub_creator_scene_start_on_event(void* context, SceneManagerEvent event) {
    furi_assert(context);

    SubCreator* app = context;

    if(event.type == SceneManagerEventTypeBack) {
        scene_manager_stop(app->scene_manager);
        view_dispatcher_stop(app->view_dispatcher);
        return true;
    } else if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, SubCreatorSceneStart, event.event);
        if(event.event == SubmenuIndexCreate) {
            scene_manager_next_scene(app->scene_manager, SubCreatorSceneSetType);
            return true;
        } else if(event.event == SubmenuIndexCreateAdvanced) {
            scene_manager_next_scene(app->scene_manager, SubCreatorSceneSetType);
            return true;
        }
    }

    return false;
}

void sub_creator_scene_start_on_exit(void* context) {
    furi_assert(context);

    SubCreator* app = context;

    submenu_reset(app->submenu);
}
