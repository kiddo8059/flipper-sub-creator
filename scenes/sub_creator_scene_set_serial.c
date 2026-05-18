#include "../sub_creator_i.h"
#include "../helpers/sub_creator_txrx_create_protocol_key.h"

#include <machine/endian.h>

static void sub_creator_scene_set_serial_byte_input_callback(void* context) {
    furi_assert(context);

    SubCreator* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, SubCreatorCustomEventByteInputDone);
}

void sub_creator_scene_set_serial_on_enter(void* context) {
    furi_assert(context);

    SubCreator* app = context;

    uint8_t* byte_ptr = NULL;
    uint8_t byte_count = 0;

    switch(app->gen_info->type) {
    case GenFaacSLH:
        byte_ptr = (uint8_t*)&app->gen_info->faac_slh.serial;
        byte_count = sizeof(app->gen_info->faac_slh.serial);
        break;
    case GenKeeloq:
        byte_ptr = (uint8_t*)&app->gen_info->keeloq.serial;
        byte_count = sizeof(app->gen_info->keeloq.serial);
        break;
    case GenCameAtomo:
        byte_ptr = (uint8_t*)&app->gen_info->came_atomo.serial;
        byte_count = sizeof(app->gen_info->came_atomo.serial);
        break;
    case GenKeeloqSeed:
        byte_ptr = (uint8_t*)&app->gen_info->keeloq_seed.serial;
        byte_count = sizeof(app->gen_info->keeloq_seed.serial);
        break;
    case GenAlutechAt4n:
        byte_ptr = (uint8_t*)&app->gen_info->alutech_at_4n.serial;
        byte_count = sizeof(app->gen_info->alutech_at_4n.serial);
        break;
    case GenSomfyTelis:
        byte_ptr = (uint8_t*)&app->gen_info->somfy_telis.serial;
        byte_count = sizeof(app->gen_info->somfy_telis.serial);
        break;
    case GenSomfyKeytis:
        byte_ptr = (uint8_t*)&app->gen_info->somfy_keytis.serial;
        byte_count = sizeof(app->gen_info->somfy_keytis.serial);
        break;
    case GenKingGatesStylo4k:
        byte_ptr = (uint8_t*)&app->gen_info->kinggates_stylo_4k.serial;
        byte_count = sizeof(app->gen_info->kinggates_stylo_4k.serial);
        break;
    case GenBenincaARC:
        byte_ptr = (uint8_t*)&app->gen_info->beninca_arc.serial;
        byte_count = sizeof(app->gen_info->beninca_arc.serial);
        break;
    case GenJarolift:
        byte_ptr = (uint8_t*)&app->gen_info->jarolift.serial;
        byte_count = sizeof(app->gen_info->jarolift.serial);
        break;
    case GenDitecGOL4:
        byte_ptr = (uint8_t*)&app->gen_info->ditec_gol4.serial;
        byte_count = sizeof(app->gen_info->ditec_gol4.serial);
        break;
    case GenNiceFlorS:
        byte_ptr = (uint8_t*)&app->gen_info->nice_flor_s.serial;
        byte_count = sizeof(app->gen_info->nice_flor_s.serial);
        break;
    case GenSecPlus2:
        byte_ptr = (uint8_t*)&app->gen_info->sec_plus_2.serial;
        byte_count = sizeof(app->gen_info->sec_plus_2.serial);
        break;
    case GenPhoenixV2:
        byte_ptr = (uint8_t*)&app->gen_info->phoenix_v2.serial;
        byte_count = sizeof(app->gen_info->phoenix_v2.serial);
        break;
    case GenData:
    case GenSecPlus1:
    default:
        furi_crash("Not implemented");
        break;
    }

    furi_assert(byte_ptr);
    furi_assert(byte_count > 0);

    *((uint32_t*)byte_ptr) = __bswap32(*((uint32_t*)byte_ptr)); // Convert

    ByteInput* byte_input = app->byte_input;
    byte_input_set_header_text(byte_input, "Enter SERIAL in hex");
    byte_input_set_result_callback(
        byte_input, sub_creator_scene_set_serial_byte_input_callback, NULL, app, byte_ptr, byte_count);
    view_dispatcher_switch_to_view(app->view_dispatcher, SubCreatorViewByteInput);
}

bool sub_creator_scene_set_serial_on_event(void* context, SceneManagerEvent event) {
    furi_assert(context);

    SubCreator* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == SubCreatorCustomEventByteInputDone) {
            switch(app->gen_info->type) {
            case GenFaacSLH:
                app->gen_info->faac_slh.serial = __bswap32(app->gen_info->faac_slh.serial);
                break;
            case GenKeeloq:
                app->gen_info->keeloq.serial = __bswap32(app->gen_info->keeloq.serial);
                break;
            case GenCameAtomo:
                app->gen_info->came_atomo.serial = __bswap32(app->gen_info->came_atomo.serial);
                break;
            case GenKeeloqSeed:
                app->gen_info->keeloq_seed.serial = __bswap32(app->gen_info->keeloq_seed.serial);
                break;
            case GenAlutechAt4n:
                app->gen_info->alutech_at_4n.serial = __bswap32(app->gen_info->alutech_at_4n.serial);
                break;
            case GenSomfyTelis:
                app->gen_info->somfy_telis.serial = __bswap32(app->gen_info->somfy_telis.serial);
                break;
            case GenSomfyKeytis:
                app->gen_info->somfy_keytis.serial = __bswap32(app->gen_info->somfy_keytis.serial);
                break;
            case GenKingGatesStylo4k:
                app->gen_info->kinggates_stylo_4k.serial = __bswap32(app->gen_info->kinggates_stylo_4k.serial);
                break;
            case GenJarolift:
                app->gen_info->jarolift.serial = __bswap32(app->gen_info->jarolift.serial);
                break;
            case GenDitecGOL4:
                app->gen_info->ditec_gol4.serial = __bswap32(app->gen_info->ditec_gol4.serial);
                break;
            case GenBenincaARC:
                app->gen_info->beninca_arc.serial = __bswap32(app->gen_info->beninca_arc.serial);
                break;
            case GenNiceFlorS:
                app->gen_info->nice_flor_s.serial = __bswap32(app->gen_info->nice_flor_s.serial);
                break;
            case GenSecPlus2:
                app->gen_info->sec_plus_2.serial = __bswap32(app->gen_info->sec_plus_2.serial);
                break;
            case GenPhoenixV2:
                app->gen_info->phoenix_v2.serial = __bswap32(app->gen_info->phoenix_v2.serial);
                break;
            case GenData:
            case GenSecPlus1:
            default:
                furi_crash("Not implemented");
                break;
            }

            switch(app->gen_info->type) {
            case GenFaacSLH:
            case GenKeeloq:
            case GenKeeloqSeed:
            case GenAlutechAt4n:
            case GenSomfyTelis:
            case GenSomfyKeytis:
            case GenKingGatesStylo4k:
            case GenBenincaARC:
            case GenJarolift:
            case GenDitecGOL4:
            case GenNiceFlorS:
            case GenSecPlus2:
                scene_manager_next_scene(app->scene_manager, SubCreatorSceneSetButton);
                break;
            case GenCameAtomo:
            case GenPhoenixV2:
                scene_manager_next_scene(app->scene_manager, SubCreatorSceneSetCounter);
                break;
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

void sub_creator_scene_set_serial_on_exit(void* context) {
    SubCreator* app = context;

    byte_input_set_result_callback(app->byte_input, NULL, NULL, NULL, NULL, 0);
    byte_input_set_header_text(app->byte_input, "");
}
