#pragma once

class CompressionToolkit {
public:
    CompressionToolkit() = delete;

    using Metadata = int;
    static constexpr size_t METADATA_SIZE = sizeof(Metadata);
    static constexpr int UNCOMPRESSED_DATA_INDICATOR = -1;
};
