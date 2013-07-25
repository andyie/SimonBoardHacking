#ifndef RING_BUFFER_H
#define RING_BUFFER_H

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

    *head = c;
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

    c = *tail;
    tail = (tail + 1) % size;

    return 0;
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
