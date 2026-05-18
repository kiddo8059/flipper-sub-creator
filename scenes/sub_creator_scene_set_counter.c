#include "../sub_creator_i.h"
#include "../helpers/sub_creator_txrx_create_protocol_key.h"

#include <machine/endian.h>

static void sub_creator_scene_set_counter_byte_input_callback(void* context) {
    furi_assert(context);

    SubCreator* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, SubCreatorCustomEventByteInputDone);
}

void sub_creator_scene_set_counter_on_enter(void* context) {
    furi_assert(context);

    SubCreator* app = context;

    uint8_t* byte_ptr = NULL;
    uint8_t byte_count = 0;

    switch(app->gen_info->type) {
    case GenFaacSLH:
        byte_ptr = (uint8_t*)&app->gen_info->faac_slh.cnt;
        byte_count = sizeof(app->gen_info->faac_slh.cnt);
        break;
    case GenKeeloq:
        byte_ptr = (uint8_t*)&app->gen_info->keeloq.cnt;
        byte_count = sizeof(app->gen_info->keeloq.cnt);
        break;
    case GenCameAtomo:
        byte_ptr = (uint8_t*)&app->gen_info->came_atomo.cnt;
        byte_count = sizeof(app->gen_info->came_atomo.cnt);
        break;
    case GenKeeloqSeed:
        byte_ptr = (uint8_t*)&app->gen_info->keeloq_seed.cnt;
        byte_count = sizeof(app->gen_info->keeloq_seed.cnt);
        break;
    case GenAlutechAt4n:
        byte_ptr = (uint8_t*)&app->gen_info->alutech_at_4n.cnt;
        byte_count = sizeof(app->gen_info->alutech_at_4n.cnt);
        break;
    case GenSomfyTelis:
        byte_ptr = (uint8_t*)&app->gen_info->somfy_telis.cnt;
        byte_count = sizeof(app->gen_info->somfy_telis.cnt);
        break;
    case GenSomfyKeytis:
        byte_ptr = (uint8_t*)&app->gen_info->somfy_keytis.cnt;
        byte_count = sizeof(app->gen_info->somfy_keytis.cnt);
        break;
    case GenKingGatesStylo4k:
        byte_ptr = (uint8_t*)&app->gen_info->kinggates_stylo_4k.cnt;
        byte_count = sizeof(app->gen_info->kinggates_stylo_4k.cnt);
        break;
    case GenBenincaARC:
        byte_ptr = (uint8_t*)&app->gen_info->beninca_arc.cnt;
        byte_count = sizeof(app->gen_info->beninca_arc.cnt);
        break;
    case GenJarolift:
        byte_ptr = (uint8_t*)&app->gen_info->jarolift.cnt;
        byte_count = sizeof(app->gen_info->jarolift.cnt);
        break;
    case GenDitecGOL4:
        byte_ptr = (uint8_t*)&app->gen_info->ditec_gol4.cnt;
        byte_count = sizeof(app->gen_info->ditec_gol4.cnt);
        break;
    case GenNiceFlorS:
        byte_ptr = (uint8_t*)&app->gen_info->nice_flor_s.cnt;
        byte_count = sizeof(app->gen_info->nice_flor_s.cnt);
        break;
    case GenSecPlus2:
        byte_ptr = (uint8_t*)&app->gen_info->sec_plus_2.cnt;
        byte_count = sizeof(app->gen_info->sec_plus_2.cnt);
        break;
    case GenPhoenixV2:
        byte_ptr = (uint8_t*)&app->gen_info->phoenix_v2.cnt;
        byte_count = sizeof(app->gen_info->phoenix_v2.cnt);
        break;
    case GenData:
    case GenSecPlus1:
    default:
        furi_crash("Not implemented");
        break;
    }

    furi_assert(byte_ptr);
    furi_assert(byte_count > 0);

    if(byte_count == 2) {
        *((uint16_t*)byte_ptr) = __bswap16(*((uint16_t*)byte_ptr));
    } else if(byte_count == 4) {
        *((uint32_t*)byte_ptr) = __bswap32(*((uint32_t*)byte_ptr));
    }

    ByteInput* byte_input = app->byte_input;
    byte_input_set_header_text(byte_input, "Enter COUNTER in hex");

    byte_input_set_result_callback(
        byte_input, sub_creator_scene_set_counter_byte_input_callback, NULL, app, byte_ptr, byte_count);
    view_dispatcher_switch_to_view(app->view_dispatcher, SubCreatorViewByteInput);
}

