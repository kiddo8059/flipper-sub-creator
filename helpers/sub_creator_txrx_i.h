#pragma once

#include "sub_creator_txrx.h"

struct SubCreatorTxRx {
    SubGhzEnvironment* environment;
    SubGhzReceiver* receiver;
    SubGhzTransmitter* transmitter;
    SubGhzProtocolDecoderBase* decoder_result;
    FlipperFormat* fff_data;

    SubGhzRadioPreset* preset;

    bool is_database_loaded;
};
