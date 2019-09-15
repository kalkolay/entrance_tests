#include <map>
#include <limits>

template<typename K, typename V>
class interval_map {
    std::map<K,V> m_map;

public:
    // constructor associates whole range of K with val by inserting (K_min, val)
    // into the map
    interval_map( V const& val) {
        m_map.insert(m_map.end(),std::make_pair(std::numeric_limits<K>::lowest(),val));
    }

    // Assign value val to interval [keyBegin, keyEnd).
    // Overwrite previous values in this interval.
    // Conforming to the C++ Standard Library conventions, the interval
    // includes keyBegin, but excludes keyEnd.
    // If !( keyBegin < keyEnd ), this designates an empty interval,
    // and assign must do nothing.
    void assign( K const& keyBegin, K const& keyEnd, V const& val ) {
        // INSERT YOUR SOLUTION HERE

        if ( !(keyBegin < keyEnd) )
            return;

        auto lowerStart = m_map.lower_bound(keyBegin);
        auto upperStart = m_map.upper_bound(keyBegin);
        auto delStart = lowerStart;

        auto lowerEnd = m_map.lower_bound(keyEnd);
        auto upperEnd = m_map.upper_bound(keyEnd);
        typename std::map<K, V>::iterator delEnd;

        if (lowerEnd == upperEnd)
            --lowerEnd;
        delEnd = lowerEnd;
        V valueEnd = delEnd->second;

        m_map.erase(delStart, ++delEnd);

        m_map.insert( std::begin(m_map), std::make_pair(keyBegin, val) );
        m_map.insert( std::begin(m_map), std::make_pair(keyEnd, valueEnd) );

        // END (unfortunately, does not match Canonicity criteria)
    }

    // look-up of the value associated with key
    V const& operator[]( K const& key ) const {
        return ( --m_map.upper_bound(key) )->second;
    }
};

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of unsigned int intervals to char.

int main()
{

    return 0;
}
