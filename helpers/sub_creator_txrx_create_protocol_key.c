#include "sub_creator_txrx_i.h"
#include "sub_creator_txrx_create_protocol_key.h"

#include <lib/subghz/transmitter.h>
#include <lib/subghz/protocols/public_api.h>

#include <flipper_format/flipper_format_i.h>
#include <lib/toolbox/stream/stream.h>
#include <lib/subghz/protocols/raw.h>

bool sub_creator_txrx_gen_data_protocol(
    void* context,
    const char* preset_name,
    uint32_t frequency,
    const char* protocol_name,
    uint64_t key,
    uint32_t bit) {
    furi_assert(context);
    SubCreatorTxRx* instance = context;

    bool res = false;

    sub_creator_txrx_set_preset(instance, preset_name, frequency, NULL, 0);
    instance->decoder_result = subghz_receiver_search_decoder_base_by_name(instance->receiver, protocol_name);

    if(instance->decoder_result == NULL) {
        return false;
    }

    do {
        Stream* fff_data_stream = flipper_format_get_raw_stream(instance->fff_data);
        stream_clean(fff_data_stream);
        if(subghz_protocol_decoder_base_serialize(instance->decoder_result, instance->fff_data, instance->preset) !=
           SubGhzProtocolStatusOk) {
            break;
        }
        if(!flipper_format_update_uint32(instance->fff_data, "Bit", &bit, 1)) {
            break;
        }

        uint8_t key_data[sizeof(uint64_t)] = {0};
        for(size_t i = 0; i < sizeof(uint64_t); i++) {
            key_data[sizeof(uint64_t) - i - 1] = (key >> (i * 8)) & 0xFF;
        }
        if(!flipper_format_update_hex(instance->fff_data, "Key", key_data, sizeof(uint64_t))) {
            break;
        }
        res = true;
    } while(false);
    return res;
}

bool sub_creator_txrx_gen_data_protocol_and_te(
    SubCreatorTxRx* instance,
    const char* preset_name,
    uint32_t frequency,
    const char* protocol_name,
    uint64_t key,
    uint32_t bit,
    uint32_t te) {
    furi_assert(instance);
    bool ret = false;
    if(sub_creator_txrx_gen_data_protocol(instance, preset_name, frequency, protocol_name, key, bit)) {
        if(!flipper_format_update_uint32(instance->fff_data, "TE", (uint32_t*)&te, 1)) {
        } else {
            ret = true;
        }
    }
    if(ret) {
        uint32_t guard_time = 30;
        if(!flipper_format_update_uint32(instance->fff_data, "Guard_time", (uint32_t*)&guard_time, 1)) {
            ret = false;
        }
    }
    return ret;
}

bool sub_creator_txrx_gen_keeloq_protocol(
    SubCreatorTxRx* instance,
    const char* preset_name,
    uint32_t frequency,
    uint32_t serial,
    uint8_t btn,
    uint16_t cnt,
    const char* manufacture_name) {
    furi_assert(instance);

    bool res = false;

    instance->transmitter = subghz_transmitter_alloc_init(instance->environment, "KeeLoq");
    sub_creator_txrx_set_preset(instance, preset_name, frequency, NULL, 0);

    if(instance->transmitter && subghz_protocol_keeloq_create_data(
                                    subghz_transmitter_get_protocol_instance(instance->transmitter),
                                    instance->fff_data,
                                    serial,
                                    btn,
                                    cnt,
                                    manufacture_name,
                                    instance->preset)) {
        flipper_format_write_string_cstr(instance->fff_data, "Manufacture", manufacture_name);
        res = true;
    }
    subghz_transmitter_free(instance->transmitter);
    return res;
}

bool sub_creator_txrx_gen_keeloq_seed_protocol(
    void* context,
    const char* preset_name,
    uint32_t frequency,
    uint32_t serial,
    uint8_t btn,
    uint16_t cnt,
    uint32_t seed,
    const char* manufacture_name) {
    SubCreatorTxRx* txrx = context;

    bool res = false;

    txrx->transmitter = subghz_transmitter_alloc_init(txrx->environment, "KeeLoq");
    sub_creator_txrx_set_preset(txrx, preset_name, frequency, NULL, 0);

#if defined(FW_ORIGIN_Unleashed)
    if(txrx->transmitter && subghz_protocol_keeloq_seed_create_data(
                                subghz_transmitter_get_protocol_instance(txrx->transmitter),
                                txrx->fff_data,
                                serial,
                                btn,
                                cnt,
                                seed,
                                manufacture_name,
                                txrx->preset)) {
        res = true;
    }
#elif defined(FW_ORIGIN_Momentum)
    if(txrx->transmitter && subghz_protocol_keeloq_bft_create_data(
                                subghz_transmitter_get_protocol_instance(txrx->transmitter),
                                txrx->fff_data,
                                serial,
                                btn,
                                cnt,
                                seed,
                                manufacture_name,
                                txrx->preset)) {
        res = true;
    }
#endif

    if(res) {
        uint8_t seed_data[sizeof(uint32_t)] = {0};
        for(size_t i = 0; i < sizeof(uint32_t); i++) {
            seed_data[sizeof(uint32_t) - i - 1] = (seed >> i * 8) & 0xFF;
        }

        flipper_format_write_hex(txrx->fff_data, "Seed", seed_data, sizeof(uint32_t));

        flipper_format_write_string_cstr(txrx->fff_data, "Manufacture", manufacture_name);
    }

    subghz_transmitter_free(txrx->transmitter);

    return res;
}

