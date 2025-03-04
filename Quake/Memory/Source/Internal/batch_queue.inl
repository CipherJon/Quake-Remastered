#include "..\..\Include\Internal\batch_queue.hpp"
#include <stdexcept>

namespace internal
{
    // Assuming these are defined elsewhere; providing placeholders if not
    enum QBool { QFalse = 0, QTrue = 1 }; // Example definition
    
    // Define QUEUED and DEQUEUED as constants for clarity
    static constexpr bool QUEUED = true;
    static constexpr bool DEQUEUED = false;
    template<typename TAllocator, std::size_t maxBatch>
    BatchQueue<TAllocator, maxBatch>::BatchQueue(TAllocator* parent, QBool flushes)
        : _parent(parent), _flags(), _numQueued(0), _hasFlushed(flushes), _flushOnClear(flushes)
    {
    }

    template<typename TAllocator, size_t maxBatch>
    BatchQueue<TAllocator, maxBatch>::~BatchQueue()
    {
        clear();
    }

    template<typename TAllocator, size_t maxBatch>
    void BatchQueue<TAllocator, maxBatch>::flush()
    {
        for (size_t i = 0; i < maxBatch; i++)
        {
            if (_isQueued(i))
            {
                _parent->allocate(_queue[i]);
                _queue[i] = Block();
            }
        }
        _flags.reset(); // Changed to reset() for std::bitset consistency
        _numQueued = 0;
        _hasFlushed = QTrue;
    }

    template<typename TAllocator, size_t maxBatch>
    void BatchQueue<TAllocator, maxBatch>::queue(Block& block)
    {
        _checkFlushStatus();
        if (isFull())
        {
            flush();
        }
        for (size_t i = 0; i < maxBatch; i++)
        {
            if (_isDequeued(i))
            {
                _queue[i] = block;
                _flags[i] = QUEUED;
                _numQueued++;
                return;
            }
        }
    }

    template<typename TAllocator, size_t maxBatch>
    void BatchQueue<TAllocator, maxBatch>::queue(size_t index, Block& block)
    {
        _checkFlushStatus();
        if (isFull())
        {
            flush();
        }
        if (_isDequeued(index))
        {
            _queue[index] = block;
            _flags[index] = QUEUED;
            _numQueued++;
        }
    }

    template<typename TAllocator, size_t maxBatch>
    size_t BatchQueue<TAllocator, maxBatch>::dequeue(Block& block)
    {
        for (size_t i = 0; i < maxBatch; i++)
        {
            if (_isQueued(i))
            {
                if (addressMatch(block.address, _queue[i].address))
                {
                    _queue[i] = Block();
                    _flags[i] = DEQUEUED;
                    _numQueued--;
                    return i;
                }
            }
        }
        return BATCH_NULL_INDEX; // Assuming defined elsewhere, e.g., static_cast<size_t>(-1)
    }
    template<typename TAllocator, size_t maxBatch>
    Block BatchQueue<TAllocator, maxBatch>::dequeue(size_t index)
    {
        if (index >= maxBatch)
        {
            throw std::out_of_range("Index out of range");
        }
        if (_isQueued(index))
        {
            Block block = _queue[index]; // Return a copy to avoid dangling reference
            _queue[index] = Block();
            _flags[index] = DEQUEUED;
            _numQueued--;
            return block;
        }
        throw std::out_of_range("Block not queued at index");
    }

    template<typename TAllocator, size_t maxBatch>
    bool BatchQueue<TAllocator, maxBatch>::owns(Block block) const
    {
        for (size_t i = 0; i < maxBatch; i++)
        {
            if (_isQueued(i))
            {
                if (addressMatch(block.address, _queue[i].address))
                {
                    return true;
                }
            }
        }
        return false;
    }

    template<typename TAllocator, size_t maxBatch>
    void BatchQueue<TAllocator, maxBatch>::clear()
    {
        for (size_t i = 0; i < maxBatch; i++)
        {
            if (_isQueued(i))
            {
                delete[] _queue[i].address;
                _queue[i].address = nullptr;
            }
        }
        _flags.reset(); // Changed to reset() for std::bitset consistency
        _numQueued = 0;
    }

    template<typename TAllocator, size_t maxBatch>
    bool BatchQueue<TAllocator, maxBatch>::isFull() const
    {
        return _flags.all();
    }
    template<typename TAllocator, size_t maxBatch>
    bool BatchQueue<TAllocator, maxBatch>::isEmpty() const
    {
        return _numQueued == 0;
    }
    template<typename TAllocator, size_t maxBatch>
    void BatchQueue<TAllocator, maxBatch>::setFlushesOnClear(QBool flushes)
    {
        _flushOnClear = flushes;
    }

    template<typename TAllocator, size_t maxBatch>
    size_t BatchQueue<TAllocator, maxBatch>::getMaxBatchSize() const
    {
        return maxBatch;
    }

    template<typename TAllocator, size_t maxBatch>
    size_t BatchQueue<TAllocator, maxBatch>::getNumQueued() const
    {
        return _numQueued;
    }
    template<typename TAllocator, size_t maxBatch>
    bool BatchQueue<TAllocator, maxBatch>::hasFlushed() const
    {
        return _hasFlushed;
    }
    template<typename TAllocator, size_t maxBatch>
    QBool BatchQueue<TAllocator, maxBatch>::flushesOnClear() const
    {
        return _flushOnClear;
    }

    template<typename TAllocator, size_t maxBatch>
    std::ostream& operator<<(std::ostream& os, const BatchQueue<TAllocator, maxBatch>& queue)
    {
        std::string message = "BatchQueue: \n";
        message += "\tMax Batch Size: " + std::to_string(maxBatch) + "\n";
        message += "\tNumber Queued: " + std::to_string(queue._numQueued) + "\n";
        message += "\tFlushes on Clear: " + std::to_string(static_cast<int>(_flushOnClear)) + "\n";
        message += "\tHas Flushed: " + std::to_string(static_cast<int>(_hasFlushed)) + "\n";
        os << message;
        return os;
    }
    template<typename TAllocator, size_t maxBatch>
    bool BatchQueue<TAllocator, maxBatch>::_isQueued(size_t index) const
    {
        return _flags[index]; // Simplified assuming QUEUED is true
    }
    template<typename TAllocator, size_t maxBatch>
    bool BatchQueue<TAllocator, maxBatch>::_isDequeued(size_t index) const
    {
        return !_flags[index]; // Simplified assuming DEQUEUED is false
    }

    template<typename TAllocator, size_t maxBatch>
    void BatchQueue<TAllocator, maxBatch>::_checkFlushStatus() const
    {
        if (_hasFlushed)
        {
            _hasFlushed = QFalse;
        }
    }
}