bool sub_creator_scene_set_counter_on_event(void* context, SceneManagerEvent event) {
    furi_assert(context);

    SubCreator* app = context;

    bool consumed = false;
    bool generated_protocol = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == SubCreatorCustomEventByteInputDone) {
            switch(app->gen_info->type) {
            case GenFaacSLH:
                app->gen_info->faac_slh.cnt = __bswap32(app->gen_info->faac_slh.cnt);
                break;
            case GenKeeloq:
                app->gen_info->keeloq.cnt = __bswap16(app->gen_info->keeloq.cnt);
                break;
            case GenCameAtomo:
                app->gen_info->came_atomo.cnt = __bswap16(app->gen_info->came_atomo.cnt);
                break;
            case GenKeeloqSeed:
                app->gen_info->keeloq_seed.cnt = __bswap16(app->gen_info->keeloq_seed.cnt);
                break;
            case GenAlutechAt4n:
                app->gen_info->alutech_at_4n.cnt = __bswap16(app->gen_info->alutech_at_4n.cnt);
                break;
            case GenSomfyTelis:
                app->gen_info->somfy_telis.cnt = __bswap16(app->gen_info->somfy_telis.cnt);
                break;
            case GenSomfyKeytis:
                app->gen_info->somfy_keytis.cnt = __bswap16(app->gen_info->somfy_keytis.cnt);
                break;
            case GenKingGatesStylo4k:
                app->gen_info->kinggates_stylo_4k.cnt = __bswap16(app->gen_info->kinggates_stylo_4k.cnt);
                break;
            case GenBenincaARC:
                app->gen_info->beninca_arc.cnt = __bswap32(app->gen_info->beninca_arc.cnt);
                break;
            case GenJarolift:
                app->gen_info->jarolift.cnt = __bswap16(app->gen_info->jarolift.cnt);
                break;
            case GenDitecGOL4:
                app->gen_info->ditec_gol4.cnt = __bswap16(app->gen_info->ditec_gol4.cnt);
                break;
            case GenNiceFlorS:
                app->gen_info->nice_flor_s.cnt = __bswap16(app->gen_info->nice_flor_s.cnt);
                break;
            case GenSecPlus2:
                app->gen_info->sec_plus_2.cnt = __bswap32(app->gen_info->sec_plus_2.cnt);
                break;
            case GenPhoenixV2:
                app->gen_info->phoenix_v2.cnt = __bswap16(app->gen_info->phoenix_v2.cnt);
                break;
            case GenData:
            case GenSecPlus1:
            default:
                furi_crash("Not implemented");
                break;
            }

            switch(app->gen_info->type) {
            case GenFaacSLH:
            case GenKeeloqSeed:
                scene_manager_next_scene(app->scene_manager, SubCreatorSceneSetSeed);
                return true;
            case GenKeeloq:
                generated_protocol = sub_creator_txrx_gen_keeloq_protocol(
                    app->txrx,
                    app->gen_info->mod,
                    app->gen_info->freq,
                    app->gen_info->keeloq.serial,
                    app->gen_info->keeloq.btn,
                    app->gen_info->keeloq.cnt,
                    app->gen_info->keeloq.manuf);
                break;
            case GenCameAtomo:
                generated_protocol = sub_creator_txrx_gen_came_atomo_protocol(
                    app->txrx,
                    app->gen_info->mod,
                    app->gen_info->freq,
                    app->gen_info->came_atomo.serial,
                    app->gen_info->came_atomo.cnt);
                break;
            case GenAlutechAt4n:
                generated_protocol = sub_creator_txrx_gen_alutech_at_4n_protocol(
                    app->txrx,
                    app->gen_info->mod,
                    app->gen_info->freq,
                    app->gen_info->alutech_at_4n.serial,
                    app->gen_info->alutech_at_4n.btn,
                    app->gen_info->alutech_at_4n.cnt);
                break;
            case GenSomfyTelis:
                generated_protocol = sub_creator_txrx_gen_somfy_telis_protocol(
                    app->txrx,
                    app->gen_info->mod,
                    app->gen_info->freq,
                    app->gen_info->somfy_telis.serial,
                    app->gen_info->somfy_telis.btn,
                    app->gen_info->somfy_telis.cnt);
                break;
            case GenSomfyKeytis:
                generated_protocol = sub_creator_txrx_gen_somfy_keytis_protocol(
                    app->txrx,
                    app->gen_info->mod,
                    app->gen_info->freq,
                    app->gen_info->somfy_keytis.serial,
                    app->gen_info->somfy_keytis.btn,
                    app->gen_info->somfy_keytis.cnt);
                break;
            case GenKingGatesStylo4k:
                generated_protocol = sub_creator_txrx_gen_kinggates_stylo_4k_protocol(
                    app->txrx,
                    app->gen_info->mod,
                    app->gen_info->freq,
                    app->gen_info->kinggates_stylo_4k.serial,
                    app->gen_info->kinggates_stylo_4k.btn,
                    app->gen_info->kinggates_stylo_4k.cnt);
                break;
            case GenBenincaARC:
                generated_protocol = sub_creator_txrx_gen_beninca_arc_protocol(
                    app->txrx,
                    app->gen_info->mod,
                    app->gen_info->freq,
                    app->gen_info->beninca_arc.serial,
                    app->gen_info->beninca_arc.btn,
                    app->gen_info->beninca_arc.cnt);
                break;
            case GenJarolift:
                generated_protocol = sub_creator_txrx_gen_jarolift_protocol(
                    app->txrx,
                    app->gen_info->mod,
                    app->gen_info->freq,
                    app->gen_info->jarolift.serial,
                    app->gen_info->jarolift.btn,
                    app->gen_info->jarolift.cnt);
                break;
            case GenDitecGOL4:
                generated_protocol = sub_creator_txrx_gen_ditec_gol4_protocol(
                    app->txrx,
                    app->gen_info->mod,
                    app->gen_info->freq,
                    app->gen_info->ditec_gol4.serial,
                    app->gen_info->ditec_gol4.btn,
                    app->gen_info->ditec_gol4.cnt);
                break;
            case GenNiceFlorS:
                generated_protocol = sub_creator_txrx_gen_nice_flor_s_protocol(
                    app->txrx,
                    app->gen_info->mod,
                    app->gen_info->freq,
                    app->gen_info->nice_flor_s.serial,
                    app->gen_info->nice_flor_s.btn,
                    app->gen_info->nice_flor_s.cnt,
                    app->gen_info->nice_flor_s.nice_one);
                break;
            case GenSecPlus2:
                generated_protocol = sub_creator_txrx_gen_secplus_v2_protocol(
                    app->txrx,
                    app->gen_info->mod,
                    app->gen_info->freq,
                    app->gen_info->sec_plus_2.serial,
                    app->gen_info->sec_plus_2.btn,
                    app->gen_info->sec_plus_2.cnt);
                break;
            case GenPhoenixV2:
                generated_protocol = sub_creator_txrx_gen_phoenix_v2_protocol(
                    app->txrx,
                    app->gen_info->mod,
                    app->gen_info->freq,
                    app->gen_info->phoenix_v2.serial,
                    app->gen_info->phoenix_v2.cnt);
                break;
            case GenData:
            case GenSecPlus1:
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

void sub_creator_scene_set_counter_on_exit(void* context) {
    furi_assert(context);

    SubCreator* app = context;

    byte_input_set_result_callback(app->byte_input, NULL, NULL, NULL, NULL, 0);
    byte_input_set_header_text(app->byte_input, "");
}
