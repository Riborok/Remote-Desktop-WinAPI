#include "../../../inc/sock/udp/MaskedDataContext.hpp"

void MaskedDataContext::initializeFromPayload(const Payload& payload) {
    _isInitialized = true;
    _currentId = payload.id;
    _residualSize = payload.totalDataSize - payload.data.size();
    _maskedData.resize(payload.totalDataSize);
    _maskedData.insertPayload(payload);
}

void MaskedDataContext::insertPayload(const Payload& payload) {
    _residualSize -= payload.data.size();
    _maskedData.insertPayload(payload);
}

bool MaskedDataContext::isOldPayload(const Payload& payload) {
    if (_currentId > payload.id) {
        ++_oldPayloadCountInRow;
        return true;
    }
    _oldPayloadCountInRow = 0;
    return false;
}

bool MaskedDataContext::hasExceededOldPayloadCountInRow() const {
    return _oldPayloadCountInRow >= MAX_OLD_PAYLOADS_IN_ROW;
}

bool MaskedDataContext::isNewPayload(const Payload& payload) const {
    return _currentId < payload.id;
}

bool MaskedDataContext::isInitialized() const { return _isInitialized; }

bool MaskedDataContext::isAllDataReceived() const { return _residualSize == 0; }

bool MaskedDataContext::isEmpty() const { return _maskedData.getData().empty(); }

MaskedData&& MaskedDataContext::extractMaskedData() { return std::move(_maskedData); }
