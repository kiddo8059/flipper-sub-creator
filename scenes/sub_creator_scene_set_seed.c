#include "../sub_creator_i.h"
#include "../helpers/sub_creator_txrx_create_protocol_key.h"

#include <machine/endian.h>

static void sub_creator_scene_set_seed_byte_input_callback(void* context) {
    furi_assert(context);

    SubCreator* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, SubCreatorCustomEventByteInputDone);
}

void sub_creator_scene_set_seed_on_enter(void* context) {
    furi_assert(context);

    SubCreator* app = context;

    uint8_t* byte_ptr = NULL;
    uint8_t byte_count = 0;

    switch(app->gen_info->type) {
    case GenFaacSLH:
        byte_ptr = (uint8_t*)&app->gen_info->faac_slh.seed;
        byte_count = sizeof(app->gen_info->faac_slh.seed);
        break;
    case GenKeeloqSeed:
        byte_ptr = (uint8_t*)&app->gen_info->keeloq_seed.seed;
        byte_count = sizeof(app->gen_info->keeloq_seed.seed);
        break;
    case GenKeeloq:
    case GenAlutechAt4n:
    case GenSomfyTelis:
    case GenSomfyKeytis:
    case GenKingGatesStylo4k:
    case GenBenincaARC:
    case GenJarolift:
    case GenDitecGOL4:
    case GenNiceFlorS:
    case GenSecPlus2:
    case GenPhoenixV2:
    case GenData:
    case GenSecPlus1:
    case GenCameAtomo:
    default:
        furi_crash("Not implemented");
        break;
    }

    furi_assert(byte_ptr);
    furi_assert(byte_count > 0);

    *((uint32_t*)byte_ptr) = __bswap32(*((uint32_t*)byte_ptr));

    ByteInput* byte_input = app->byte_input;
    byte_input_set_header_text(byte_input, "Enter SEED in hex");
    byte_input_set_result_callback(
        byte_input, sub_creator_scene_set_seed_byte_input_callback, NULL, app, byte_ptr, byte_count);
    view_dispatcher_switch_to_view(app->view_dispatcher, SubCreatorViewByteInput);
}

bool sub_creator_scene_set_seed_on_event(void* context, SceneManagerEvent event) {
    furi_assert(context);

    SubCreator* app = context;

    bool consumed = false;
    bool generated_protocol = false;
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == SubCreatorCustomEventByteInputDone) {
            switch(app->gen_info->type) {
            case GenFaacSLH:
                app->gen_info->faac_slh.seed = __bswap32(app->gen_info->faac_slh.seed);
                generated_protocol = sub_creator_txrx_gen_faac_slh_protocol(
                    app->txrx,
                    app->gen_info->mod,
                    app->gen_info->freq,
                    app->gen_info->faac_slh.serial,
                    app->gen_info->faac_slh.btn,
                    app->gen_info->faac_slh.cnt,
                    app->gen_info->faac_slh.seed,
                    app->gen_info->faac_slh.manuf);
                break;
            case GenKeeloqSeed:
                app->gen_info->keeloq_seed.seed = __bswap32(app->gen_info->keeloq_seed.seed);
                generated_protocol = sub_creator_txrx_gen_keeloq_seed_protocol(
                    app->txrx,
                    app->gen_info->mod,
                    app->gen_info->freq,
                    app->gen_info->keeloq_seed.serial,
                    app->gen_info->keeloq_seed.btn,
                    app->gen_info->keeloq_seed.cnt,
                    app->gen_info->keeloq_seed.seed,
                    app->gen_info->keeloq_seed.manuf);
                break;
            case GenKeeloq:
            case GenAlutechAt4n:
            case GenSomfyTelis:
            case GenSomfyKeytis:
            case GenKingGatesStylo4k:
            case GenBenincaARC:
            case GenJarolift:
            case GenDitecGOL4:
            case GenNiceFlorS:
            case GenSecPlus2:
            case GenPhoenixV2:
            case GenData:
            case GenSecPlus1:
            case GenCameAtomo:
            default:
                furi_crash("Not implemented");
                break;
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

void sub_creator_scene_set_seed_on_exit(void* context) {
    furi_assert(context);

    SubCreator* app = context;

    byte_input_set_result_callback(app->byte_input, NULL, NULL, NULL, NULL, 0);
    byte_input_set_header_text(app->byte_input, "");
}
