#pragma once

#include "sub_creator_types.h"

#include <lib/subghz/receiver.h>
#include <lib/subghz/transmitter.h>

typedef struct SubCreatorTxRx SubCreatorTxRx;

/**
 * Allocate SubGhzTxRx
 *
 * @return SubGhzTxRx* pointer to SubGhzTxRx
 */
SubCreatorTxRx* sub_creator_txrx_alloc(void);

/**
 * Free SubGhzTxRx
 *
 * @param instance Pointer to a SubGhzTxRx
 */
void sub_creator_txrx_free(SubCreatorTxRx* instance);

/**
 * Set preset
 *
 * @param instance Pointer to a SubGhzTxRx
 * @param preset_name Name of preset
 * @param frequency Frequency in Hz
 * @param preset_data Data of preset
 * @param preset_data_size Size of preset data
 */
void sub_creator_txrx_set_preset(
    SubCreatorTxRx* instance,
    const char* preset_name,
    uint32_t frequency,
    uint8_t* preset_data,
    size_t preset_data_size);

/**
 * Get pointer to a load data key
 *
 * @param instance Pointer to a SubGhzTxRx
 * @return FlipperFormat*
 */
FlipperFormat* sub_creator_txrx_get_fff_data(SubCreatorTxRx* instance);
