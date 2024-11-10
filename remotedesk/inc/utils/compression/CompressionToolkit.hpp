#pragma once

class CompressionToolkit {
public:
    CompressionToolkit() = delete;

    static constexpr size_t METADATA_SIZE = sizeof(int);
    static constexpr int UNCOMPRESSED_DATA_INDICATOR = -1;
};