bool sub_creator_txrx_gen_nice_flor_s_protocol(
    void* context,
    const char* preset_name,
    uint32_t frequency,
    uint32_t serial,
    uint8_t btn,
    uint16_t cnt,
    bool nice_one) {
    SubCreatorTxRx* txrx = context;

    bool res = false;

    txrx->transmitter = subghz_transmitter_alloc_init(txrx->environment, "Nice FloR-S");
    sub_creator_txrx_set_preset(txrx, preset_name, frequency, NULL, 0);

    if(txrx->transmitter && subghz_protocol_nice_flor_s_create_data(
                                subghz_transmitter_get_protocol_instance(txrx->transmitter),
                                txrx->fff_data,
                                serial,
                                btn,
                                cnt,
                                txrx->preset,
                                nice_one)) {
        res = true;
    }

    subghz_transmitter_free(txrx->transmitter);

    return res;
}

bool sub_creator_txrx_gen_faac_slh_protocol(
    void* context,
    const char* preset_name,
    uint32_t frequency,
    uint32_t serial,
    uint8_t btn,
    uint32_t cnt,
    uint32_t seed,
    const char* manufacture_name) {
    SubCreatorTxRx* txrx = context;

    bool res = false;

    txrx->transmitter = subghz_transmitter_alloc_init(txrx->environment, "Faac SLH");
    sub_creator_txrx_set_preset(txrx, preset_name, frequency, NULL, 0);

    if(txrx->transmitter && subghz_protocol_faac_slh_create_data(
                                subghz_transmitter_get_protocol_instance(txrx->transmitter),
                                txrx->fff_data,
                                serial,
                                btn,
                                cnt,
                                seed,
                                manufacture_name,
                                txrx->preset)) {
        res = true;
    }

    if(res) {
        uint8_t seed_data[sizeof(uint32_t)] = {0};
        for(size_t i = 0; i < sizeof(uint32_t); i++) {
            seed_data[sizeof(uint32_t) - i - 1] = (seed >> i * 8) & 0xFF;
        }

        bool tmp_allow_zero_seed = true;
        flipper_format_write_hex(txrx->fff_data, "Seed", seed_data, sizeof(uint32_t));
        flipper_format_write_bool(txrx->fff_data, "AllowZeroSeed", &tmp_allow_zero_seed, 1);
    }

    subghz_transmitter_free(txrx->transmitter);

    return res;
}

bool sub_creator_txrx_gen_alutech_at_4n_protocol(
    void* context,
    const char* preset_name,
    uint32_t frequency,
    uint32_t serial,
    uint8_t btn,
    uint16_t cnt) {
    SubCreatorTxRx* txrx = context;

    bool res = false;

    txrx->transmitter = subghz_transmitter_alloc_init(txrx->environment, "Alutech AT-4N");
    sub_creator_txrx_set_preset(txrx, preset_name, frequency, NULL, 0);

    if(txrx->transmitter &&
       subghz_protocol_alutech_at_4n_create_data(
           subghz_transmitter_get_protocol_instance(txrx->transmitter), txrx->fff_data, serial, btn, cnt, txrx->preset)) {
        res = true;
    }

    subghz_transmitter_free(txrx->transmitter);

    return res;
}

bool sub_creator_txrx_gen_came_atomo_protocol(
    void* context,
    const char* preset_name,
    uint32_t frequency,
    uint32_t serial,
    uint16_t cnt) {
    SubCreatorTxRx* txrx = context;

    bool res = false;

    txrx->transmitter = subghz_transmitter_alloc_init(txrx->environment, "CAME Atomo");
    sub_creator_txrx_set_preset(txrx, preset_name, frequency, NULL, 0);

    if(txrx->transmitter &&
       subghz_protocol_came_atomo_create_data(
           subghz_transmitter_get_protocol_instance(txrx->transmitter), txrx->fff_data, serial, cnt, txrx->preset)) {
        res = true;
    }

    subghz_transmitter_free(txrx->transmitter);

    return res;
}

