#include "sub_creator_txrx_i.h"

#include <lib/subghz/subghz_protocol_registry.h>

SubCreatorTxRx* sub_creator_txrx_alloc(void) {
    SubCreatorTxRx* instance = malloc(sizeof(SubCreatorTxRx));

    instance->preset = malloc(sizeof(SubGhzRadioPreset));
    instance->preset->name = furi_string_alloc();

    instance->fff_data = flipper_format_string_alloc();

    instance->environment = subghz_environment_alloc();
    instance->is_database_loaded =
        subghz_environment_load_keystore(instance->environment, APP_ASSETS_PATH("encrypted"));
    subghz_environment_set_alutech_at_4n_rainbow_table_file_name(instance->environment, SUBGHZ_ALUTECH_AT_4N_DIR_NAME);
    subghz_environment_set_nice_flor_s_rainbow_table_file_name(instance->environment, SUBGHZ_NICE_FLOR_S_DIR_NAME);
    subghz_environment_set_protocol_registry(instance->environment, (void*)&subghz_protocol_registry);
    instance->receiver = subghz_receiver_alloc_init(instance->environment);

    return instance;
}

void sub_creator_txrx_free(SubCreatorTxRx* instance) {
    furi_assert(instance);

    subghz_receiver_free(instance->receiver);
    subghz_environment_free(instance->environment);
    flipper_format_free(instance->fff_data);
    furi_string_free(instance->preset->name);

    free(instance->preset);
    free(instance);
}

void sub_creator_txrx_set_preset(
    SubCreatorTxRx* instance,
    const char* preset_name,
    uint32_t frequency,
    uint8_t* preset_data,
    size_t preset_data_size) {
    furi_assert(instance);
    furi_string_set(instance->preset->name, preset_name);

    SubGhzRadioPreset* preset = instance->preset;
    preset->frequency = frequency;
    preset->data = preset_data;
    preset->data_size = preset_data_size;
}

FlipperFormat* sub_creator_txrx_get_fff_data(SubCreatorTxRx* instance) {
    furi_assert(instance);
    return instance->fff_data;
}
