#include "../sub_creator_i.h"
#include "../helpers/sub_creator_txrx_create_protocol_key.h"

static void sub_creator_scene_set_button_byte_input_callback(void* context) {
    furi_assert(context);

    SubCreator* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, SubCreatorCustomEventByteInputDone);
}

void sub_creator_scene_set_button_on_enter(void* context) {
    furi_assert(context);

    SubCreator* app = context;

    uint8_t* byte_ptr = NULL;
    uint8_t byte_count = 0;

    switch(app->gen_info->type) {
    case GenFaacSLH:
        byte_ptr = &app->gen_info->faac_slh.btn;
        byte_count = sizeof(app->gen_info->faac_slh.btn);
        break;
    case GenKeeloq:
        byte_ptr = &app->gen_info->keeloq.btn;
        byte_count = sizeof(app->gen_info->keeloq.btn);
        break;
    case GenKeeloqSeed:
        byte_ptr = &app->gen_info->keeloq_seed.btn;
        byte_count = sizeof(app->gen_info->keeloq_seed.btn);
        break;
    case GenAlutechAt4n:
        byte_ptr = &app->gen_info->alutech_at_4n.btn;
        byte_count = sizeof(app->gen_info->alutech_at_4n.btn);
        break;
    case GenSomfyTelis:
        byte_ptr = &app->gen_info->somfy_telis.btn;
        byte_count = sizeof(app->gen_info->somfy_telis.btn);
        break;
    case GenSomfyKeytis:
        byte_ptr = &app->gen_info->somfy_keytis.btn;
        byte_count = sizeof(app->gen_info->somfy_keytis.btn);
        break;
    case GenKingGatesStylo4k:
        byte_ptr = &app->gen_info->kinggates_stylo_4k.btn;
        byte_count = sizeof(app->gen_info->kinggates_stylo_4k.btn);
        break;
    case GenBenincaARC:
        byte_ptr = &app->gen_info->beninca_arc.btn;
        byte_count = sizeof(app->gen_info->beninca_arc.btn);
        break;
    case GenJarolift:
        byte_ptr = &app->gen_info->jarolift.btn;
        byte_count = sizeof(app->gen_info->jarolift.btn);
        break;
    case GenDitecGOL4:
        byte_ptr = &app->gen_info->ditec_gol4.btn;
        byte_count = sizeof(app->gen_info->ditec_gol4.btn);
        break;
    case GenNiceFlorS:
        byte_ptr = &app->gen_info->nice_flor_s.btn;
        byte_count = sizeof(app->gen_info->nice_flor_s.btn);
        break;
    case GenSecPlus2:
        byte_ptr = &app->gen_info->sec_plus_2.btn;
        byte_count = sizeof(app->gen_info->sec_plus_2.btn);
        break;
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

    ByteInput* byte_input = app->byte_input;
    byte_input_set_header_text(byte_input, "Enter BUTTON in hex");
    byte_input_set_result_callback(
        byte_input, sub_creator_scene_set_button_byte_input_callback, NULL, app, byte_ptr, byte_count);
    view_dispatcher_switch_to_view(app->view_dispatcher, SubCreatorViewByteInput);
}

bool sub_creator_scene_set_button_on_event(void* context, SceneManagerEvent event) {
    furi_assert(context);

    SubCreator* app = context;

    bool consumed = false;
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == SubCreatorCustomEventByteInputDone) {
            switch(app->gen_info->type) {
            case GenFaacSLH:
            case GenKeeloq:
            case GenKeeloqSeed:
            case GenAlutechAt4n:
            case GenSomfyTelis:
            case GenKingGatesStylo4k:
            case GenBenincaARC:
            case GenJarolift:
            case GenDitecGOL4:
            case GenNiceFlorS:
            case GenSomfyKeytis:
            case GenSecPlus2:
                scene_manager_next_scene(app->scene_manager, SubCreatorSceneSetCounter);
                break;
            case GenCameAtomo:
            case GenPhoenixV2:
            case GenData:
            case GenSecPlus1:
            default:
                furi_crash("Not implemented");
                break;
            }

            consumed = true;
        }
    }
    return consumed;
}

void sub_creator_scene_set_button_on_exit(void* context) {
    furi_assert(context);

    SubCreator* app = context;

    byte_input_set_result_callback(app->byte_input, NULL, NULL, NULL, NULL, 0);
    byte_input_set_header_text(app->byte_input, "");
}
