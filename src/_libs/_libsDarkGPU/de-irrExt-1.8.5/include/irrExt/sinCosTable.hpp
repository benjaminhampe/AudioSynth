// Copyright (C) 2002-2018 Benjamin Hampe, all rights reserved.

#ifndef HAMPE_SINCOS_LOOKUP_TABLE_HPP
#define HAMPE_SINCOS_LOOKUP_TABLE_HPP

#include <math.h>
#include <stdint.h>
#include <vector>

/// @brief Sinus and cosinus lookup table item

#pragma pack(push, 1)

template <class T>
struct TSinCosTableItem
{
    T s;
    T c;

    TSinCosTableItem() : s(0.0f), c(0.0f) {}

    TSinCosTableItem( T sinValue, T cosValue ) : s(sinValue), c(cosValue) {}
};

#pragma pack(pop)

/// @brief Sinus and cosinus lookup table
template <class T>
class TSinCosTable
{
    TSinCosTable() = delete;
    TSinCosTable( TSinCosTable const & other ) = delete;
    TSinCosTable& operator=( TSinCosTable const & other ) = delete;
public:
    explicit
    TSinCosTable(
        uint32_t count = 12,
        T rad_start = T(0.0),
        T rad_ende = T(2.0 * M_PI) )
    {
        m_Data.resize( ( count > 0 ) ? count : 1 );

        assert( getTableSize() > 0 );

        m_AngleStart = rad_start;
        m_AngleEnd = rad_ende;
        m_AngleStep = (m_AngleEnd - m_AngleStart) / T( getTableSize() );

        for ( uint32_t i = 0; i < getTableSize(); ++i )
        {
            T const phi = m_AngleStart + m_AngleStep * i;

            if ( sizeof(T) < 8 )
            {
                m_Data[i].s = sinf( phi );
                m_Data[i].c = cosf( phi );
            }
            else
            {
                m_Data[i].s = sin( phi );
                m_Data[i].c = cos( phi );
            }
        }
    }

    ~TSinCosTable() {}

    uint32_t
    getTableSize() const
    {
        return static_cast< uint32_t >( m_Data.size() );
    }

    TSinCosTableItem<T> const &
    operator[] ( uint32_t index ) const
    {
        assert( getTableSize() > 0 );
        return m_Data[ index % getTableSize() ];
    }

    TSinCosTableItem<T> &
    operator[] ( uint32_t index )
    {
        assert( getTableSize() > 0 );
        return m_Data[ index % getTableSize() ];
    }

    T getSin( uint32_t index ) const
    {
        assert( getTableSize() > 0 );
        return m_Data[ index % getTableSize() ].s;
    }

    T getCos( uint32_t index )
    {
        assert( getTableSize() > 0 );
        return m_Data[ index % getTableSize() ].c;
    }


    T const & getAngleStartRad() const { return m_AngleStart; }
    T const & getAngleEndRad() const { return m_AngleEnd; }
    T const & getAngleStepRad() const { return m_AngleStep; }

private:
    T m_AngleStart;
    T m_AngleEnd;
    T m_AngleStep;
    std::vector< TSinCosTableItem<T> > m_Data;
};

typedef TSinCosTable< float > SinCosTablef;

#endif // HAMPE_SINCOS_LOOKUP_TABLE_HPP
