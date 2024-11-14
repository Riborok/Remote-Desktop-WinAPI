#pragma once

struct Metadata {
    using FieldType = size_t;
    
    FieldType fragmentId;
    FieldType fragmentNumber;
    FieldType totalDataSize;
};
