/*
 * Copyright 2014-2018 Real Logic Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef INCLUDED_AERON_CONCURRENT_LOGBUFFER_HEADER__
#define INCLUDED_AERON_CONCURRENT_LOGBUFFER_HEADER__

#include <util/Index.h>
#include <util/BitUtil.h>
#include <concurrent/AtomicBuffer.h>
#include "DataFrameHeader.h"

namespace aeron { namespace concurrent { namespace logbuffer {

/**
 * Represents the header of the data frame for accessing meta data fields.
 */
class Header
{
public:
    Header(std::int32_t initialTermId, util::index_t capacity) :
        m_offset(0), m_initialTermId(initialTermId)
    {
        m_positionBitsToShift = util::BitUtil::numberOfTrailingZeroes(capacity);
    }

    Header(const Header& header) = default;

    /**
     * Get the initial term id this stream started at.
     *
     * @return the initial term id this stream started at.
     */
    inline std::int32_t initialTermId() const
    {
        return m_initialTermId;
    }

    inline void initialTermId(std::int32_t initialTermId)
    {
        m_initialTermId = initialTermId;
    }

    /**
     * The offset at which the frame begins.
     *
     * @return offset at which the frame begins.
     */
    inline util::index_t offset() const
    {
        return m_offset;
    }

    inline void offset(util::index_t offset)
    {
        m_offset = offset;
    }

    /**
     * The AtomicBuffer containing the header.
     *
     * @return AtomicBuffer containing the header.
     */
    inline AtomicBuffer& buffer()
    {
        return m_buffer;
    }

    inline void buffer(AtomicBuffer& buffer)
    {
        m_buffer.wrap(buffer);
    }

    /**
     * The total length of the frame including the header.
     *
     * @return the total length of the frame including the header.
     */
    inline std::int32_t frameLength() const
    {
        // TODO: add LITTLE_ENDIAN check
        return m_buffer.getInt32(m_offset);
    }

    /**
     * The session ID to which the frame belongs.
     *
     * @return the session ID to which the frame belongs.
     */
    inline std::int32_t sessionId() const
    {
        // TODO: add LITTLE_ENDIAN check
        return m_buffer.getInt32(m_offset + DataFrameHeader::SESSION_ID_FIELD_OFFSET);
    }

    /**
     * The stream ID to which the frame belongs.
     *
     * @return the stream ID to which the frame belongs.
     */
    inline std::int32_t streamId() const
    {
        // TODO: add LITTLE_ENDIAN check
        return m_buffer.getInt32(m_offset + DataFrameHeader::STREAM_ID_FIELD_OFFSET);
    }

    /**
     * The term ID to which the frame belongs.
     *
     * @return the term ID to which the frame belongs.
     */
    inline std::int32_t termId() const
    {
        // TODO: add LITTLE_ENDIAN check
        return m_buffer.getInt32(m_offset + DataFrameHeader::TERM_ID_FIELD_OFFSET);
    }

    /**
     * The offset in the term at which the frame begins. This will be the same as {@link #offset()}
     *
     * @return the offset in the term at which the frame begins.
     */
    inline std::int32_t termOffset() const
    {
        return m_offset;
    }

    /**
     * The type of the the frame which should always be {@link DataFrameHeader::HDR_TYPE_DATA}
     *
     * @return type of the the frame which should always be {@link DataFrameHeader::HDR_TYPE_DATA}
     */
    inline std::uint16_t type()
    {
        // TODO: add LITTLE_ENDIAN check
        return m_buffer.getUInt16(m_offset + DataFrameHeader::TYPE_FIELD_OFFSET);
    }

    /**
     * The flags for this frame. Valid flags are {@link DataFrameHeader::BEGIN_FLAG}
     * and {@link DataFrameHeader::END_FLAG}. A convenience flag {@link DataFrameHeader::BEGIN_AND_END_FLAGS}
     * can be used for both flags.
     *
     * @return the flags for this frame.
     */
    inline std::uint8_t flags()
    {
        return m_buffer.getUInt8(m_offset + DataFrameHeader::FLAGS_FIELD_OFFSET);
    }

    /**
     * Get the current position to which the Image has advanced on reading this message.
     *
     * @return the current position to which the Image has advanced on reading this message.
     */
    inline std::int64_t position() const
    {
        const std::int32_t resultingOffset = util::BitUtil::align(termOffset() + frameLength(), FrameDescriptor::FRAME_ALIGNMENT);
        return LogBufferDescriptor::computePosition(termId(), resultingOffset, m_positionBitsToShift, m_initialTermId);
    }

    /**
     * Get the value stored in the reserve space at the end of a data frame header.
     *
     * @return the value stored in the reserve space at the end of a data frame header.
     */
    inline std::int64_t reservedValue() const
    {
        return m_buffer.getInt64(m_offset + DataFrameHeader::RESERVED_VALUE_FIELD_OFFSET);
    }

private:
    AtomicBuffer m_buffer;
    util::index_t m_offset;
    std::int32_t m_initialTermId;
    std::int32_t m_positionBitsToShift;
};

}}}

#endif
