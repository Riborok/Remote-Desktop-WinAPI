#pragma once

class CompressionToolkit {
public:
    CompressionToolkit() = delete;

    using OriginalSize = int;
    static constexpr size_t METADATA_SIZE = sizeof(OriginalSize);
    static constexpr int UNCOMPRESSED_DATA_INDICATOR = -1;
};
