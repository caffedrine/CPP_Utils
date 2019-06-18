#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t* buffer;			//!< buffer supporting the queue
    uint32_t bufferSize;		//!< size of the support buffer, in bytes
    uint32_t headByteIndex;		//!< head index
    uint32_t tailByteIndex;		//!< tail index
    uint32_t elementSize;		//!< size of an element, in bytes
} queue_t;


/**
 * @brief   initialize a queue
 *
 * @param   me                  pointer to the queue instance
 * @param   buffer              pointer to the support buffer
 * @param	bufferSize			size of the support buffer
 * @param   elementSize         size of an element
 * @return  TRUE upon success
 */
bool queue_init(queue_t* me, void* buffer, uint32_t bufferSize, uint32_t elementSize);


/**
 * @brief   push an element into a queue
 *
 * @param   me                  pointer to the queue instance
 * @param   pushElement         pointer to the element to push
 * @param	overwriteIfFull		if TRUE, overwrites last element in case the queue is full
 * @return  TRUE upon success (FALSE if the queue is full)
 *
 * @note
 * overwriteIfFull == TRUE breaks thread-safety: be sure to avoid concurrency between
 * producer and consumer.
 * This happens because overwrite causes push to change head index instead of tail index.
 */
bool queue_push(queue_t* me, void* pushElement, bool overwriteIfFull);


/**
 * @brief   pop an element from the queue
 *
 * @param   me                  pointer to the queue instance
 * @param   popElement          pointer to the element that will be popped
 * @return  TRUE upon success (FALSE if the queue is empty)
 */
bool queue_pop(queue_t* me, void* popElement);


/**
 * @brief   peek an element, without popping it
 *
 * @param   me                  pointer to the queue instance
 * @param   index               offset from the first element
 * @param   peekElement         pointer to the element that will be copied
 * @return  TRUE upon success (FALSE if the selected element doesn't exist)
 */
bool queue_peek(queue_t* me, uint32_t index, void* peekElement);


/**
 * @brief   discard all the elements in a queue
 *
 * @param   me                  pointer to the queue instance
 * @return  TRUE upon success
 */
bool queue_trash(queue_t* me);


/**
 * @brief	check if the queue is empty
 *
 * @param 	me					pointer to the queue instance
 * @return 	TRUE if the queue is empty
 */
bool queue_isEmpty(queue_t* me);


#endif /* QUEUE_H_ */

/**
 * @}
 */