bool sub_creator_txrx_gen_somfy_telis_protocol(
    void* context,
    const char* preset_name,
    uint32_t frequency,
    uint32_t serial,
    uint8_t btn,
    uint16_t cnt) {
    SubCreatorTxRx* txrx = context;

    bool res = false;

    txrx->transmitter = subghz_transmitter_alloc_init(txrx->environment, "Somfy Telis");
    sub_creator_txrx_set_preset(txrx, preset_name, frequency, NULL, 0);

    if(txrx->transmitter &&
       subghz_protocol_somfy_telis_create_data(
           subghz_transmitter_get_protocol_instance(txrx->transmitter), txrx->fff_data, serial, btn, cnt, txrx->preset)) {
        res = true;
    }

    subghz_transmitter_free(txrx->transmitter);

    return res;
}

bool sub_creator_txrx_gen_somfy_keytis_protocol(
    void* context,
    const char* preset_name,
    uint32_t frequency,
    uint32_t serial,
    uint8_t btn,
    uint16_t cnt) {
    SubCreatorTxRx* txrx = context;

    bool res = false;

    txrx->transmitter = subghz_transmitter_alloc_init(txrx->environment, "Somfy Keytis");
    sub_creator_txrx_set_preset(txrx, preset_name, frequency, NULL, 0);

    if(txrx->transmitter &&
       subghz_protocol_somfy_keytis_create_data(
           subghz_transmitter_get_protocol_instance(txrx->transmitter), txrx->fff_data, serial, btn, cnt, txrx->preset)) {
        res = true;
    }

    subghz_transmitter_free(txrx->transmitter);

    return res;
}

bool sub_creator_txrx_gen_kinggates_stylo_4k_protocol(
    void* context,
    const char* preset_name,
    uint32_t frequency,
    uint32_t serial,
    uint8_t btn,
    uint16_t cnt) {
    SubCreatorTxRx* txrx = context;

    bool res = false;

    txrx->transmitter = subghz_transmitter_alloc_init(txrx->environment, "KingGates Stylo4k");
    sub_creator_txrx_set_preset(txrx, preset_name, frequency, NULL, 0);

    if(txrx->transmitter &&
       subghz_protocol_kinggates_stylo_4k_create_data(
           subghz_transmitter_get_protocol_instance(txrx->transmitter), txrx->fff_data, serial, btn, cnt, txrx->preset)) {
        res = true;
    }

    subghz_transmitter_free(txrx->transmitter);

    return res;
}

bool sub_creator_txrx_gen_beninca_arc_protocol(
    void* context,
    const char* preset_name,
    uint32_t frequency,
    uint32_t serial,
    uint8_t btn,
    uint32_t cnt) {
    SubCreatorTxRx* txrx = context;

    bool res = false;

    txrx->transmitter = subghz_transmitter_alloc_init(txrx->environment, "Beninca ARC");
    sub_creator_txrx_set_preset(txrx, preset_name, frequency, NULL, 0);

    if(txrx->transmitter &&
       subghz_protocol_beninca_arc_create_data(
           subghz_transmitter_get_protocol_instance(txrx->transmitter), txrx->fff_data, serial, btn, cnt, txrx->preset)) {
        res = true;
    }

    subghz_transmitter_free(txrx->transmitter);

    return res;
}

bool sub_creator_txrx_gen_jarolift_protocol(
    void* context,
    const char* preset_name,
    uint32_t frequency,
    uint32_t serial,
    uint8_t btn,
    uint16_t cnt) {
    SubCreatorTxRx* txrx = context;

    bool res = false;

    txrx->transmitter = subghz_transmitter_alloc_init(txrx->environment, "Jarolift");
    sub_creator_txrx_set_preset(txrx, preset_name, frequency, NULL, 0);

    if(txrx->transmitter &&
       subghz_protocol_jarolift_create_data(
           subghz_transmitter_get_protocol_instance(txrx->transmitter), txrx->fff_data, serial, btn, cnt, txrx->preset)) {
        res = true;
    }

    subghz_transmitter_free(txrx->transmitter);

    return res;
}

