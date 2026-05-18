#include "sub_creator_i.h"

#include <flipper_format/flipper_format.h>
#include <flipper_format/flipper_format_i.h>

void sub_creator_file_name_clear(SubCreator* app) {
    furi_assert(app);
    furi_string_set(app->file_path, SUBGHZ_APP_FOLDER);
    furi_string_reset(app->file_path_tmp);
}

bool sub_creator_path_is_file(FuriString* path) {
    return furi_string_end_with(path, SUBGHZ_APP_FILENAME_EXTENSION);
}

bool sub_creator_rename_file(SubCreator* app) {
    furi_assert(app);
    bool ret = true;

    Storage* storage = furi_record_open(RECORD_STORAGE);

    if(furi_string_cmp(app->file_path_tmp, app->file_path)) {
        FS_Error fs_result = storage_common_rename(
            storage, furi_string_get_cstr(app->file_path_tmp), furi_string_get_cstr(app->file_path));

        if(fs_result != FSE_OK) {
            dialog_message_show_storage_error(app->dialogs, "Cannot rename\n file/directory");
            ret = false;
        }
    }
    furi_record_close(RECORD_STORAGE);

    return ret;
}

bool sub_creator_save_protocol_to_file(SubCreator* app, FlipperFormat* flipper_format, const char* dev_file_name) {
    furi_assert(app);
    furi_assert(flipper_format);
    furi_assert(dev_file_name);

    Storage* storage = furi_record_open(RECORD_STORAGE);
    Stream* flipper_format_stream = flipper_format_get_raw_stream(flipper_format);

    bool saved = false;
    FuriString* file_dir = furi_string_alloc();

    path_extract_dirname(dev_file_name, file_dir);
    do {
        flipper_format_delete_key(flipper_format, "Repeat");

        if(!storage_simply_mkdir(storage, furi_string_get_cstr(file_dir))) {
            dialog_message_show_storage_error(app->dialogs, "Cannot create\nfolder");
            break;
        }

        if(!storage_simply_remove(storage, dev_file_name)) {
            break;
        }

        stream_seek(flipper_format_stream, 0, StreamOffsetFromStart);
        stream_save_to_file(flipper_format_stream, storage, dev_file_name, FSOM_CREATE_ALWAYS);

        if(storage_common_stat(storage, dev_file_name, NULL) != FSE_OK) {
            break;
        }

        saved = true;
    } while(0);
    furi_string_free(file_dir);
    furi_record_close(RECORD_STORAGE);
    return saved;
}
