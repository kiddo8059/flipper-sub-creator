#include "../sub_creator_i.h"

#include <toolbox/name_generator.h>

static void sub_creator_scene_save_name_text_input_callback(void* context) {
    furi_assert(context);

    SubCreator* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, SubCreatorCustomEventSceneSaveName);
}

void sub_creator_scene_save_name_on_enter(void* context) {
    furi_assert(context);

    SubCreator* app = context;

    TextInput* text_input = app->text_input;
    bool dev_name_empty = false;

    FuriString* tmp_str = furi_string_alloc();
    FuriString* file_name = furi_string_alloc();
    FuriString* dir_name = furi_string_alloc();

    char file_name_buf[SUB_CREATOR_MAX_LEN_NAME] = {0};
    DateTime* datetime = app->save_datetime_set ? &app->save_datetime : NULL;
    app->save_datetime_set = false;
    if(!sub_creator_path_is_file(app->file_path)) {
        FlipperFormat* fff_data = sub_creator_txrx_get_fff_data(app->txrx);

        flipper_format_rewind(fff_data);
        if(flipper_format_key_exist(fff_data, "Manufacture")) {
            flipper_format_read_string(fff_data, "Manufacture", tmp_str);
        } else if(flipper_format_key_exist(fff_data, "Protocol")) {
            flipper_format_read_string(fff_data, "Protocol", tmp_str);
        }

        if(furi_string_empty(tmp_str)) {
            name_generator_make_auto_datetime(file_name_buf, SUB_CREATOR_MAX_LEN_NAME, NULL, datetime);
        } else {
            furi_string_replace_all_str(tmp_str, " ", "_");
            name_generator_make_auto_datetime(
                file_name_buf, SUB_CREATOR_MAX_LEN_NAME, furi_string_get_cstr(tmp_str), datetime);
        }

        furi_string_set(file_name, file_name_buf);
        furi_string_set(app->file_path, SUBGHZ_APP_FOLDER);
        dev_name_empty = true;
    } else {
        furi_string_reset(app->file_path_tmp);
        furi_string_set(app->file_path_tmp, app->file_path);
        path_extract_dirname(furi_string_get_cstr(app->file_path), dir_name);
        path_extract_filename(app->file_path, file_name, true);
        furi_string_set(app->file_path, dir_name);
    }

    strncpy(app->file_name_tmp, furi_string_get_cstr(file_name), SUB_CREATOR_MAX_LEN_NAME);
    text_input_set_header_text(text_input, "Name signal");
    text_input_set_result_callback(
        text_input,
        sub_creator_scene_save_name_text_input_callback,
        app,
        app->file_name_tmp,
        SUB_CREATOR_MAX_LEN_NAME,
        dev_name_empty);

    ValidatorIsFile* validator_is_file =
        validator_is_file_alloc_init(furi_string_get_cstr(app->file_path), SUBGHZ_APP_FILENAME_EXTENSION, "");
    text_input_set_validator(text_input, validator_is_file_callback, validator_is_file);

    furi_string_free(tmp_str);
    furi_string_free(file_name);
    furi_string_free(dir_name);

    view_dispatcher_switch_to_view(app->view_dispatcher, SubCreatorViewTextInput);
}

bool sub_creator_scene_save_name_on_event(void* context, SceneManagerEvent event) {
    furi_assert(context);

    SubCreator* app = context;

    if(event.type == SceneManagerEventTypeBack) {
        furi_string_set(app->file_path, SUBGHZ_APP_FOLDER);

        scene_manager_previous_scene(app->scene_manager);

        return true;
    } else if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == SubCreatorCustomEventSceneSaveName) {
            if(strcmp(app->file_name_tmp, "") != 0) {
                furi_string_cat_printf(app->file_path, "/%s%s", app->file_name_tmp, SUBGHZ_APP_FILENAME_EXTENSION);
                if(sub_creator_path_is_file(app->file_path_tmp)) {
                    if(!sub_creator_rename_file(app)) {
                        return false;
                    }
                } else {
                    if(scene_manager_get_scene_state(app->scene_manager, SubCreatorSceneSetType) !=
                       SubCreatorCustomEventManagerNoSet) {
                        sub_creator_save_protocol_to_file(
                            app, sub_creator_txrx_get_fff_data(app->txrx), furi_string_get_cstr(app->file_path));
                        scene_manager_set_scene_state(
                            app->scene_manager, SubCreatorSceneSetType, SubCreatorCustomEventManagerNoSet);
                    }
                }

                sub_creator_file_name_clear(app);

                scene_manager_next_scene(app->scene_manager, SubCreatorSceneSaveSuccess);

                return true;
            } else {
                furi_string_set(app->error_str, "No name file");
                scene_manager_next_scene(app->scene_manager, SubCreatorSceneShowErrorSub);
                return true;
            }
        }
    }
    return false;
}

void sub_creator_scene_save_name_on_exit(void* context) {
    furi_assert(context);

    SubCreator* app = context;

    void* validator_context = text_input_get_validator_callback_context(app->text_input);
    text_input_set_validator(app->text_input, NULL, NULL);
    validator_is_file_free(validator_context);

    text_input_reset(app->text_input);
}