bool sub_creator_txrx_gen_ditec_gol4_protocol(
    void* context,
    const char* preset_name,
    uint32_t frequency,
    uint32_t serial,
    uint8_t btn,
    uint16_t cnt) {
    SubCreatorTxRx* txrx = context;

    bool res = false;

    txrx->transmitter = subghz_transmitter_alloc_init(txrx->environment, "Ditec GOL4");
    sub_creator_txrx_set_preset(txrx, preset_name, frequency, NULL, 0);

    if(txrx->transmitter &&
       subghz_protocol_ditec_gol4_create_data(
           subghz_transmitter_get_protocol_instance(txrx->transmitter), txrx->fff_data, serial, btn, cnt, txrx->preset)) {
        res = true;
    }

    subghz_transmitter_free(txrx->transmitter);

    return res;
}

bool sub_creator_txrx_gen_secplus_v2_protocol(
    SubCreatorTxRx* instance,
    const char* name_preset,
    uint32_t frequency,
    uint32_t serial,
    uint8_t btn,
    uint32_t cnt) {
    furi_assert(instance);

    bool ret = false;
    instance->transmitter = subghz_transmitter_alloc_init(instance->environment, "Security+ 2.0");
    sub_creator_txrx_set_preset(instance, name_preset, frequency, NULL, 0);
    if(instance->transmitter) {
        subghz_protocol_secplus_v2_create_data(
            subghz_transmitter_get_protocol_instance(instance->transmitter),
            instance->fff_data,
            serial,
            btn,
            cnt,
            instance->preset);
        ret = true;
    }
    return ret;
}

bool sub_creator_txrx_gen_secplus_v1_protocol(SubCreatorTxRx* instance, const char* name_preset, uint32_t frequency) {
    furi_assert(instance);

    bool ret = false;
    uint32_t serial = (uint32_t)rand();
    while(!subghz_protocol_secplus_v1_check_fixed(serial)) {
        serial = (uint32_t)rand();
    }
    if(sub_creator_txrx_gen_data_protocol(
           instance, name_preset, frequency, "Security+ 1.0", (uint64_t)serial << 32 | 0xE6000000, 42)) {
        ret = true;
    }
    return ret;
}

bool sub_creator_txrx_gen_phoenix_v2_protocol(
    void* context,
    const char* preset_name,
    uint32_t frequency,
    uint32_t serial,
    uint16_t cnt) {
    SubCreatorTxRx* txrx = context;

    bool res = false;

    txrx->transmitter = subghz_transmitter_alloc_init(txrx->environment, "Phoenix_V2");
    sub_creator_txrx_set_preset(txrx, preset_name, frequency, NULL, 0);

    if(txrx->transmitter &&
       subghz_protocol_phoenix_v2_create_data(
           subghz_transmitter_get_protocol_instance(txrx->transmitter), txrx->fff_data, serial, cnt, txrx->preset)) {
        res = true;
    }

    subghz_transmitter_free(txrx->transmitter);

    return res;
}

void sub_creator_txrx_gen_serial_gangqi(uint64_t* result_key) {
    uint64_t randkey = (uint64_t)rand();
    uint16_t serial = (uint16_t)((randkey) & 0xFFFF);
    uint8_t const_and_button = (uint8_t)(0xD0 | 0xD);
    uint8_t serial_high = (uint8_t)(serial >> 8);
    uint8_t serial_low = (uint8_t)(serial & 0xFF);
    uint8_t bytesum = (uint8_t)(0xC8 - serial_high - serial_low - const_and_button);

    *result_key = ((uint64_t)serial << 18) | ((uint64_t)const_and_button << 10) | ((uint64_t)bytesum << 2);
}

static uint8_t marantec_crc8(uint8_t* data, size_t len) {
    uint8_t crc = 0x01;
    size_t i, j;
    for(i = 0; i < len; i++) {
        crc ^= data[i];
        for(j = 0; j < 8; j++) {
            if((crc & 0x80) != 0)
                crc = (uint8_t)((crc << 1) ^ 0x1D);
            else
                crc <<= 1;
        }
    }
    return crc;
}

void sub_creator_txrx_gen_key_marantec(uint64_t* result_key) {
    uint64_t randkey = (uint64_t)rand();
    uint32_t serial = (uint32_t)((randkey) & 0xFFFFF);
    uint64_t full_key_no_crc =
        (uint64_t)((uint64_t)0x130 << 40 | (uint64_t)serial << 20 | (uint64_t)0x4 << 16 | (uint64_t)0x86 << 8);

    uint8_t tdata[6] = {
        full_key_no_crc >> 48,
        full_key_no_crc >> 40,
        full_key_no_crc >> 32,
        full_key_no_crc >> 24,
        full_key_no_crc >> 16,
        full_key_no_crc >> 8};

    uint8_t crc = marantec_crc8(tdata, sizeof(tdata));

    *result_key = ((full_key_no_crc >> 8) << 8) | crc;
}
