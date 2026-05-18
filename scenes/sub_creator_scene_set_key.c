#include "../sub_creator_i.h"
#include "../helpers/sub_creator_txrx_create_protocol_key.h"

#include <machine/endian.h>

static void sub_creator_scene_set_key_byte_input_callback(void* context) {
    furi_assert(context);

    SubCreator* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, SubCreatorCustomEventByteInputDone);
}

void sub_creator_scene_set_key_on_enter(void* context) {
    furi_assert(context);

    SubCreator* app = context;

    uint8_t* byte_ptr = NULL;
    uint8_t byte_count = 0;

    if(app->gen_info->type == GenData) {
        byte_ptr = (uint8_t*)&app->gen_info->data.key;
        byte_count = sizeof(app->gen_info->data.key);
    } else {
        furi_crash("Not implemented");
    }

    furi_assert(byte_ptr);
    furi_assert(byte_count > 0);

    *((uint64_t*)byte_ptr) = __bswap64(*((uint64_t*)byte_ptr));

    ByteInput* byte_input = app->byte_input;
    byte_input_set_header_text(byte_input, "Enter KEY in hex");
    byte_input_set_result_callback(
        byte_input, sub_creator_scene_set_key_byte_input_callback, NULL, app, byte_ptr, byte_count);
    view_dispatcher_switch_to_view(app->view_dispatcher, SubCreatorViewByteInput);
}

bool sub_creator_scene_set_key_on_event(void* context, SceneManagerEvent event) {
    SubCreator* app = context;
    bool consumed = false;
    bool generated_protocol = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == SubCreatorCustomEventByteInputDone) {
            if(app->gen_info->type == GenData) {
                app->gen_info->data.key = __bswap64(app->gen_info->data.key);

                if(app->gen_info->data.te) {
                    generated_protocol = sub_creator_txrx_gen_data_protocol_and_te(
                        app->txrx,
                        app->gen_info->mod,
                        app->gen_info->freq,
                        app->gen_info->data.name,
                        app->gen_info->data.key,
                        app->gen_info->data.bits,
                        app->gen_info->data.te);
                } else {
                    generated_protocol = sub_creator_txrx_gen_data_protocol(
                        app->txrx,
                        app->gen_info->mod,
                        app->gen_info->freq,
                        app->gen_info->data.name,
                        app->gen_info->data.key,
                        app->gen_info->data.bits);
                }
            }

            consumed = true;

            if(!generated_protocol) {
                furi_string_set(app->error_str, "Function requires\nan SD card with\nfresh databases.");
                scene_manager_next_scene(app->scene_manager, SubCreatorSceneShowError);
            } else {
                sub_creator_file_name_clear(app);

                scene_manager_set_scene_state(
                    app->scene_manager, SubCreatorSceneSetType, SubCreatorCustomEventManagerSet);
                scene_manager_next_scene(app->scene_manager, SubCreatorSceneSaveName);
            }
        }
    }
    return consumed;
}

void sub_creator_scene_set_key_on_exit(void* context) {
    furi_assert(context);

    SubCreator* app = context;

    byte_input_set_result_callback(app->byte_input, NULL, NULL, NULL, NULL, 0);
    byte_input_set_header_text(app->byte_input, "");
}
