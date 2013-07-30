#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stddef.h>

/**
 * A super-lightweight ring buffer class. Only byte-wise operations are
 * supported. Using powers of two for the buffer size is strongly recommended.
 */
template <size_t size>
class RingBuffer
{
public:
  RingBuffer() : head(0), tail(0) {
  }

  /**
   * Put a character into the ring buffer.
   *
   * @param c The character to put.
   *
   * @return 0 on success, -1 on failure.
   */
  int put(char c) {
    // Don't push if it will make the buffer appear as empty.
    if ((head + 1) % size == tail)
    {
      return -1;
    }

    buf[head] = c;
    head = (head + 1) % size;

    return 0;
  }

  /**
   * Get a character out of the ring buffer.
   *
   * @param[out] c Set to the value of the extracted character.
   *
   * @return 0 on success, -1 on failure.
   */
  int get(char &c) {
    // Don't pop if the buffer is empty.
    if (tail == head)
    {
      return -1;
    }

    c = buf[tail];
    tail = (tail + 1) % size;

    return 0;
  }

  /**
   * Get the number of chars currently stored in the ring buffer.
   *
   * @return The number of chars stored in the ring buffer.
   */
  size_t num_elements()
  {
    if (head > tail)
    {
      //All characters are stored between head and tail
      return head - tail;
    }
    else
    {
      //All characters are stored outside of head and tail
      return size - (tail - head);
    }
  }

  /**
   * Get the number of free bytes currently available in the buffer. Note that
   * the maximum number of elements in the buffer is one less than the buffer's
   * size (hence the (- 1)).
   *
   * @return The number of bytes of free space in the buffer.
   */
  size_t num_free() {
    return size - num_elements() - 1;
  }

private:
  /**
   * Index of where to write the next character.
   */
  size_t head;

  /**
   * Index of where to read the next character.
   */
  size_t tail;

  /**
   * The buffer owned by this ring buffer instance.
   */
  char buf[size];
};

#endif
