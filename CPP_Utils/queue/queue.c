#include "queue.h"
#include <stdlib.h>

bool queue_init(queue_t* me, void* buffer, uint32_t bufferSize, uint32_t elementSize) {
    me->buffer = (uint8_t*) buffer;
    me->bufferSize = bufferSize;
    me->elementSize = elementSize;
    me->headByteIndex = 0;
    me->tailByteIndex = 0;

    return true;
}

bool queue_push(queue_t* me, void* pushItem, bool overwriteIfFull) {
    bool ok;

    if (me == NULL) {
        ok = false;
    } else {
        // copied here in order to avoid concurrency problems
        uint32_t headByteIndex = me->headByteIndex;
        uint32_t tailByteIndex = me->tailByteIndex;

        uint32_t nextTail = (tailByteIndex + me->elementSize);
        if (nextTail >= me->bufferSize) {
            nextTail = 0;
        }

        if (nextTail == headByteIndex) {
            if (!overwriteIfFull) {
                ok = false;
            } else {
                int8_t* tempPtr = (int8_t*) pushItem;
                uint32_t bufferIndex = headByteIndex;
                int32_t i;

                for (i = 0; i < me->elementSize; i++) {
                    me->buffer[bufferIndex++] = *tempPtr++;
                }

                if (bufferIndex >= me->bufferSize) {
                    bufferIndex = 0;
                }

                me->headByteIndex = bufferIndex;

                ok = true;
            }
        } else {
            uint8_t* tempPtr = (uint8_t*) pushItem;
            uint32_t bufferIndex = tailByteIndex;

            uint32_t i = 0;
            for (i = 0; i < me->elementSize; i++) {
                me->buffer[bufferIndex++] = tempPtr[i];
            }

            me->tailByteIndex = nextTail;
            ok = true;
        }
    }

    return ok;
}

bool queue_pop(queue_t* me, void* popItem) {
    bool ok;

    if (me == NULL) {
        ok = false;
    } else {
        // copied here in order to avoid concurrency problems
        uint32_t headByteIndex = me->headByteIndex;
        uint32_t tailByteIndex = me->tailByteIndex;

        if (headByteIndex == tailByteIndex) {
            ok = false;
        } else {
            uint32_t nextHead = headByteIndex + me->elementSize;
            uint8_t* tempPtr = (uint8_t*) popItem;
            uint32_t bufferIndex = headByteIndex;
            uint32_t i;

            if (nextHead >= me->bufferSize) {
                nextHead = 0;
            }

            for (i = 0; i < me->elementSize; i++) {
                tempPtr[i] = me->buffer[bufferIndex++];
            }

            me->headByteIndex = nextHead;
            ok = true;
        }
    }

    return ok;
}

bool queue_peek(queue_t* me, uint32_t index, void* peekItem) {
    bool ok;

    if (me == NULL) {
        ok = false;
    } else {
        uint32_t i;
        uint32_t headByteIndex = me->headByteIndex;
        uint32_t tailByteIndex = me->tailByteIndex;
        ok = true;

        for (i = 0; ok && (i < index); i++) {
            if (headByteIndex == tailByteIndex) {
                ok = false;
            } else {
                headByteIndex += me->elementSize;
                if (headByteIndex >= me->bufferSize) {
                    headByteIndex = 0;
                }
            }
        }

        if (headByteIndex == tailByteIndex) {
            ok = false;
        }

        if (ok) {
            uint8_t* tempPtr = (uint8_t*) peekItem;
            uint32_t bufferIndex = headByteIndex;
            uint32_t i;

            for (i = 0; i < me->elementSize; i++) {
                tempPtr[i] = me->buffer[bufferIndex++];
            }
        }
    }

    return ok;
}

bool queue_trash(queue_t* me) {
    if (me == NULL) {
        return false;
    } else {
        me->headByteIndex = 0;
        me->tailByteIndex = 0;

        return true;
    }
}

bool queue_isEmpty(queue_t* me) {
    if (me == NULL) {
        return false;
    } else {
        bool empty;

        uint32_t headByteIndex = me->headByteIndex;
        uint32_t tailByteIndex = me->tailByteIndex;

        if (headByteIndex == tailByteIndex) {
            empty = true;
        } else {
            empty = false;
        }

        return empty;
    }
}
