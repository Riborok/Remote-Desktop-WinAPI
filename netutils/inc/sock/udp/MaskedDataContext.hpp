#pragma once

#include "MaskedData.hpp"

class MaskedDataContext {
    static constexpr size_t MAX_OLD_PAYLOADS_IN_ROW = 10;

    bool _isInitialized = false;
    size_t _oldPayloadCountInRow = 0;
    size_t _currentId = 0;
    size_t _residualSize = 0;
    MaskedData _maskedData;
public:
    void initializeFromPayload(const Payload& payload);
    void insertPayload(const Payload& payload);

    bool isOldPayload(const Payload& payload);
    bool hasExceededOldPayloadCountInRow() const;
    bool isNewPayload(const Payload& payload) const;

    bool isInitialized() const;
    bool isAllDataReceived() const;
    bool isEmpty() const;
    
    MaskedData&& extractMaskedData();
};
